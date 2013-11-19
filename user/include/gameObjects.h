#ifndef GAMEOBJECTS_H_
#define GAMEOBJECTS_H_

#include "_globals.h"
#include "_object.h"
#include "_systems.h"

#include "predefined_functions.h"

bool SetGameObjects_Instance1();

void Camara_second_before_render(CGameObject* gameObject);
void Camara_second_after_render(CGameObject* gameObject);

void Cubo_main_movimiento(CGameObject* gameObject);
void Cubo_second_movimiento(CGameObject* gameObject);
void Cubo_second_cout(CGameObject* gameObject);
void Camara_main_behaviour(CGameObject* gameObject);
void Camara_main_movimiento(CGameObject* gameObject);
void Camara_second_movimiento(CGameObject* gameObject);


void Hada_movimiento(CGameObject* gameObject);

#endif /* GAMEOBJECTS_H_ */
