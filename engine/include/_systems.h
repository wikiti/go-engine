/**
 * @file
 * @brief Fichero que incluye todos los tipos de sistemas.
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

/**
 * @brief Iniciar sistemas.
 *
 * LLamado automáticamente desde gEngine.
 * @return true si todos se inician correctamente, false en caso contrario.
 */
bool Systems_Init();
/**
 * @brief Cerrar sistemas.
 *
 * LLamado automáticamente desde gEngine.
 */
void Systems_Close();
/**
 * @brief Reiniciar sistemas.
 *
 * LLamado automáticamente desde gEngine.
 * @return true si todos se reinician correctamente, false en caso contrario.
 */
bool Systems_Reset();

/**
 * @brief Gestionar eventos de sistemas.
 *
 * LLamado automáticamente desde gEngine.
 */
void Systems_OnEvent();
/**
 * @brief Gestionar entrada para los sistemas.
 *
 * LLamado automáticamente desde gEngine.
 */
void Systems_OnKeyEvent();
/**
 * @brief Gestionar comportamiento de sistemas.
 *
 * LLamado automáticamente desde gEngine.
 */
void Systems_OnLoop();
/**
 * @brief Gestionar renderizado de sistemas.
 *
 * LLamado automáticamente desde gEngine.
 */
void Systems_OnRender();

#endif /* __SYSTEMS_H_ */
