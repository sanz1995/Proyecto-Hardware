/*********************************************************************************************
* Fichero:		timer.c
* Autor:
* Descrip:		funciones de control del timer0 del s3c44b0x
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "44b.h"
#include "44blib.h"


/*--- declaracion de funciones ---*/
void undef_ISR(void) __attribute__((interrupt("UNDEF")));
void undef_init(void);

/*--- codigo de las funciones ---*/
void undef_ISR(void)
{
	parpadear=1;
	Timer2_Empezar();
}

void undef_init(void)
{

	//se escribe la direccion de la funcion en el elemento correspondiente a undef
	//en la tabla de vectores
	pISR_UNDEF = (unsigned) undef_ISR;
}

