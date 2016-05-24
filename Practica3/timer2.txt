/*
 * timer2.c
 *
 *  Created on: 23/10/2015
 *      Author: a680182
 */
#include "44b.h"

/*--- variables globales ---*/
int timer2_num_int=0;


/*--- declaracion de funciones ---*/
void Timer2_ISR(void) __attribute__((interrupt("IRQ")));
void Timer2_Inicializar(void);
void Timer2_Empezar(void);
int Timer2_Leer(void);


/*--- codigo de las funciones ---*/
void Timer2_ISR(void)
{
	timer2_num_int++;

	/* borrar bit en I_ISPC para desactivar la solicitud de interrupción*/
	rI_ISPC |= BIT_TIMER2; // BIT_TIMER2 está definido en 44b.h y pone un uno en el bit 11 que correponde al Timer2
}

void Timer2_Inicializar(void)
{
	/* Configuraion controlador de interrupciones */
	rINTMOD = 0x0; // Configura las linas como de tipo IRQ
	rINTCON = 0x1; // Habilita int. vectorizadas y la linea IRQ (FIQ no)
	rINTMSK =(~(BIT_TIMER2)) & rINTMSK; // Emascara todas las lineas excepto Timer0 y el bit global (bits 26 y 13, BIT_GLOBAL y BIT_TIMER0 están definidos en 44b.h)
//(~(BIT_TIMER2)) & rINTMSK
	/* Establece la rutina de servicio para TIMER2 */
	pISR_TIMER2 = (unsigned) Timer2_ISR;


	/* Configura el Timer2 */
	rTCFG0=0xFF00FFFF & rTCFG0;  /* preescalado a 0*/
	rTCFG1 = 0x0;			/*valor del divisor a 1/2*/
	rTCNTB2 = 0xFFFFFFFF;	/*valor inicial al maximo*/
	rTCMPB2 = 0x00000000;	/*valor de comparacion*/

	rTCON = rTCON | 0x2000; /* establecer update=manual (bit 13)*/
	rTCON = (rTCON & 0xF) | 0x9000;/* iniciar timer (bit 12) con auto-reload (bit 15) mientras desactivas manual bit (bit 13)
	 	 	 	 	 	 	 	 	 * y mantienes los bits del timer0*/
}

/*--- codigo de las funciones ---*/
void Timer2_Empezar(void)
{
	rTCNTO2 = 0xFFFFFFFF;
	timer2_num_int=0;
}

/*--- codigo de las funciones ---*/
int Timer2_Leer(void){

	long cuenta=rTCNTB2;

	cuenta-=rTCNTO2;

	cuenta+=(timer2_num_int*rTCNTB2);

	cuenta=cuenta/32;

	return cuenta;
}
