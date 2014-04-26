/*
 * general.h
 *
 *  Created on: 25/04/2014
 *      Author: Dani
 */

#ifndef GENERAL_H_
#define GENERAL_H_

#include "_components.h"
#include "_systems.h"
#include "_globals.h"
#include "engine/_engine.h"

#include "predefined_functions.h"

extern int diapositiva_actual;
const uint NUM_DIAPOSITIVAS = 10;

const colorf_t COLOR_TITULO(0.5, 0.5, 0.5, 1);
const colorf_t COLOR_TEXTO(0.95, 0.95, 0.95, 1);

typedef bool (*slide_f)();

extern vector<slide_f> diapositivas_funcs_init;
extern vector<slide_f> diapositivas_funcs_close;

bool Presentacion_Init();

bool Slide1_Init();
bool Slide1_Close();
bool Slide2_Init();
bool Slide2_Close();
bool Slide3_Init();
bool Slide3_Close();
bool Slide4_Init();
bool Slide4_Close();
bool Slide4_Init();
bool Slide4_Close();
bool Slide5_Init();
bool Slide5_Close();
bool Slide6_Init();
bool Slide6_Close();
bool Slide7_Init();
bool Slide7_Close();
bool Slide8_Init();
bool Slide8_Close();
bool Slide9_Init();
bool Slide9_Close();
bool Slide10_Init();
bool Slide10_Close();

#endif /* GENERAL_H_ */
