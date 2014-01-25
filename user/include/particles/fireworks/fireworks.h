#ifndef FIREWORKS_H_
#define FIREWORKS_H_

#include "_components.h"
#include "_systems.h"
#include "_globals.h"
#include "engine/_engine.h"

#include "predefined_functions.h"

bool SetGameObjects_Instance1_Fireworks();

void Firework_Manager_KeyEvent(CGameObject* gameObject);
void Firework_Manager_Behaviour(CGameObject* gameObject);

#endif /* FIREWORKS_H_ */
