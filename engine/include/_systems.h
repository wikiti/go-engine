/*
 * systems.h
 *
 *  Created on: 22/08/2013
 *      Author: Dani
 */

#ifndef __SYSTEMS_H_
#define __SYSTEMS_H_

#include "systems/_data.h"
#include "systems/_manager.h"
#include "systems/_debug.h"
#include "systems/_render.h"
#include "systems/_resource.h"
#include "systems/_other.h"
#include "systems/_mixer.h"
#include "systems/_shader.h"
#include "systems/_input.h"

bool Systems_Init();
void Systems_Close();
bool Systems_Reset();

void Systems_OnEvent();
void Systems_OnKeyEvent();
void Systems_OnLoop();
void Systems_OnRender();


#endif /* __SYSTEMS_H_ */
