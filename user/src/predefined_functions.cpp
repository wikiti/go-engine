#include "predefined_functions.h"

void SetTransformByVar(CGameObject* go, string var_name)
{
  string pos_var_name, angle_var_name, scale_var_name;
  pos_var_name = var_name + "_POS_";
  angle_var_name = var_name + "_ANGLE_";
  scale_var_name = var_name + "_SCALE_";

  go->transform()->SetPosition(gSystem_Data_Storage.GetFloat((pos_var_name+"X")), gSystem_Data_Storage.GetFloat((pos_var_name+"Y")), gSystem_Data_Storage.GetFloat((pos_var_name+"Z")));
  go->transform()->SetAngle(gSystem_Data_Storage.GetFloat((angle_var_name+"X")), gSystem_Data_Storage.GetFloat((angle_var_name+"Y")), gSystem_Data_Storage.GetFloat((angle_var_name+"Z")));
  go->transform()->SetScale(gSystem_Data_Storage.GetFloat((scale_var_name+"X")), gSystem_Data_Storage.GetFloat((scale_var_name+"Y")), gSystem_Data_Storage.GetFloat((scale_var_name+"Z")));
}

void DefineTramsformByVar(string var_name)
{
  string pos_var_name, angle_var_name, scale_var_name;
  pos_var_name = var_name + "_POS_";
  angle_var_name = var_name + "_ANGLE_";
  scale_var_name = var_name + "_SCALE_";

  // Position
  // Comprobar si existen para definir valores nulos
  if(!gSystem_Data_Storage.ExistsFloat(pos_var_name + "X")) gSystem_Data_Storage.SetFloat(pos_var_name + "X", 0);
  if(!gSystem_Data_Storage.ExistsFloat(pos_var_name + "Y")) gSystem_Data_Storage.SetFloat(pos_var_name + "Y", 0);
  if(!gSystem_Data_Storage.ExistsFloat(pos_var_name + "Z")) gSystem_Data_Storage.SetFloat(pos_var_name + "Z", 0);

  // Angle
  if(!gSystem_Data_Storage.ExistsFloat(angle_var_name + "X")) gSystem_Data_Storage.SetFloat(angle_var_name + "X", 0);
  if(!gSystem_Data_Storage.ExistsFloat(angle_var_name + "Y")) gSystem_Data_Storage.SetFloat(angle_var_name + "Y", 0);
  if(!gSystem_Data_Storage.ExistsFloat(angle_var_name + "Z")) gSystem_Data_Storage.SetFloat(angle_var_name + "Z", 0);

  // Scale
  if(!gSystem_Data_Storage.ExistsFloat(scale_var_name + "X")) gSystem_Data_Storage.SetFloat(scale_var_name + "X", 1.f);
  if(!gSystem_Data_Storage.ExistsFloat(scale_var_name + "Y")) gSystem_Data_Storage.SetFloat(scale_var_name + "Y", 1.f);
  if(!gSystem_Data_Storage.ExistsFloat(scale_var_name + "Z")) gSystem_Data_Storage.SetFloat(scale_var_name + "Z", 1.f);
}

