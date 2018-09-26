/*
 * TADTablaTransiciones.h
 *
 *  Created on: 16 abr. 2018
 *      Author: javier3rm
 */

#ifndef TADTABLATRANSICIONES_H_
#define TADTABLATRANSICIONES_H_

#include <cstdlib>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

#define NO_DEFINIDO -1
#define TAMTABLA 20

typedef char tipo_cadena[50];

struct tablaTransiciones{
    int transiciones [TAMTABLA][TAMTABLA]; //filas == estados de entrada; columnas == estados de salida;
};

typedef tablaTransiciones * tTrans;

/* DESC:   Inicializa todas las transiciones a NO_DEFINIDO
* PRE:    matriz apuntando a basura apuntando a basura
* POST:   matriz con valores NO_DEFINIDO.
* PARAM:  -
* COMP:   O(n²)
*/
void  inicializar (tTrans & tabla);


/* DESC:   añade un nuevo valor a la transicion [estadoEntrada][estadoSalida]
* PRE:    -
* POST:   Devuelve FALSE si ya tenía un valor asignado la transicion (no se ha podido completar
      la acción, por tanto), y TRUE si no lo tenía.
* PARAM:  estadoEntrada: valor de la fila de la matriz de transiciones (estado de entrada)
          estadoSalida: valor de la columna de la matriz de transiciones (estado de salida)
          transicion: valor asignado a la transicion en cuestion. Será el valor que tome [estadoEntrada][estadoSalida]
* RET: Devuelve FALSE si ya tenía un valor asignado la transicion (no se ha podido completar
      la acción, por tanto), y TRUE si no lo tenía.
* COMP:   O(1)
*/
bool nuevaTransicion(tTrans tabla, int estadoEntrada, int estadoSalida, int transicion);

/* DESC:   devuelve el valor del estado de entrada y salida, para una transicion. Si no se encuentra la transacion, se devuelve false, si se encuentra, se devuelve true,
* PRE:    -
* POST:   -
* PARAM:   SALIDA estadoEntrada: valor de la fila de la matriz de transiciones (estado de entrada)
 	         SALIDA estadoSalida: valor de la columna de la matriz de transiciones (estado de salida)
          transicion: valor que habrá en la posicion [i][j], donde i es el valor del estado salida buscado.
* COMP:   O(n)
*/
bool estadosTransicion(tTrans tabla, int transicion, int & estadoEntrada, int & estadoSalida);

/* DESC:   devuelve el valor de la transicion que va de estadoEntrada a estadoSalida
* PRE:    -
* POST:   Devuelve el valor  de la transicion, y -1 si no lo tiene.
* PARAM:  estadoEntrada: valor de la fila de la matriz de transiciones (estado de entrada)
          estadoSalida: valor de la columna de la matriz de transiciones (estado de salida)
* RET: valor que habrá en la posicion [i][j], donde i es el valor del estadoEntrada buscado y j de estadoSalida.
* COMP:   O(1)
*/
int consultarTransicion(tTrans tabla, int estadoEntrada, int estadoSalida);

/* DESC:   devuelve el valor del estado de salida, para una transicion y una entrada. Si no hay estado de salida, devuelve -1, si no, el valor del estado.
* PRE:    -
* POST:   Devuelve el valor del estado de salida si tiene un valor asignado la transicion, y -1 si no lo tiene.
* PARAM:   estadoEntrada: valor de la fila de la matriz de transiciones (estado de entrada)
            transicion: valor que habrá en la posicion [estadoEntrada][i], donde i es el valor del estado salida buscado.
* COMP:   O(n)
*/
int estadoSalida(tTrans tabla, int estadoEntrada, int transicion);

/* DESC:   Muestra la matriz de transiciones, donde se codifican las transaciones entre estados, por el flujo de salida 'fsal'
 * PRE:    Lista creada.
 * POST:   -
 * PARAM:  fsal -> flujo de salida asociado al fichero donde debe mostrarse la información
 * RET:    -
 * COMP:   O(n²)
 */
void mostrarTransiciones (tTrans tabla, ofstream & fsal);

/* DESC:   Muestra la matriz de transiciones, donde se codifican las transaciones entre
      estados, por el flujo de salida 'fsal' (hasta numEstados)
 * PRE:    Lista creada.
 * POST:   -
 * PARAM:  fsal -> flujo de salida asociado al fichero donde debe mostrarse la información
 * RET:    -
 * COMP:   O(n²)
 */
void mostrarTransiciones (tTrans tabla, int numEstados, ofstream & fsal);

/* DESC:   Muestra la matriz de transiciones, donde se codifican las transaciones entre
      estados, por el flujo de salida 'fsal' (hasta numEstados). Cambia los ids por los nombres de estados y transiciones
 * PRE:    Lista creada.
 * POST:   -
 * PARAM:  fsal -> flujo de salida asociado al fichero donde debe mostrarse la información
 * RET:    -
 * COMP:   O(n²)
 */
void mostrarTransiciones (tTrans tabla, int numEstados, ofstream & fsal, tipo_cadena estados [], tipo_cadena transiciones []);

/* DESC:   comprueba si hay estados inaccesibles. Usa internamente un algoritmo de backtracking.
 * PRE:    Lista creada.
 * POST:   -
 * PARAM: numEstados -> cuantos estados validos tiene la matriz
 * RET:    TRUE si existen estados inaccesibles y FALSE si no existen,
 * COMP:   O(n²)
 */
bool estadosInaccesibles (tTrans tabla, int numEstados);

/* DESC:   comprueba si hay estados muertos.  Usa internamente un algoritmo de backtracking.
 * PRE:    Lista creada.
 * POST:   -
 * PARAM:   numEstados -> cuantos estados validos tiene la matriz
 * RET:    TRUE si existen estados muertos y FALSE si no existen,
 * COMP:   O(n²)
 */
bool estadosMuertos (tTrans tabla, int numEstados);


#endif /* TADTABLATRANSICIONES_H_ */
