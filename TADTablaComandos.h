/*
 * TADTablaComandos.h
 *
 *  Created on: 17 may. 2018
 *      Author: javier3rm
 */

#ifndef TADTABLACOMANDOS_H_
#define TADTABLACOMANDOS_H_

#include <cstdlib>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "TADArbol.h"
#include "TADEjemplosAux.h"
#include "TADTablaTransiciones.h"


using namespace std;

#define NO_DEFINIDO -1
#define NUM_ESTADOS_MAX 20

//definicion de los tipos de comandos
#define ASIGNACION_  0
#define ACTIVAR_    1
#define DESACTIVAR_ 2
#define TRANSICION_  3
#define SI_SIMPLE   4
#define SI_NO       5




typedef char tipo_cadena[50];


/*****************************************/
/****** TABLA CORRESPONDENCIA ***********/
/***************************************/

/*Estructura que almacenará la correspondencia entre las intrucciones y
  una determinada zona (estado, if, else, ...)*/
struct instruccionesZona{
  int primeraInstruccion; //numero de la primera instruccion para la zona
  int numInstrucciones; //numero de instrucciones que tendrá dicha zona
};

typedef  instruccionesZona tablaCorrespondencia [NUM_ESTADOS_MAX];

////////////////////////////////////////////
///////////////////////////////////////////



/*****************************************/
/****** TABLA INSTRUCCIONES   ***********/
/***************************************/

/*Estructura que almacena la informacion de distinto tipo que puede tener un
  parametro de una instruccion*/
struct tipo_parametro{
  arbolBinario expresion; //expresion aritmetica o logica
  instruccionesZona instrucciones1; //INSTRUCCIONES PARA SI (DE SI_NO) O SI_SIMPLE
  instruccionesZona instrucciones2; //INSTRUCCIONES PARA NO (DE SI_NO)
  tipo_cadena nombre; //nombre de una variable, sensor,...
};

/*Estructura que almacena la informacion para una instruccion*/
struct tipo_instruccion{
    int numInstruccion;
    int comando;
    tipo_parametro param1;
    tipo_parametro param2;
};

struct nodo_Instruccion{
  nodo_Instruccion * next;
  tipo_instruccion instruccion;
  nodo_Instruccion * before;
};
typedef nodo_Instruccion * tablaInstrucciones;

////////////////////////////////////////////
///////////////////////////////////////////



/****************************************************************************/
/****** COMANDOS = TABLA CORRESPONDENCIA + TABLA INSTRUCCIONES   ***********/
/**************************************************************************/

/*Estructura principal, que almacenará el vector de correspondencia
  instrucciones-estado, y la tabla donde estan codificadas las instrucciones*/
struct tablaComandosEstatico{
  tablaCorrespondencia insEst;
  tablaInstrucciones instrucciones;
};
typedef tablaComandosEstatico * tablaComandos;

////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////









/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/*************************                    *******************************/
/*************************    OPERACIONES     *******************************/
/*************************                    *******************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/


/* DESC:   Inicializa todas las estructuras necesarias para la tabla de comandos.
 * PRE:    -
 * POST:   Estructuras inicializadas
 * PARAM:  -
 * RET:    -
 * COMP:   O(n)
 */
void  inicializar (tablaComandos & tComandos);

/* DESC:   Nos devuelve 'true' si no hay ninguna instruccion en la tablaInstrucciones 'instrucciones'.
No es necesario comprobar 'insEst', pues, si la tabla de instrucciones está vacía, esta informacion carece
de importancia.
* PRE:    tabla inicilizada
* POST:   -
* PARAM:  -
* RET:    -
* COMP:   O(1)
*/
bool  estaVacia(tablaComandos tComandos);


/* DESC:   Inserta una nueva instrucciona en la tablaInstrucciones de 'tComandos'.
* PRE:    tabla inicializaada
* POST:   estructura con una instruccion más al final
* PARAM:  instruccion-> instruccion a insertar
* RET:    -
* COMP:   O(n)
*/
void  insertarInstruccion(tablaComandos tComandos, tipo_instruccion intruccion);

/* DESC:   Inserta una nueva instrucciona en la tablaInstrucciones de 'tComandos'. Se usa para insertar
  la instruccion en la zona adecuada, en lugar de al final. Para instrucciones: SI_SIMPLE, SI_NO
  * PRE:    tabla inicializaada
  * POST:   estructura con una instrucción más en la posicion adecuada (según numInstruccion)
  * PARAM:  instruccionZona -> instruccion a insertar
  * RET:    -
  * COMP:   O(n)
  */
void insertarInstruccionZona(tablaComandos tComandos, tipo_instruccion instruccionZona);

/* DESC:   Modifica los parametros de 'instruccionesZona', para la celda 'estado' del vector 'insEst'
* PRE:    tabla inicializaada
* POST:   valor de la tabla de correspondencia para un estado modificado
* PARAM:  estado -> estado sobre el que modificar
          primeraInstruccion -> numero de la primera instruccion del estados
          numInstrucciones -> numero de instrucciones del estado
* RET:    -
* COMP:   O(1)
*/
void actualizarInstrEstado(tablaComandos tComandos, int estado, int primeraInstruccion, int numInstrucciones);

/* DESC:   Muestra los datos de todas las estructuras almacenadas por el flujo de salida 'fsal'
    (Tabla de comandos y de correspondencia Estado-instrucciones)
 * PRE:    tabla inicializada
 * POST:   -
 * PARAM:   fsal -> flujo de salida asociado al fichero donde debe mostrarse la información
            numEstados -> numero de estados
            estados -> array con la codificacion de los estados y su nombre
 * RET:    -
 * COMP:   O(n)
 */
void mostrarComandos (tablaComandos tComandos, ofstream & fsal, int numEstados, tipo_cadena estados []);


/***********************************************************************/
/******************* METODOS PARA TRATAR LOS EJEMPLOS ******************/
/***********************************************************************/

/* DESC:   trata un estado para un ejemplo, empezando por el estado
 * PRE:    tabla inicializada
 * POST:   -
 * PARAM:   tTransiciones -> tabla de transiciones
            variables -> tabla de simbolos (con los valores de sensores actualizados)
            numEstado -> numero de estado a tratar
            sensoresEjemplo -> vector con los valores para los sensores en el ejemplo
            ejemplo -> estructura para almacenar el resultado del ejemplo
 * RET:    -
 * COMP:   O(n)
 */
bool tratarEstado (tablaComandos tComandos, tTrans tTransiciones,  int numEstado, tipo_tabla variables,
  vectorSensores sensoresEjemplo, EjemploPuntero ejemplo);

  /* DESC:   trata un ejemplo, empezando por el estado 0
   * PRE:    tabla inicializada
   * POST:   -
   * PARAM:   tTransiciones -> tabla de transiciones
              variables -> tabla de simbolos
              sensoresEjemplo -> vector con los valores para los sensores en el ejemplo
              ejemplo -> estructura para almacenar el resultado del ejemplo
   * RET:    -
   * COMP:   O(n)
   */
bool tratarEjemplo (tablaComandos tComandos, tTrans tTransiciones,  tipo_tabla variables,
  vectorSensores sensoresEjemplo, EjemploPuntero ejemplo);

#endif /* TADTABLACOMANDOS_H_ */
