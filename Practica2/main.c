/*********************************************************************************************
* Fichero:	main.c
* Autor:
* Descrip:	punto de entrada de C
* Version:  <P4-ARM.timer-leds>
*********************************************************************************************/

/*--- ficheros de cabecera ---*/
#include "44blib.h"
#include "44b.h"
#include "stdio.h"


/*--- funciones externas ---*/
extern void timer_init();
extern void Eint4567_init();
extern void D8Led_init();
extern void exception_init();
extern void Timer2_Inicializar();
extern int Timer2_Leer();
extern void Timer2_Empezar();
extern void push_debug(int ID_evento, int auxData);
extern void pila_Init();
extern void Timer3_Inicializar(void);
extern void Timer0_Empezar(void);
extern void juegoSudoku();

/*--- declaracion de funciones ---*/
void Main(void);

/*--- codigo de funciones ---*/
void Main(void)
{
	/* Inicializa controladores */
	exception_init();
	sys_init();        // Inicializacion de la placa, interrupciones y puertos
	timer_init();	   // Inicializacion del temporizador
	Eint4567_init();	// inicializamos los pulsadores. Cada vez que se pulse se verá reflejado en el 8led
	D8Led_init(); // inicializamos el 8led
	Timer2_Inicializar(); /* Configura el Timer2 */
	Timer3_Inicializar(); /* Configura el Timer2 */
	pila_Init();

	Timer0_Empezar();


	Timer2_Empezar();


	juegoSudoku();


	//push_debug(0xFFFFFFFF,Timer2_Leer());

	while(1);
}
