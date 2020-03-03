 /**
 * @file    CAFETERA FAST LINE.c
 * @brief   Application entry point.
 */
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL25Z4.h"
#include "fsl_debug_console.h"
#include "functions.h"

void ADC0_IRQHandler(void);

int menu;
int banderaADC, temperaturaADC, posicionTaza = 0;

int main(void) {

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();

    config();
    printf("Ingrese cualquier numero para empezar programa: ");
    scanf("%d", &menu);
    tazaATope(ARRIBA, &posicionTaza);
    timer(1,1);

    while(true) {
    	printf("1.- Programa principal 2.-Pruebas\n");
    	scanf("%d", &menu);
    	if(menu == 1){
    		//Ingrese proceso principal
    	}

    	else{
    		printf("");
    		scanf("%d", &menu);
    	}

    }

    return 0;
}

void ADC0_IRQHandler(void)
{
	temperaturaADC=ADC0->R[0];//Aplicar ADC al valor del sensor
	temperaturaADC /= 100; //dejar valor en tres digitos
	banderaADC = 1;//Encendemos bandera
}



