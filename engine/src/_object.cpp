#include "_object.h"
#include "_components.h"
#include "systems/_manager.h"

CGameObject::CGameObject(string str)
{
  inited = false;
  enabled = false;
  name = str;
  id = -1;

  preserve = false;

  Parent = NULL;
  start = behaviour = event_behaviour = keyevent_behaviour = render = NULL;
}

CGameObject::CGameObject(): CGameObject("")
{

}

void CGameObject::Init()
{
  if(inited)
    return;

  inited = enabled = true;
  flags = gof_none;
  //id = -1;

  CallStartFunction();

  AddComponent<CComponent_Transform>();

  //functions["helloworld"] = (&CGameObject::CallFunc_HelloWorld);
}

void CGameObject::Close()
{
  if(!inited) // Si ya se ha cerrado, no volver a cerrarlo
    return;

  inited = enabled = false;
  //id = -1;
  name = "";

  for(map<int, CComponent*>::const_iterator it = components.begin(); it != components.end(); ++it)
    delete it->second;
  components.clear();

  //functions.clear();

   for(map<string, CGameObject*>::iterator it = children.begin(); it != children.end(); ++it)
     it->second->Close();
   children.clear();
}

bool dfs_search_children_loops(CGameObject* father, CGameObject* new_go)
{
  bool repetido = false;

  if(father == new_go)   // Si son el mismo, es que se ha intentado emparentar a un objeto consigo mismo
  {
    repetido = true;
    return repetido;
  }

  if(father->GetNumChildren() == 0) // sin hijos
    return repetido;

  for(uint i = 0; i < father->GetNumChildren(); i++)
    if((repetido = dfs_search_children_loops(father->GetChild(i), new_go)) == true)
      return repetido;

  return repetido;
}

bool CGameObject::AddChild(CGameObject* ch)
{
  if(ch != NULL)
  {
    // Hacer esta comprobación de forma recursiva por todos los hijos, así se evitan bucles infinitos y esas mierdas a la hora de llamar funciones
    // un dfs debe de estar bien, supongo (comprobar para todos los hijos)

    // Buscar hijos de forma recursiva
    // O mejor aún, usar un puntero a un "parent" para que un objeto solo tenga un padre. Se actualiza para que se emparenta
    // Para poner a punto lo de arriba, hay que evitar ciclos
    // Buscar un algoritmo para ello
    if(!dfs_search_children_loops(this, ch) && !dfs_search_children_loops(ch, this)) // Buscar repetidos hacia adelante y hacia atrás (importante en ambos sentidos)
    {
      children.insert(pair<string, CGameObject*>(ch->GetName(), ch));
      children[ch->GetName()]->SetParent(this);
      return true;
    }
    //cout << "BUCLE DE HIJOS!!!: " << name << " - " << children->GetName() << endl;
    return false;
  }
  return false;
}

short int CGameObject::AddChildren(const vector<CGameObject*>& children)
{
  short int output = 1;

  for(vector<CGameObject*>::const_iterator it = children.begin(); it != children.end(); ++it)
  {
    if(!AddChild((*it)))
      output = -(it - children.begin());
  }
  return output;
}

bool CGameObject::RemoveChild(string str)
{
  map<string, CGameObject*>::iterator it = children.find(str);
  if(it != children.end())
  {
    it->second->UnParent();
    children.erase(it);
    return true;
  }
  return false;
}

void CGameObject::RemoveChildren()
{
  for(map<string, CGameObject*>::iterator it = children.begin(); it != children.end(); ++it)
    it->second->UnParent();

  children.clear();
}

CGameObject* CGameObject::GetChild(string str)
{
  if(children.find(str) != children.end())
    return children[str];

  return NULL;
}

CGameObject* CGameObject::GetChild(uint index)
{
  if(index < 0 or index >= children.size())
    return NULL;

  map<string, CGameObject*>::iterator it = children.begin();
  advance(it, index);
  return it->second;
//  map<string, CGameObject*>::iterator it = children.begin();
//  for(uint i = 0; i < index; i++)
//    ++it;
//
//  if(it == children.end())
//    return NULL;
//
//  return it->second;
}

/*void CGameObject::SendMessage(CGameObject* dest, string func, input_t data, output_t o_data)
{
  if(dest == NULL)
    return;

  dest->CallFunc(func, data, o_data);
}

bool CGameObject::SendMessage(string dest, string func, input_t data, output_t o_data)
{
  CGameObject* recv = gSystem_GameObject_Manager.GetGameObject(dest);
  if(recv == NULL)
    return false;

  recv->CallFunc(func, data, o_data);
  return true;
}

void CGameObject::CallFunc(string func, input_t data, output_t o_data)
{
  map<string, callfunc_ptr>::iterator it = functions.find(func);
  if(it != functions.end())
    (this->*it->second)(data, o_data);
  else
    gSystem_Debug.error("From CGameObject: Function with name %s not found in gameObject %s (%i)", func.c_str(), name.c_str(), id);//cout << "Function with name " << func << " not found in gameObject " << name << "(" << id << ")" << endl;
}

void CGameObject::CallFunc_HelloWorld(input_t data, output_t o_data)
{
  cout << name << ": Hello World!!" << endl;
}*/

