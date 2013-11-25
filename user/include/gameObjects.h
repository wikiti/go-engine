#ifndef GAMEOBJECTS_H_
#define GAMEOBJECTS_H_

#include "_globals.h"
#include "_object.h"
#include "_components.h"
#include "_systems.h"

#include "engine/_engine.h"

#include "predefined_functions.h"
#include "random/random_stuff.h"
#include "particles/fireworks/fireworks.h"
#include "particles/rainbow/rainbow.h"

bool SetGameObjects_Instance1();
void Camara_main_movimiento(CGameObject* gameObject);
void Camara_mouse_movimiento(CGameObject* gameObject);


#endif /* GAMEOBJECTS_H_ */
