/*
 * random_stuff.h
 *
 *  Created on: 24/11/2013
 *      Author: Dani
 */

#ifndef RANDOM_STUFF_H_
#define RANDOM_STUFF_H_

#include "_globals.h"
#include "_object.h"
#include "_components.h"
#include "_systems.h"

#include "engine/_engine.h"

#include "predefined_functions.h"

bool SetGameObjects_Instance1_RandomStuff();

void Camara_second_before_render(CGameObject* gameObject);
void Camara_second_after_render(CGameObject* gameObject);

void Cubo_main_movimiento(CGameObject* gameObject);
void Cubo_second_movimiento(CGameObject* gameObject);
void Cubo_third_behaviour(CGameObject* gameObject);


void Camara_second_movimiento(CGameObject* gameObject);

void Particle_emitter_explosions(CGameObject* gameObject);

void Hada_movimiento(CGameObject* gameObject);


#endif /* RANDOM_STUFF_H_ */
