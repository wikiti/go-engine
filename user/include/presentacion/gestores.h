/*
 * gestores.h
 *
 *  Created on: 25/04/2014
 *      Author: Dani
 */

#ifndef GESTORES_H_
#define GESTORES_H_

#include "_components.h"
#include "_systems.h"
#include "_globals.h"
#include "engine/_engine.h"

#include "predefined_functions.h"

void Camara_main_rotacion(CGameObject* gameObject);
void Camara_main_input(CGameObject* gameObject);
void Planeta_Movimiento(CGameObject* gameObject);
void Crate_Movimiento(CGameObject* gameObject);
void Movimiento_Rotatorio(CGameObject* gameObject);
void Firework_Manager_Comportamiento(CGameObject* gameObject);

void Camera_BeforeRender(CGameObject* gameObject);
void Camera_AfterRender(CGameObject* gameObject);

#endif /* GESTORES_H_ */