int CGameObject::AddToSystem()
{
  CGameObject* recv = gSystem_GameObject_Manager.GetGameObject(name);
  if(recv != NULL) // Si ya existe, devolvemos falso
    return -1;

  gSystem_GameObject_Manager.AddGameObject(this);

  return id;
}

bool CGameObject::RemoveFromSystem()
{
  if(id != -1 && gSystem_GameObject_Manager.RemoveGameObject(name))
  {
    id = -1;
    return true;
  }

  return false;
}

// En vez de iterar por todos los componentes, se debería iterar por los principales que se sabe que van a funcionar (o algo)
// Por ejemplo, CComponent_Mesh_Render NO debería tener keyevent, ni event, ni behaviour

void CGameObject::OnEvent()
{
  if(!enabled)
    return;

  CallEventFunction();

  for(map<int, CComponent*>::iterator it = components.begin(); it != components.end(); ++it)
    it->second->OnEvent();
}

void CGameObject::OnKeyEvent()
{
  if(!enabled)
    return;

  CallKeyEventFunction();

  for(map<int, CComponent*>::iterator it = components.begin(); it != components.end(); ++it)
    it->second->OnKeyEvent();
}

void CGameObject::OnLoop()
{
  if(!enabled)
    return;

  CallBehaviourFunction();

  for(map<int, CComponent*>::iterator it = components.begin(); it != components.end(); ++it)
    it->second->OnLoop();
}

void CGameObject::OnRender(glm::mat4 projMatrix, glm::mat4 modelViewMatrix)
{
  if(!enabled)
    return;

  //if(flags & gof_render)
  //for(map<int, CComponent*>::iterator it = components.begin(); it != components.end(); ++it)
    //it->second->OnRender();
  if(GetComponent<CComponent_Mesh_Render>())
    GetComponent<CComponent_Mesh_Render>()->OnRender(projMatrix, modelViewMatrix);

  if(GetComponent<CComponent_Particle_Emitter>())
    GetComponent<CComponent_Particle_Emitter>()->OnRender(projMatrix, modelViewMatrix);

  // Dummys
  if(GetComponent<CComponent_Dummy1>())
    GetComponent<CComponent_Dummy1>()->OnRender(projMatrix, modelViewMatrix);

  if(GetComponent<CComponent_Dummy2>())
    GetComponent<CComponent_Dummy2>()->OnRender(projMatrix, modelViewMatrix);

  if(GetComponent<CComponent_Dummy3>())
    GetComponent<CComponent_Dummy3>()->OnRender(projMatrix, modelViewMatrix);

  CallRenderFunction();
}

void CGameObject::Enable(bool recursive)
{
  enabled = true;

  if(recursive)
    for(map<string, CGameObject*>::iterator it = children.begin(); it != children.end(); ++it)
      it->second->Enable(true);
}

void CGameObject::Disable(bool recursive)
{
  enabled = false;

  if(recursive)
    for(map<string, CGameObject*>::iterator it = children.begin(); it != children.end(); ++it)
      it->second->Disable(true);
}

void CGameObject::SetState(bool state, bool recursive)
{
  enabled = state;

  if(recursive)
    for(map<string, CGameObject*>::iterator it = children.begin(); it != children.end(); ++it)
      it->second->SetState(state, true);
}

void CGameObject::Preserve(bool recursive)
{
  preserve = true;

  if(recursive)
    for(map<string, CGameObject*>::iterator it = children.begin(); it != children.end(); ++it)
      it->second->Preserve(true);
}

void CGameObject::UnPreserve(bool recursive)
{
  preserve = false;

  if(recursive)
    for(map<string, CGameObject*>::iterator it = children.begin(); it != children.end(); ++it)
      it->second->UnPreserve(true);
}

void CGameObject::SetPreserve(bool state, bool recursive)
{
  preserve = state;

  if(recursive)
    for(map<string, CGameObject*>::iterator it = children.begin(); it != children.end(); ++it)
      it->second->SetPreserve(state, true);
}

bool CGameObject::NearBy(CGameObject* go, double distance)
{
  if(go->Transform()->Position().distance_to(Transform()->Position()) < distance)
    return true;

  return false;
}

// ¿?¿?
/*void CGameObject::OnRenderDebug()
{
  if(GetComponent<CComponent_Transform>())
    GetComponent<CComponent_Transform>()->OnRender();

  // ---
}*/

/*void CGameObject::SetAuxStrings()
{
  for(map<string, callfunc_ptr>::iterator it = functions.begin(); it != functions.end(); ++it)
    functions_str.push_back(it->first);

  if(Parent)
    Parent_str = Parent->GetName();
  else
    Parent_str = __OBJECT_NO_PARENT;

  for(map<string, CGameObject*>::iterator it = children.begin(); it != children.end(); ++it)
    children_str.push_back(it->first);
}

void CGameObject::UnSetAuxStrings()
{
  for(vector<string>::iterator it = functions_str.begin(); it != functions_str.end(); ++it)
     it->clear();

  functions_str.clear();

  //Parent_str.clear();
  //children_str.clear();
}*/
