#include "systems/_network.h"
#include "systems/_debug.h"

using namespace std;

Network::CSystem_Network Network::gSystem_Network;
Network::CSystem_Network& gNetwork = Network::gSystem_Network;

Network::CBuffer::CBuffer(std::string buffer)
{
  m_buffer = buffer;
}

Network::CBuffer::CBuffer(char* buffer, int size)
{
  m_buffer = std::string(buffer, size);

}

int Network::CBuffer::toInt(bool little_endian)
{
  int result = 0;

  if(m_buffer.size() == 4)
  {
    if(little_endian)
    {
      for(int n = 4 - 1; n >= 0; n--)
      {
        result <<= 8;
        result |= (int)m_buffer[n] & 0xFF;
      }
    }
    else
    {
      for(unsigned n = 0; n < 4; n++)
      {
        result <<= 8;
        result |= (int)m_buffer[n] & 0xFF;
      }
    }
  }

  return result;
}

const char* Network::CBuffer::toStr()
{
  return m_buffer.c_str();
}

/*_____ NETWORK SYSTEM _____ */

Network::CSystem_Network::CSystem_Network(): CSystem()
{
  m_machine_type = NONE;
}

bool Network::CSystem_Network::Init()
{
  CSystem::Init();

  m_machine_type = NONE;


  m_send_data = false;

  m_recv_hash.clear();
  m_send_hash.clear();

  return true;
};

void Network::CSystem_Network::Close()
{
  CSystem::Close();

  m_machine_type = NONE;

  m_recv_hash.clear();
  m_send_hash.clear();
};

bool Network::CSystem_Network::Reset()
{
  CSystem::Reset();

  m_machine_type = NONE; // ¿Desonectar usuarios?

  m_recv_hash.clear();
  m_send_hash.clear();

  return true;
};


void Network::CSystem_Network::OnLoop()
{
  bool sent = false;

  if(m_send_data)
  {
    switch(m_machine_type)
    {
      case NONE:
        for(std::map<std::string, std::string>::iterator it = m_send_hash.begin(); it != m_send_hash.end(); ++it)
          m_recv_hash[it->first] = it->second;
        sent = true;
      break;
      default: break;
    }

    m_send_data = false;
  }

  // Limpiar si se han enviado los datos.
  m_send_hash.clear();
}

bool Network::CSystem_Network::Send(const std::string& key, const std::string& value)
{
  // Comprobar si la clave es válida
  if(!Utils::validateIdentifier(key))
  {
    gSystem_Debug.console_error_msg("Error from Network: Invalid send key \"%s\": Can only contain alphanumerics or underscores.", key.c_str());
    return false;
  }

  // Añadir al hash, y sobreescribir si ya existe
  m_send_hash[key] = value;
  m_send_data = true;

  return true;
}

std::string Network::CSystem_Network::Recv(const std::string& key)
{
  // Comprobar si existe la clave
  std::map<std::string, std::string>::iterator find;
  if( (find = m_recv_hash.find(key)) == m_recv_hash.end() )
    return "";

  return find->second;
}

void Network::CSystem_Network::ClearRecvData()
{
  m_recv_hash.clear();
}

void Network::CSystem_Network::ClearSendData()
{
  m_send_hash.clear();
}

