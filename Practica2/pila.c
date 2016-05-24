/*
 * pila.c
 *
 *  Created on: 31/10/2015
 *      Author: Jorge
 */

/*--- ficheros de cabecera ---*/
#include "44b.h"
#include "44blib.h"
#include <inttypes.h>




int inicioPila=_ISR_STARTADDRESS-0xf00-256;
int limite=_ISR_STARTADDRESS-0xf00;
int *actual;


/*--- declaracion de funciones ---*/
void push_debug(int ID_evento, int auxData);
void pila_Init();
int Timer0_Leer(void);


void pila_Init(){
	actual=_ISR_STARTADDRESS-0xf00-256;
}


void push_debug(int ID_evento, int auxData){
	if(actual+3>limite){
		actual=inicioPila;
	}
	*actual = Timer0_Leer();
	actual ++;

	*actual = ID_evento;
	actual++;

	*actual = auxData;
	actual++;
}
