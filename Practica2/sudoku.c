/*
 * sudoku.c
 *
 *  Created on: 18/11/2015
 *      Author: a680182
 */


#include <inttypes.h>

extern int confirmacion;
extern unsigned int int_count;
extern int reset;
extern void D8Led_symbol(int_count);

// Tamaños de la cuadricula
// Se utilizan 16 columnas para facilitar la visualización
enum {NUM_FILAS = 9, NUM_COLUMNAS = 16};
// Definiciones para valores muy utilizados
enum {FALSE = 0, TRUE = 1};

typedef uint16_t CELDA;
// La información de cada celda está codificada en 16 bits
// con el siguiente formato (empezando en el bit más significativo):
// 4 MSB VALOR
// 1 bit PISTA
// 1 bit ERROR
// 1 bit no usado
// 9 LSB CANDIDATOS

const uint8_t index_reticula[9]={0,0,0,3,3,3,6,6,6};
CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]=
		{0x9800,0x6800,0x0000,0x0000,0x0000,0x0000,0x7800,0x0000,0x8800,0,0,0,0,0,0,0,
	    0x8800,0x0000,0x0000,0x0000,0x0000,0x4800,0x3800,0x0000,0x0000,0,0,0,0,0,0,0,
	    0x1800,0x0000,0x0000,0x5800,0x0000,0x0000,0x0000,0x0000,0x0000,0,0,0,0,0,0,0,
	    0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x1800,0x7800,0x6800,0,0,0,0,0,0,0,
	    0x2800,0x0000,0x0000,0x0000,0x9800,0x3800,0x0000,0x0000,0x5800,0,0,0,0,0,0,0,
	    0x7800,0x0000,0x8800,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0,0,0,0,0,0,0,
	    0x0000,0x0000,0x7800,0x0000,0x3800,0x2800,0x0000,0x4800,0x0000,0,0,0,0,0,0,0,
	    0x3800,0x8800,0x2800,0x1800,0x0000,0x5800,0x6800,0x0000,0x0000,0,0,0,0,0,0,0,
	    0x0000,0x4800,0x1800,0x0000,0x0000,0x9800,0x5800,0x2800,0x0000,0,0,0,0,0,0,0};






// modifica el valor almacenado en la celda indicada
inline void
celda_poner_valor(CELDA *celdaptr, uint8_t val) {
    *celdaptr = (*celdaptr & 0x0FFF) | ((val & 0x000F) << 12);
}




// lee el valor almacenado en la celda indicada
inline uint8_t
celda_leer_valor(CELDA celda) {
    return (celda >> 12);
}


// funcion a implementar en ARM
extern int
sudoku_recalcular_arm_arm(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]);

// funcion a implementar en ARM
extern int
sudoku_recalcular_arm_thumb(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]);

// funcion a implementar en ARM
extern int
sudoku_recalcular_arm_c(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]);

// funcion a implementar en ARM
extern int
sudoku_candidatos_arm(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS],
                        uint8_t fila, uint8_t columna);

// funcion a implementar en Thumb
extern int
sudoku_candidatos_thumb(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS],
                        uint8_t fila, uint8_t columna);


////////////////////////////////////////////////////////////////////////////////
// dada una determinada celda encuentra los posibles valores candidatos
// y guarda el mapa de bits en la celda
// retorna false si la celda esta vacia, true si contiene un valor
int					// 48 lineas de codigo		1480 instrucciones
sudoku_candidatos_c(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS],
                   uint8_t fila, uint8_t columna) {
    // iniciar candidatos
	uint16_t candidatos=0x01FF;
	uint8_t valor;
    // recorrer fila recalculando candidatos
	int i=0;
	while(i<NUM_FILAS){
		valor=celda_leer_valor(cuadricula[fila][i]);
		if((valor!=0) & (i!=columna)){
			candidatos=(~(1<<(valor-1))) & candidatos;
		}
		i++;
	}
    // recorrer columna recalculando candidatos
	i=0;
	while(i<NUM_FILAS){
		valor=celda_leer_valor(cuadricula[i][columna]);
		if((valor!=0) & (i!=fila)){
			candidatos=(~(1<<(valor-1))) & candidatos;
		}
		i++;
	}
    // recorrer region recalculando candidatos
	int bloqueF=index_reticula[fila];
	i=bloqueF;
	while(index_reticula[i]==bloqueF){
		int bloqueC=index_reticula[columna];
		int j=bloqueC;
		while((index_reticula[j]==bloqueC) & (j<NUM_FILAS)){
			valor=celda_leer_valor(cuadricula[i][j]);
			if((valor!=0) & !((i==fila) & (j==columna))){
				candidatos=(~(1<<(valor-1))) & candidatos;
			}
			j++;
		}
		i++;
	}



	cuadricula[fila][columna]=(cuadricula[fila][columna] & 0xFE00) | candidatos;

	valor=celda_leer_valor(cuadricula[fila][columna]);

	if(valor!=0){
		int mascaraValor=1<<(valor-1);
		//si el valor no esta en candidatos se marca el bit de error
		if(!(mascaraValor & candidatos)){
			cuadricula[fila][columna]=cuadricula[fila][columna] | 0x400;
		}
	}

    // retornar indicando si la celda tiene un valor o esta vacia
    if ( (cuadricula[fila][columna] & 0xF000) != 0) {
    	return TRUE;
    }else{
    	return FALSE;
    }
}

