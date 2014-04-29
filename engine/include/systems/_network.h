#ifndef __SYSTEM_NETWORK_H_
#define __SYSTEM_NETWORK_H_

#include "_system.h"
#include "_globals.h"

namespace Network
{
  enum machine_t { NONE = -1, HOST, CLIENT, BOTH /* Hardcore */ };

  class CBuffer
  {
    private:
      std::string m_buffer;

    public:
      CBuffer(std::string buffer);
      CBuffer(char* buffer, int size);

      int toInt(bool little_endian = true);
      const char* toStr();

      //std::string toBinary();
  };

  // http://r3dux.org/2011/01/a-simple-sdl_net-chat-server-client/
  class CSystem_Network : public CSystem
  {
    friend class CInstance;
    friend class CSystem_Debug;

    protected:
      // En cada iteración se limpia el hash de envíos
      std::map<std::string, std::string> m_send_hash;
      std::map<std::string, std::string> m_recv_hash;

      machine_t m_machine_type; // Saber si, actualmente, se trata de un cliente, de un host, o de nada (nada por defecto)

      bool m_send_data; // Saber si hay que enviar datos.

    public:
      CSystem_Network();
      ~CSystem_Network(){};

      bool Init();
      void Close();

      bool Reset();

      // Enviar y recibir datos de verdad
      void OnLoop();

      // "Enviar" y "recibir" datos
      bool Send(const std::string& key, const std::string& value);
      std::string Recv(const std::string& key);

      void ClearSendData();
      void ClearRecvData();
  };

  extern CSystem_Network gSystem_Network;
}


extern Network::CSystem_Network& gNetwork;

#endif /* __SYSTEM_NETWORK_H_ */
