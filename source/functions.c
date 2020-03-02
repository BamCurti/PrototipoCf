/*
 * functions.c
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
#include "functions.h"

void config(){
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
    BOARD_InitDebugConsole();

    //	Habilita reloj de PORTC
    SIM->SCGC5|= SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTB_MASK|SIM_SCGC5_PORTC_MASK|SIM_SCGC5_PORTE_MASK;
    SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK | SIM_SCGC6_ADC0_MASK ;//Prender reloj timer

    //Habilitar la salida del oscilador externo para cristal de 8 MHz
    OSC0->CR |= OSC_CR_ERCLKEN_MASK;

    //Conectar el cristal de 8 MHz
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(2);
    TPM0->SC |= TPM_SC_PS(6);
    ADC0->CFG1=ADC_CFG1_ADICLK(1)|ADC_CFG1_MODE(3);//Capitulo 28 el bus corre a 25 Mhz  | MODE 3 son 16 bits
    EnableIRQ(ADC0_IRQn);

    //Selección sabores
    PORTA->PCR[9] = PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_IRQC(10) | PORT_PCR_MUX(1); //SABOR 1
    PORTA->PCR[8] = PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_IRQC(10) | PORT_PCR_MUX(1); //SABOR 2
    PORTA->PCR[5] = PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_IRQC(10) | PORT_PCR_MUX(1); //SABOR 3

    GPIOA->PDDR &= (1<<9) || (1<<8) || (1<<5);
    EnableIRQ(PORTA_IRQn);

    //Configurar puerto C en el pin 1 y 2 GPIO
    //ADC
    PORTE->PCR[20] = PORT_PCR_MUX(0);

    //SENSOR DE MOTOR TAZA
    PORTE->PCR[2] = PORT_PCR_MUX(1);

    //SENSOR DE NIVEL
    PORTB->PCR[11] = PORT_PCR_MUX(1);

    //VENTILADOR
    PORTB->PCR[10] = PORT_PCR_MUX(1);

    //MOTORES ESCOGER CAFÉ
    PORTB->PCR[1] = PORT_PCR_MUX(1);//MOTOR 1
    PORTB->PCR[2] = PORT_PCR_MUX(1);//MOTOR 2
    PORTB->PCR[3] = PORT_PCR_MUX(1);//MOTOR 3

    //MOLINO CAFÉ
    PORTB->PCR[0] = PORT_PCR_MUX(1);

    //VALVULAS
    PORTC->PCR[1] = PORT_PCR_MUX(1);//Limpieza tanque
    PORTB->PCR[9] = PORT_PCR_MUX(1);//limpieza
    PORTC->PCR[2] = PORT_PCR_MUX(1);//Cafe
    PORTE->PCR[22] = PORT_PCR_MUX(1);//Rellenar

    //RESISTENCIA
    PORTE->PCR[5] = PORT_PCR_MUX(1);

    //FILTRO
    PORTE->PCR[4] = PORT_PCR_MUX(1);

    //MOTORES A PASO

	//MOTOR ELECION
	PORTE->PCR[3] = PORT_PCR_MUX(1);//PASO
	PORTE->PCR[21] = PORT_PCR_MUX(1);//DIR
	//MOTOR TAZA
	PORTE->PCR[29] = PORT_PCR_MUX(1);//PASO
	PORTE->PCR[30] = PORT_PCR_MUX(1);//DIR

	GPIOB->PDDR|=(1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<11)|(1<<10)|(1<<9);
	GPIOB->PCOR|=(1<<1)|(1<<2)|(1<<3)|(1<<0)|(1<<11)|(1<<10)|(1<<9);

	GPIOC->PDDR|=(1<<1)|(1<<2);
	GPIOC->PCOR|=(1<<1)|(1<<2);

	//Configurar puerto C en el pin 19 como salida
	GPIOE->PDDR|=(1<<21)|(1<<22)|(1<<23)|(1<<29)|(1<<30)|(1<<5)|(1<<4)|(1<<3)|(1<<2);
	GPIOE->PCOR|=(1<<21)|(1<<22)|(1<<23)|(1<<29)|(1<<30)|(1<<5)|(1<<4)|(1<<3)|(1<<2) ;

}
void moverRodillo(){
	for (int u=700;u>0;u--)
	{
		GPIOE->PSOR|=(1<<4);
		timer(2,0);
		GPIOE->PCOR|=(1<<4);
		timer(2,0);
	}
}
void timer (unsigned int tiempo, unsigned char largo){
	if(largo==0)TPM0->MOD = 120; //800 uS
	else if(largo==1)TPM0->MOD = 62500; //500 mS
	else if(largo==2)TPM0->MOD = 500; // 350 2.8 mS
	else if(largo==3)TPM0->MOD = 90; //200uS
	else if(largo==4)TPM0->MOD = 100;
	else if(largo==5)TPM0->MOD = 63000;	//Relleno de taza
	else if(largo==6)TPM0->MOD = 60500; // SOLO PARA MOLINO

	for (unsigned char i=tiempo;i>0;i--){
		TPM0->SC |= TPM_SC_CMOD(1);
		do{} while ((TPM0->SC & (TPM_SC_TOF_MASK))==0);

		(void)TPM0->SC;
		TPM0->SC &= ~TPM_SC_TOF_MASK;//Apagar bandera

		do{} while ((TPM0->SC & (TPM_SC_TOF_MASK))==0);
		(void)TPM0->SC;
		TPM0->SC &= ~TPM_SC_TOF_MASK;//Apagar bandera
	}

	TPM0->SC &= ~(TPM_SC_CMOD(1));//Apagar tiemer
}
void molerCafe (int TempMol, int motor){
	prenderSolenoide; //prender solenoide
	timer(1,1);
	prenderMolino; //Prender molino
	timer(8,1);

	GPIOB->PSOR|=(1<<motor); //prender motor sabor
	timer(2 * TempMol ,6);
	GPIOB->PCOR|=(1<<motor); //Apagar motor sabor

	timer(20,1);
	apagarSolenoide;
	apagarMolino; //Apagar molino
}
void ADCTIMER (void){
	ADC0->SC1[0]=ADC_SC1_ADCH(0) | ADC_SC1_AIEN_MASK;     //Activar pin PTE31 || Activar interrupcion
	timer(1,1);
}
void moverTaza(int pasos, unsigned char Direccion, int* posicion){
	int j;

	if(Direccion == ARRIBA){
		GPIOE->PSOR|=(1<<30); //Pa rriba
		*posicion += pasos;
	}

	else if(Direccion == ABAJO){
		GPIOE->PCOR|=(1<<30); //Pa bajo
		*posicion -= pasos;
	}

	for (j=1; j<= 100 * pasos ;j++)
	{
		GPIOE->PSOR|=(1<<29);
		timer(1,3);

		GPIOE->PCOR|=(1<<29);
		timer(1,3);

	}
}
void moverSelector(unsigned short pasos, unsigned char Direccion){
	int j;
	if(Direccion == 0) 			GPIOE->PSOR|=(1<<21);//Izquiera
	else if(Direccion == 1) 	GPIOE->PCOR|=(1<<21);//Derecha

	for (j = 1;j<= pasos*10; j++)
	{
		GPIOE->PSOR|=(1<<3);
		timer(1,2);

		GPIOE->PCOR|=(1<<3);
		timer(1,2);
	}
}
void llenarTaza(int Tempo){

	prenderTermoTaza; //prender
	timer(25, 1); //11.5 segundos hechando agua
	prenderBombaTermo; //prender

	timer(15, 1); //otros 7.5 segundos

	apagarTermoTaza; //Apagar
	apagarBombaTermo; //Apagar
	//Apagar

	timer(30, 1); //20 segundos reposo reposo anes de rellenar
	rellenarTanque();

	//otros 105 segundos
	prenderTermoTaza; //prender termo a taza

	for(int i = 1; i <= Tempo; i++){
		timer(6, 1); //15 segundos hechando agua
		prenderBombaTermo; //prender bomba a termo

		timer(15, 1); //otros 5 segundos
		apagarBombaTermo; //apagar bomba a termo
	}

	timer(30, 1);	//ES EL UNICO A CORREGIR iempo que sigue hechandoagua pero no al tanque
	apagarTermoTaza; //apagar termo a taza
}
void enjuague(short cycle){
	printf("ENJUAGUE\n");
	timer(2,1);

	for(int i = 1; i <= cycle; i++){
		prenderValvulaEnjuague; //prender enguaje
		timer(5 ,1);// 20 seg
		apagarValvulaEnjuague; //apagar enguaje
		timer(4 ,1); //reposo 60 seg
	}
}
void tazaATope(int dir, int* posicion){
	while(!sensorRiel){
		printf("\r%d", *posicion);
		moverTaza(1, dir, posicion); //puro parriba compa pepe
	}

	if(dir == ARRIBA)
		*posicion = MAXPASOS;

	printf("\nLimite\n");
}
void calentarAgua(int *banderaADC, int *temperatura){
	printf("CALENTAR AGUA\n");
	prenderResistencia; //prender resistencia

	do{
		*banderaADC = 0;
		ADCTIMER();
		while (!banderaADC);
		printf("%d\n",*temperatura);
		*banderaADC =0;
		ADC0->SC1[0]=ADC_SC1_ADCH(31); //Desactivar ADC
	} while(*temperatura >= 442);//442

	printf("\nAgua lista");
	apagarResistencia;
}
void condicionesParaPrepararCafe(int *posicion, int *temperatura, int *banderaADC){
	unsigned char bandera = 0; //1 para taza en presión, 2 para agua llena, 4 para agua caliente

	while(!tazaEnPresion(bandera) || !tanqueLleno(bandera) || !aguaCaliente(bandera)){
		//bajar pasos necesarios
		if(*posicion != 0) //Si no se ha llegado al paso 0 o presión
			moverTaza(1, ABAJO, posicion);

		else
			bandera |= 1;

		//Llenar
		if(!sensorTanque) //tanque no está lleno
			prenderBombaTermo; //bomba a termo

		else{ //Se asume que ya está lleno
			apagarBombaTermo;
			bandera |= 2;

			//calentar agua
			*banderaADC = 0;
			ADCTIMER();
			while (!banderaADC);
			printf("temperatura: %d\n",*temperatura);
			*banderaADC = 0;
			ADC0->SC1[0]=ADC_SC1_ADCH(31); //Desactivar ADC

			if(*temperatura >= 442){
				prenderResistencia;
			}

			else{
				bandera |= 4;
				apagarResistencia;
			}
		}
	}
	printf("Condiciones listas\n");
}
void rellenarTanque(){
	//llenar el tanque antes de usarlo
	prenderBombaTermo;
	while(!sensorTanque); //lee entrada del sensor
	apagarBombaTermo;

	printf("Tanque lleno\n");
}

/*void PORTA_IRQHandler(void){
	long flags = PORTA->ISFR;
	PORTA->ISFR |= 0xFFFFFFFF;
	if (flags & (1<<9)) sabor = 1;
	if (flags & (1<<8)) sabor = 2;
	if (flags & (1<<5)) sabor = 3;
}*/

