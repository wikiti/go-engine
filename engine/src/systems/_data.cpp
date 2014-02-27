#include "systems/_data.h"
#include "systems/_debug.h"

CSystem_Data_Storage gSystem_Data_Storage;
CSystem_Data_Storage& gData = gSystem_Data_Storage;

bool CSystem_Data_Storage::Init()
{
  if(enabled) return true;

  // Enable, etc...
  //RemoveAll();
  //LoadConfig();
  //Load(__CSYSTEM_DATA_STORAGE_CONFIG_SAVEFILE);

  //return Load();
  LoadConfig();

  CSystem::Init();

  return true;
}

void CSystem_Data_Storage::Close()
{
  if(!enabled) return;
  CSystem::Close();

  // Disable, etc...
  SaveConfig();
  //Save(__CSYSTEM_DATA_STORAGE_CONFIG_SAVEFILE);

  RemoveAll();
}

bool CSystem_Data_Storage::Reset()
{
  SaveConfig();

  return true;
}

bool CSystem_Data_Storage::Save(const char* file)
{
  ofstream os;
  os.open(file, ios::out | ios::binary);

  if(!os.good() || os.bad())
  {
    gSystem_Debug.error("From CSystem_Data_Storage: Could not save vars in file %s", file);
    gSystem_Debug.console_error_msg("From CSystem_Data_Storage: Could not save vars in file %s", file);
    return false;
  }

  os.seekp(0, os.beg);

  // Save strings
  uint num_to_load = strings.size();
  os.write((char*)&num_to_load, sizeof(unsigned int));
  for(map<string, string>::iterator it = strings.begin(); it != strings.end(); it++)
  {
    num_to_load = it->first.size() + 1;
    os.write((char*)&num_to_load, sizeof(uint));
    os.write(it->first.c_str(), num_to_load);

    num_to_load = it->second.size() + 1;
    os.write((char*)&num_to_load, sizeof(uint));
    os.write(it->second.c_str(), num_to_load);
  }

  // Save ints
  num_to_load = ints.size();
  os.write((char*)&num_to_load, sizeof(unsigned int));
  for(map<string, int>::iterator it = ints.begin(); it != ints.end(); it++)
  {
    num_to_load = it->first.size() + 1;
    os.write((char*)&num_to_load, sizeof(uint));
    os.write(it->first.c_str(), num_to_load);

    os.write((char*)&it->second, sizeof(int));
  }

  // Save floats
  num_to_load = floats.size();
  os.write((char*)&num_to_load, sizeof(unsigned int));
  for(map<string, float>::iterator it = floats.begin(); it != floats.end(); it++)
  {
    num_to_load = it->first.size() + 1;
    os.write((char*)&num_to_load, sizeof(uint));
    os.write(it->first.c_str(), num_to_load);

    os.write((char*)&it->second, sizeof(float));
  }

  os.close();

  return true;
}

bool CSystem_Data_Storage::Load(const char* file)
{
  ifstream is;
  is.open(file, ios::in | ios::binary);

  if(!is.good() || is.bad())
  {
    gSystem_Debug.error("From CSystem_Data_Storage: Could not load vars from file %s", file);
    gSystem_Debug.console_error_msg("From CSystem_Data_Storage: Could not load vars from file %s", file);
    return false;
  }

  RemoveAll();

  is.seekg(0, is.end);
  if(is.tellg() <= 0)
  {
    is.close();
    return true;
  }
  is.seekg(0, is.beg);

  uint num_to_load = 0;
  uint size;

  // Load strings
  is.read((char*)&num_to_load, sizeof(uint));
  for(uint i = 0; i < num_to_load && num_to_load != 0; i++)
  {
    is.read((char*)&size, sizeof(uint));
    char id[size];
    is.read(id, size);

    is.read((char*)&size, sizeof(uint));
    char value[size];
    is.read(value, size);

    strings.insert(pair<string, string>(id, value));
  }

  // Load ints
  is.read((char*)&num_to_load, sizeof(uint));
  for(uint i = 0; i < num_to_load && num_to_load != 0; i++)
  {
    is.read((char*)&size, sizeof(uint));
    char id[size];
    is.read(id, size);

    int value;
    is.read((char*)&value, sizeof(int));

    ints.insert(pair<string, int>(id, value));
  }

  // Load floats
  is.read((char*)&num_to_load, sizeof(uint));
  for(uint i = 0; i < num_to_load && num_to_load != 0; i++)
  {
    is.read((char*)&size, sizeof(uint));
    char id[size];
    is.read(id, size);

    float value;
    is.read((char*)&value, sizeof(float));

    floats.insert(pair<string, float>(id, value));
  }

  is.close();
  return true;
}

