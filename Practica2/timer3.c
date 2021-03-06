/*
 * timer3.c
 *
 *  Created on: 16/11/2015
 *      Author: a680182
 */

#include "44b.h"

extern int estado;
extern int boton;
extern unsigned int int_count;

extern void D8Led_symbol(int_count);

/*--- variables globales ---*/
int timer3_num_int=0;
int mantener=0;
int reset=1;
/*--- declaracion de funciones ---*/
void Timer3_ISR(void) __attribute__((interrupt("IRQ")));
void Timer3_Inicializar(void);
void Timer3_Empezar(void);
int Timer3_Leer(void);


/*--- codigo de las funciones ---*/
void Timer3_ISR(void)
{
	timer3_num_int++;

	if(estado==1){
		if(Timer3_Leer()>100000){
			estado=2;
			Timer3_Empezar();

		}
	}else if(estado==2){

		if(Timer3_Leer()>10000){
			Timer3_Empezar();
			if(boton==1){
				if(rPDATG & 0x40){
					estado=3;
				}else{
					if(mantener){
						if(Timer2_Leer()>300000){
							Timer2_Empezar();
							int_count++;
							if(int_count==10){
								int_count=reset;
							}
							D8Led_symbol(int_count); // sacamos el valor por pantalla (m�dulo 16)
						}
					}else{
						if(Timer2_Leer()>500000){
							Timer2_Empezar();
							mantener=1;
						}
					}
				}
			}else if(boton==2){
				if(rPDATG & 0x80){
					estado=3;
				}
			}
		}
	}else if(estado==3){
		if(Timer3_Leer()>100000){
			mantener=0;
			estado=0;
			//bajar peticiones y habilitar boton
			rEXTINTPND = 0xf;				// borra los bits en EXTINTPND
			rI_ISPC   |= BIT_EINT4567;		// borra el bit pendiente en INTPND
			rINTMSK =(~(BIT_EINT4567)) & rINTMSK;
		}
	}



	/* borrar bit en I_ISPC para desactivar la solicitud de interrupci�n*/
	rI_ISPC |= BIT_TIMER3; // BIT_TIMER2 est� definido en 44b.h y pone un uno en el bit 11 que correponde al Timer2
}

void Timer3_Inicializar(void)
{
	/* Configuraion controlador de interrupciones */
	rINTMOD = 0x0; // Configura las linas como de tipo IRQ
	rINTCON = 0x1; // Habilita int. vectorizadas y la linea IRQ (FIQ no)
	rINTMSK =(~(BIT_TIMER3)) & rINTMSK; // Emascara todas las lineas excepto Timer0 y el bit global (bits 26 y 13, BIT_GLOBAL y BIT_TIMER0 est�n definidos en 44b.h)
//(~(BIT_TIMER2)) & rINTMSK
	/* Establece la rutina de servicio para TIMER2 */
	pISR_TIMER3 = (unsigned) Timer3_ISR;


	/* Configura el Timer3 */
	rTCFG0=0xFF00FFFF & rTCFG0;  /* preescalado a 0*/
	rTCFG1 = 0x0;			/*valor del divisor a 1/2*/
	rTCNTB3 = 0xFFFFFFFF;	/*valor inicial al maximo*/
	rTCMPB3 = 0x00000000;	/*valor de comparacion*/

	rTCON = rTCON | 0x20000; /* establecer update=manual (bit 17)*/
	rTCON = (rTCON & 0xF00F) | 0x90000;/* iniciar timer (bit 16) con auto-reload (bit 19) mientras desactivas manual bit (bit 17)*/
}

/*--- codigo de las funciones ---*/
void Timer3_Empezar(void)
{
	rTCNTO3 = 0xFFFFFFFF;
	timer3_num_int=0;
}

/*--- codigo de las funciones ---*/
int Timer3_Leer(void){

	long cuenta=rTCNTB3;

	cuenta-=rTCNTO3;

	cuenta+=(timer3_num_int*rTCNTB3);

	cuenta=cuenta/32;

	return cuenta;
}
