/*********************************************************************************************
* Fichero:	button.c
* Autor:
* Descrip:	Funciones de manejo de los pulsadores (EINT6-7)
* Version:
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "44blib.h"
#include "44b.h"
#include "def.h"
extern int Timer3_Empezar();
extern void push_debug(int ID_evento, int auxData);
extern int reset;

/*--- variables globales ---*/
/* int_count la utilizamos para sacar un n�mero por el 8led.
  Cuando se pulsa un bot�n sumamos y con el otro restamos. �A veces hay rebotes! */
unsigned int int_count = 0;
int estado=0;
int boton=0;
int confirmacion=0;

/*--- declaracion de funciones ---*/
void Eint4567_ISR(void) __attribute__((interrupt("IRQ")));
void Eint4567_init(void);
extern void D8Led_symbol(int value); // declaramos la funci�n que escribe en el 8led

/*--- codigo de funciones ---*/
void Eint4567_init(void)
{
	/* Configuracion del controlador de interrupciones. Estos registros est�n definidos en 44b.h */
	rI_ISPC    = 0x3ffffff;	// Borra INTPND escribiendo 1s en I_ISPC
	rEXTINTPND = 0xf;       // Borra EXTINTPND escribiendo 1s en el propio registro
	rINTMOD    = 0x0;		// Configura las linas como de tipo IRQ
	rINTCON    = 0x1;	    // Habilita int. vectorizadas y la linea IRQ (FIQ no)
	rINTMSK    = ~(BIT_GLOBAL | BIT_EINT4567 | BIT_TIMER0); // Enmascara todas las lineas excepto eint4567, el bit global y el timer0

	/* Establece la rutina de servicio para Eint4567 */
	pISR_EINT4567 = (int)Eint4567_ISR;

	/* Configuracion del puerto G */
	rPCONG  = 0xffff;        		// Establece la funcion de los pines (EINT0-7)
	rPUPG   = 0x0;                  // Habilita el "pull up" del puerto
	//rEXTINT = rEXTINT | 0x22222222;   // Configura las lineas de int. como de flanco de bajada
	rEXTINT = 0x22222222;   // Configura las lineas de int. como de flanco de bajada

	/* Por precaucion, se vuelven a borrar los bits de INTPND y EXTINTPND */
	rI_ISPC    |= (BIT_EINT4567);
	rEXTINTPND = 0xf;
}

void Eint4567_ISR(void)
{
	/* Identificar la interrupcion (hay dos pulsadores)*/
	int which_int = rEXTINTPND;
	switch (which_int)
	{
		case 4:
			push_debug(0xFFFFFFFF,0xFFFFFFFF);

			boton=1;
			rINTMSK =BIT_EINT4567 | rINTMSK; //deshabilitar boton
			Timer3_Empezar();

			estado=1;
			int_count++; // incrementamos el contador
			if(int_count==10){
				int_count=reset;
			}
			break;
		case 8:
			boton=2;
			rINTMSK =BIT_EINT4567 | rINTMSK; //deshabilitar boton
			Timer3_Empezar();

			confirmacion=1;
			estado=1;
			break;
		default:
			break;
	}
	// }
	D8Led_symbol(int_count); // sacamos el valor por pantalla (m�dulo 16)
	/* Finalizar ISR */
	rEXTINTPND = 0xf;				// borra los bits en EXTINTPND
	rI_ISPC   |= BIT_EINT4567;		// borra el bit pendiente en INTPND

	//esto
	//rINTMSK    = ~(BIT_GLOBAL | BIT_TIMER0| BIT_TIMER2);

}

