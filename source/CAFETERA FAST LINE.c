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
int aux1, aux2;

int main(void) {

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();

    config();
    printf("\rIngrese cualquier numero para empezar programa: \n\r");
    scanf("%d", &menu);
    tazaATope(ARRIBA, &posicionTaza);
    timer(1,1);
//    condicionesParaPrepararCafe(&posicionTaza, &temperaturaADC, &banderaADC);

    while(true) {
        leerTemperatura(&banderaADC, &temperaturaADC);
        printf("temperatura: %d\n\r", temperaturaADC);
//    	if(temperaturaADC > 442)	condicionesParaPrepararCafe(&posicionTaza, &temperaturaADC, &banderaADC);
    	printf("\r1.- Programa principal 2.-Pruebas:\n\r");
    	scanf("%d", &menu);

    	if(menu == 1){
    		//Ingrese proceso principal
    		condicionesParaPrepararCafe(&posicionTaza, &temperaturaADC, &banderaADC);
    		molerCafe(8, 3);
    		llenarTaza(7);

    		tiempoDeGoteo(); //Llenar tanques
    		enjuague(1);

    		//PRENDER RESISTENCIA
    		timer(55, 1);
    		tazaATope(ARRIBA, &posicionTaza);
    		moverRodillo();
    		moverTaza(410, ABAJO, &posicionTaza);
    		enjuague(1);
    		timer(32,1);
    		moverTaza(35, ARRIBA, &posicionTaza);
    		moverRodillo();
    		moverTaza(35, ABAJO, &posicionTaza);

    		//APAGAR RESISTENCIA
    		apagarResistencia; //Se prende resistencia en tiempo de goteo
    		printf("CAFE LISTO\n\r");
    	}

    	if(menu == 2)
    		while(menu){
    			printf("1.-Moler café	2.-mover Rodillo	3.-Mover Taza\n\r"
    					"4.-Taza a tope	5.-Llenar Taza	6.-Enjuague\n\r"
    					"7.- Leer temp	8.-Condiciones	9.-Calentar agua\n\r"
    					"10.- Rellenar tanque\n\r"
    					"0.- Salir\n\r");
    			scanf("%d", &menu);

    			switch(menu){
    			case 1:
    				molerCafe(8, 3);
    				break;

    			case 2:
    				moverRodillo();
    				break;

    			case 3:
    				printf("Ingrese el número de pasos: ");
    				scanf("%d", &aux1);
    				printf("\n\r0 para subir, 1 para bajar: ");
    				scanf("%d", &aux2);
    				moverTaza(aux1, aux2, &posicionTaza);
    				break;

    			case 4:
    				printf("\n\r0 para subir, 1 para bajar: ");
    				scanf("%d", &aux1);
    				tazaATope(aux1, &posicionTaza);
    				break;

    			case 5:
    				llenarTaza(7);
    				break;

    			case 6:
    				enjuague(1);
    				break;

    			case 7:
    				leerTemperatura(&banderaADC, &temperaturaADC);
    				printf("\n\rtemperatura: %d\n\r", temperaturaADC);
    				break;

    			case 8:
    				condicionesParaPrepararCafe(&posicionTaza, &temperaturaADC, &banderaADC);
    				break;

    			case 9:
    				calentarAgua(&banderaADC, &temperaturaADC);
    				break;

    			case 10:
    				rellenarTanque();
    				break;
    			}
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



