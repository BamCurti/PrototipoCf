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
int banderaADC = 0, temperaturaADC = 0, posicionTaza = 0;

int main(void) {

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();

    config();
    printf("\rIngrese cualquier numero para empezar programa: ");
    scanf("%d", &menu);
    tazaATope(ARRIBA, &posicionTaza);
    timer(1,1);

    while(true) {
    	printf("\r1.- Programa principal 2.-Condiciones 3.- calentar el agua 4.- echar agua\n\r");
    	scanf("%d", &menu);

    	if(menu == 1){
    		leerTemperatura(&banderaADC, &temperaturaADC);
    		printf("\rtemperatura: %d \n"
    				"\rPosicion: %d\n", temperaturaADC, posicionTaza);
    		//Ingrese proceso principal
    	}
    	if(menu == 2)	condicionesParaPrepararCafe(&posicionTaza, &temperaturaADC, &banderaADC);
    	if(menu == 3)	calentarAgua(&banderaADC, &temperaturaADC);
    	if(menu == 4)  	prenderTermoTaza;
    }

    return 0;
}

void ADC0_IRQHandler(void)
{
	temperaturaADC=ADC0->R[0];//Aplicar ADC al valor del sensor
	temperaturaADC /= 100; //dejar valor en tres digitos
	banderaADC = 1;//Encendemos bandera
}



