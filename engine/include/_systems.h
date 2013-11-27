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

bool Systems_Init();
void Systems_Close();

void Systems_OnEvent();
void Systems_OnKeyEvent();
void Systems_OnLoop();
void Systems_OnRender();


#endif /* __SYSTEMS_H_ */
