/*
 * functions.h
 *
 *  Created on: 29/02/2020
 *      Author: Edgar Vallejo Curti
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

//DEFINICIONES
enum direccion {ARRIBA, ABAJO};
#define MAXPASOS 410

#define apagarSolenoide 	GPIOB->PCOR |= (1<<10)
#define prenderSolenoide 	GPIOB->PSOR |= (1<<10)

#define apagarMolino 		GPIOB->PCOR|=(1<<0)
#define prenderMolino 		GPIOB->PSOR|=(1<<0)

#define apagarBombaTermo 	GPIOE->PCOR|=(1<<22) //bomba a termo
#define prenderBombaTermo 	GPIOE->PSOR|=(1<<22) //bomba a termo

#define apagarTermoTaza		GPIOC->PCOR|=(1<<2); //termo a taza
#define prenderTermoTaza	GPIOC->PSOR|=(1<<2); //termo a taza

#define apagarValvulaEnjuague	GPIOB->PCOR|=(1<<9); //prender enguaje
#define prenderValvulaEnjuague	GPIOB->PSOR|=(1<<9); //prender enguaje

#define prenderResistencia 	GPIOE->PSOR|=(1<<5); //prender resistencia
#define apagarResistencia 	GPIOE->PCOR|=(1<<5);

#define aguaCaliente(bandera) (bandera & 4)
#define tazaEnPresion(bandera) (bandera & 1)
#define tanqueLleno(bandera) (bandera & 2)

#define sensorTanque GPIO_ReadPinInput(GPIOB, 11)
#define sensorRiel GPIO_ReadPinInput(GPIOE, 2)

//FUNCIONES LISTAS
void config();
void ADC0_IRQHandler(void);
void moverRodillo();
void timer (unsigned int tiempo, unsigned char largo);
void molerCafe (int TempMol, int motor);
void ADCTIMER (void);
void moverTaza(int pasos, unsigned char Direccion, int* posicion);
void moverSelector(unsigned short pasos, unsigned char Direccion);
void llenarTaza(int Tempo); //Cambiar modos, para que sean dos funciones, o algo
void enjuague(short cycle); //Tiempos definidos
void tazaATope(int dir, int* posicion);	//Ver areas de mejora
void condicionesParaPrepararCafe(int *posicion, int *temperatura, int *banderaADC);

//FUNCIONES A MEJORAR
void calentarAgua(); //Por interrupción
void rellenarTanque(); //Por interrupción

#endif /* FUNCTIONS_H_ */