int comprobar(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS],
				CELDA objetivo[NUM_FILAS][NUM_COLUMNAS]){
	//para cada fila
	int i=0;
	while(i<NUM_FILAS){
		//para cada columna
		int j=0;
		while(j<NUM_FILAS){
			//determinar candidatos
			if(cuadricula[i][j]!=objetivo[i][j]){
				//actualizar contador de celdas vacias
				return FALSE;
			}
			j++;
		}
		i++;
	}
	return TRUE;

}




////////////////////////////////////////////////////////////////////////////////
// recalcula todo el tablero (9x9)
// retorna el numero de celdas vacias
int
sudoku_recalcular_c_c(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]) {



	int error=0;
	int vacias=0;
	//para cada fila
	int i=0;
	while(i<NUM_FILAS){
		//para cada columna
		int j=0;
		while(j<NUM_FILAS){
			//determinar candidatos
			if(!sudoku_candidatos_c(cuadricula,i,j) & error==0){
				//actualizar contador de celdas vacias
				vacias++;
			}
			if(cuadricula[i][j] & 0x400){
				error=1;
				vacias=-1;
			}

			j++;
		}
		i++;
	}
    //retornar el numero de celdas vacias
    return vacias;
}

////////////////////////////////////////////////////////////////////////////////
// recalcula todo el tablero (9x9)
// retorna el numero de celdas vacias
int
sudoku_recalcular_c_arm(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]) {
	int vacias=0;
	//para cada fila
	int i=0;
	while(i<NUM_FILAS){
		//para cada columna
		int j=0;
		while(j<NUM_FILAS){
			//determinar candidatos
			if(!sudoku_candidatos_arm(cuadricula,i,j)){
				//actualizar contador de celdas vacias
				vacias++;
			}
			j++;
		}
		i++;
	}
    //retornar el numero de celdas vacias
    return vacias;
}



////////////////////////////////////////////////////////////////////////////////
// recalcula todo el tablero (9x9)
// retorna el numero de celdas vacias
int
sudoku_recalcular_c_thumb(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS]) {
	int vacias=0;
	//para cada fila
	int i=0;
	while(i<NUM_FILAS){
		//para cada columna
		int j=0;
		while(j<NUM_FILAS){
			//determinar candidatos
			if(!(sudoku_candidatos_thumb+1)(cuadricula,i,j)){
				//actualizar contador de celdas vacias
				vacias++;
			}
			j++;
		}
		i++;
	}
    //retornar el numero de celdas vacias
    return vacias;
}





////////////////////////////////////////////////////////////////////////////////
// proceso principal del juego que recibe el tablero,
// y la señal de ready que indica que se han actualizado fila y columna
void
sudoku9x9_c_c(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS], char *ready) {
    int celdas_vacias;     //numero de celdas aun vacias

    celdas_vacias = sudoku_recalcular_c_c(cuadricula);


    if(celdas_vacias==0){
    	*ready='Y';
    }else{
    	*ready='N';
    }

}


////////////////////////////////////////////////////////////////////////////////
// proceso principal del juego que recibe el tablero,
// y la señal de ready que indica que se han actualizado fila y columna
void
sudoku9x9_c_arm(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS], char *ready) {
    int celdas_vacias;     //numero de celdas aun vacias

    celdas_vacias = sudoku_recalcular_c_arm(cuadricula);


}


////////////////////////////////////////////////////////////////////////////////
// proceso principal del juego que recibe el tablero,
// y la señal de ready que indica que se han actualizado fila y columna
void
sudoku9x9_c_thumb(CELDA cuadricula[NUM_FILAS][NUM_COLUMNAS], char *ready) {
    int celdas_vacias;     //numero de celdas aun vacias

    celdas_vacias = sudoku_recalcular_c_thumb(cuadricula);

}


void juegoSudoku(){

	//sudoku9x9_c_c(cuadricula,&ready); //32 ms
	//sudoku9x9_c_arm(cuadricula,&ready); //5.7 ms
	//sudoku9x9_c_thumb(cuadricula,&ready); //7.5 ms
	//sudoku_recalcular_arm_c(cuadricula); //31.9 ms
	//sudoku_recalcular_arm_arm(cuadricula); //13 ms
	//sudoku_recalcular_arm_thumb(cuadricula); //7.3 ms



	char ready='N';
	sudoku9x9_c_c(cuadricula,&ready);



	while(ready!='Y'){

		D8Led_symbol(15);
		int_count=0;
		while(confirmacion==0);
		confirmacion=0;
		int fila=int_count;
		D8Led_symbol(12);
		int_count=0;
		while(confirmacion==0);
		confirmacion=0;
		int columna=int_count;
		int_count=0;
		reset=0;
		D8Led_symbol(16);
		while(confirmacion==0);
		confirmacion=0;
		int valor=int_count;

		celda_poner_valor(&cuadricula[fila-1][columna-1],valor);


		sudoku9x9_c_c(cuadricula,ready);
	}

}