void CSystem_Data_Storage::LoadConfig()
{
  if(!Load(__CSYSTEM_DATA_STORAGE_CONFIG_SAVEFILE))
  {
    gSystem_Debug.log("From CSystem_Data_Storage: Created config file %s", __CSYSTEM_DATA_STORAGE_CONFIG_SAVEFILE);
    gSystem_Debug.console_msg("From CSystem_Data_Storage: Created config file %s", __CSYSTEM_DATA_STORAGE_CONFIG_SAVEFILE);

    SetInt("__RENDER_RESOLUTION_WIDTH", __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_RESOLUTION_WIDTH);
    SetInt("__RENDER_RESOLUTION_HEIGHT", __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_RESOLUTION_HEIGHT);
    SetString("__RENDER_RESOLUTION_WINDOW_MODE", __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_RESOLUTION_WINDOW_MODE);
    SetInt("__RENDER_RESOLUTION_MULTISAMPLING", __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_RESOLUTION_MULTISAMPLING);
    SetInt("__RENDER_RESOLUTION_MULTISAMPLING_VALUE", __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_RESOLUTION_MULTISAMPLING_VALUE);

    SetInt("__RENDER_SOUND_RADIUS", __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_SOUND_RADIUS);
    SetFloat("__RENDER_SOUND_RADIUS_Z", __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_SOUND_RADIUS_Z);
    SetFloat("__RENDER_SOUND_RADIUS_X", __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_SOUND_RADIUS_X);

    SetInt("__RENDER_TRANSFORM", __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_TRANSFORM);
    SetInt("__RENDER_TRANSFORM_GRID", __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_TRANSFORM_GRID);
    SetInt("__RENDER_TRANSFORM_GRID_ROWS",  __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_TRANSFORM_GRID_ROWS);
    SetInt("__RENDER_TRANSFORM_GRID_COLS", __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_TRANSFORM_GRID_COLS);
    SetFloat("__RENDER_TRANSFORM_GRID_ROWS_SCALE", __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_TRANSFORM_GRID_ROWS_SCALE);
    SetFloat("__RENDER_TRANSFORM_GRID_COLS_SCALE", __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_RENDER_TRANSFORM_GRID_COLS_SCALE);

    SetFloat("__SOUND_VOLUME", __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_SOUND_VOLUME);
    SetFloat("__SOUND_MUSIC_VOLUME", __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_SOUND_MUSIC_VOLUME);

    SetInt("__SOUND_NUMBER_SOURCES", __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_SOUND_NUMBER_SOURCES);
    SetInt("__SOUND_NUMBER_SOURCES_ONESHOT", __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_SOUND_NUMBER_SOURCES_ONESHOT);

    SetString("__INPUT_AXIS1_UP_KEY", __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_AXIS1_UP_KEY);
    SetString("__INPUT_AXIS1_DOWN_KEY", __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_AXIS1_DOWN_KEY);
    SetString("__INPUT_AXIS1_LEFT_KEY", __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_AXIS1_LEFT_KEY);
    SetString("__INPUT_AXIS1_RIGHT_KEY", __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_AXIS1_RIGHT_KEY);
    SetString("__INPUT_AXIS2_UP_KEY", __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_AXIS2_UP_KEY);
    SetString("__INPUT_AXIS2_DOWN_KEY", __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_AXIS2_DOWN_KEY);
    SetString("__INPUT_AXIS2_LEFT_KEY", __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_AXIS2_LEFT_KEY);
    SetString("__INPUT_AXIS2_RIGHT_KEY", __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_AXIS2_RIGHT_KEY);
    SetString("__INPUT_ACTION1_KEY", __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_ACTION1_KEY);
    SetString("__INPUT_ACTION2_KEY", __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_ACTION2_KEY);
    SetString("__INPUT_ACTION3_KEY", __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_ACTION3_KEY);
    SetString("__INPUT_ACTION4_KEY", __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_ACTION4_KEY);
    SetString("__INPUT_RUN_KEY", __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_RUN_KEY);
    SetString("__INPUT_CROUCH_KEY", __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_CROUCH_KEY);
    SetString("__INPUT_JUMP_KEY", __CSYSTEM_DATA_STORAGE_DEFAULTOPTIONS_INPUT_JUMP_KEY);
  }
}

