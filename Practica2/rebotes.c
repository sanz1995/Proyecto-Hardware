/*
 * rebotes.c
 *
 *  Created on: 02/11/2015
 *      Author: Jorge
 */

/*--- ficheros de cabecera ---*/
#include "44b.h"
#include "44blib.h"


/*--- variables globales ---*/
int pulsado = 0;
int gestionando=0;
int tiempoEspera=10000000;

/*--- declaracion de funciones ---*/
void timer_ISR(void) __attribute__((interrupt("IRQ")));
void timer_init(void);
