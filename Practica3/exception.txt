/*********************************************************************************************
* Fichero:		timer.c
* Autor:
* Descrip:		funciones de control del timer0 del s3c44b0x
* Version:
*********************************************************************************************/

#include <inttypes.h>

/*--- ficheros de cabecera ---*/
#include "44b.h"
#include "44blib.h"

/*--- funciones externas ---*/
extern void D8Led_symbol(int value);

/*--- declaracion de funciones ---*/
void exception_ISR(void) __attribute__((interrupt("SWI")));
void exception_init(void);

/*--- codigo de las funciones ---*/
void exception_ISR(void)
{
	//parpadear=1 ;
	uint32_t code=0;
/**
	asm volatile(
		"mrs %[CPSR], cpsr" : [CPSR] "=r" (codigo)
	);
*/
	asm volatile(
			"mrs %[code], CPSR\n\t"
			"and %[code], %[code], #0x1F\n\t"
			: [code] "+r" (code)
	);

	switch(code){
		case 19:
			code=1;
			break;
		case 23:
			code=2;
			break;
		case 27:
			code=3;
			break;
		default:
			code=4;
		break;
	}
	//codigo=codigo & 0x1F;
	while(1){
		Delay(4000);
		D8Led_symbol(16);
		Delay(4000);
		D8Led_symbol(code);
	}
}

void exception_init(void)
{

	//se escribe la direccion de la funcion en el elemento correspondiente a undef
	//en la tabla de vectores

	pISR_UNDEF = (unsigned) exception_ISR;
	pISR_SWI = (unsigned) exception_ISR;
	pISR_PABORT = (unsigned) exception_ISR;
	pISR_DABORT = (unsigned) exception_ISR;

}