void CSystem_Data_Storage::SaveConfig()
{
  if(!Save(__CSYSTEM_DATA_STORAGE_CONFIG_SAVEFILE))
  {
    gSystem_Debug.error("From CSystem_Data_Storage: Could not save options in %s", __CSYSTEM_DATA_STORAGE_CONFIG_SAVEFILE);
    gSystem_Debug.console_error_msg("From CSystem_Data_Storage: Could not save options in %s", __CSYSTEM_DATA_STORAGE_CONFIG_SAVEFILE);
  }
  //else
    //gSystem_Debug.log("From CSystem_Data_Storage: Created config file %s", __CSYSTEM_DATA_STORAGE_CONFIG_SAVEFILE);
}


void CSystem_Data_Storage::SetString(string name_id, string value)
{
  if(name_id.size() && value.size())
    strings[name_id] = value;
}

void CSystem_Data_Storage::SetInt(string name_id, int value)
{
  if(name_id.size())
    ints[name_id] = value;
}

void CSystem_Data_Storage::SetFloat(string name_id, float value)
{
  if(name_id.size())
    floats[name_id] = value;
}

string CSystem_Data_Storage::GetString(string name_id)
{
  map<string, string>::iterator it = strings.find(name_id);
  if(it != strings.end())
    return it->second;
  else
    return __CSYSTEM_DATA_STORAGE_NOSTRING;
}

int CSystem_Data_Storage::GetInt(string name_id)
{
  map<string, int>::iterator it = ints.find(name_id);
  if(it != ints.end())
    return it->second;
  else
    return __CSYSTEM_DATA_STORAGE_NOINT;
}

float CSystem_Data_Storage::GetFloat(string name_id)
{
  map<string, float>::iterator it = floats.find(name_id);
  if(it != floats.end())
    return it->second;
  else
    return __CSYSTEM_DATA_STORAGE_NOFLOAT;
}

bool CSystem_Data_Storage::ExistsInt(string name_id)
{
  if(GetInt(name_id) != __CSYSTEM_DATA_STORAGE_NOINT)
    return true;
  else
    return false;
}

bool CSystem_Data_Storage::ExistsFloat(string name_id)
{
  if(GetFloat(name_id) != __CSYSTEM_DATA_STORAGE_NOFLOAT)
    return true;
  else
    return false;
}

bool CSystem_Data_Storage::ExistsString(string name_id)
{
  if(GetString(name_id) != __CSYSTEM_DATA_STORAGE_NOSTRING)
    return true;
  else
    return false;
}


bool CSystem_Data_Storage::RemoveString(string name_id)
{
  if(!strings.erase(name_id))
    return false;

  return true;
}

bool CSystem_Data_Storage::RemoveInt(string name_id)
{
  if(!ints.erase(name_id))
    return false;

  return true;
}

bool CSystem_Data_Storage::RemoveFloat(string name_id)
{
  if(!floats.erase(name_id))
    return false;

  return true;
}

void CSystem_Data_Storage::RemoveAll()
{
  strings.clear();
  ints.clear();
  floats.clear();
}

void CSystem_Data_Storage::RemoveUserVars()
{
  for(map<string, string>::iterator it = strings.begin(); it != strings.end(); it++)
    if(it->first[0] != '_' && it->first[1] != '_')
      strings.erase(it);

  for(map<string, int>::iterator it = ints.begin(); it != ints.end(); it++)
    if(it->first[0] != '_' && it->first[1] != '_')
      ints.erase(it);

  for(map<string, float>::iterator it = floats.begin(); it != floats.end(); it++)
    if(it->first[0] != '_' && it->first[1] != '_')
      floats.erase(it);
}
