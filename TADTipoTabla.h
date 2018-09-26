/*
 * TADTipoTabla.h
 *
 *  Created on: 16 abr. 2018
 *      Author: javier3rm
 */

#ifndef TADTIPOTABLA_H_
#define TADTIPOTABLA_H_

#include <cstdlib>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

//Estructura para almacenar todos los datos relativos a una variable o sensor
/*
CAMPO TIPO:
    variable de tipo entero 	- 0
    variable de tipo real 	- 1
    variable de tipo logico 	- 2

    sensor de tipo entero 	- 10
    sensor de tipo real 	- 11
    sensor de tipo logico 	- 12

    actuador			-3
    estado			-4
    transición			-5
*/
typedef char tipo_cadena[50];
union tipo_valor{
    int valor_entero;
    float valor_real;
    bool valor_logico;
};
struct tipo_datoTS{
    tipo_cadena nombre;
    int tipo;
    tipo_valor valor;
    bool inicializado;
};

struct nodo{
    nodo * next;
    tipo_datoTS variable;
};

typedef nodo * tipo_tabla;

/* DESC:   Inicializa la lista, si no lo está
 * PRE:    Lista apuntando a basura
 * POST:   Lista apuntando a NULL
 * PARAM:  tabla: tabla de símbolos
 * COMP:   O(1)
 */
void  inicializar (tipo_tabla &tabla);

/* DESC:   Consulta si la lista esta vacia
 * PRE:    Lista creada
 * POST:   -
 * PARAM:  tabla: tabla de símbolos
 * RET:    TRUE:  lista vacia
 *         FALSE: lista no vacia
 * COMP:   O(1)
 */
bool  estaVacia (tipo_tabla tabla);

/* DESC:   Copia la tabla 'tOriginal' en la tabla 'tCopia'
 * PRE:    tOriginal inicilizada
 * POST:   -
 * PARAM:  tOriginal -> tabla original, con datos
            tCopia -> tabla sobre la que copiaremos los datos
 * RET:
 * COMP:   O(n)
 */
void  copiarTabla (tipo_tabla & tCopia, tipo_tabla tOriginal);

/* DESC:   Inserta un elemento en la lista, al final de la misma
 * PRE:    Lista creada.
 * POST:   Lista con un elemento mas al final de la misma
 * PARAM:  tabla: tabla de símbolos
          E: dato -> Elemento a insertar
 * RET:    -
 * COMP:   O(n)
 */
void  insertar (tipo_tabla & tabla,tipo_datoTS dato);

/* DESC:   Comprueba si existe un elemento en la lista
 * PRE:    Lista creada.
 * POST:   -
 * PARAM:   tabla: tabla de símbolos
            E: nombre -> nombre del elemento a buscar
 * RET:    	TRUE: existe una variable con el nombre especificado
			FALSE: NO existe una variable con el nombre especificado
 * COMP:   O(n)
 */
bool  existe (tipo_tabla tabla,tipo_cadena nombre);

/* DESC:   Modifica el elemento de la lista con el mismo nombre que 'dato.nombre'
 * PRE:    Lista creada.
 * POST:   Lista con un elemento modificado
 * PARAM:  E: dato -> Elemento a modificar
 * RET:    	TRUE: Se ha podido modificar el elemento
 * 			FALSE: No existe el elemento, y no se ha podido modificar
 * COMP:   O(n)
 */
bool  modificar (tipo_tabla tabla,tipo_datoTS dato);

/* DESC:   Devuelve el elemento de la lista con nombre igual al pasado por parametro
 * PRE:    Lista creada.
 * POST:   -
 * PARAM:   tabla: tabla de símbolos
            S: dato ->  la variable a devolver
            E: nombre -> nombre del dato a devolver
 * RET:    bool, a true si existe el dato. Si no existe, a false.
 * COMP:   O(n)
 */
bool consultar (tipo_tabla tabla, tipo_cadena nombre, tipo_datoTS & dato);

/* DESC:   Devuelve el nombre de la transicion con id pasado por parametro
 * PRE:    Lista creada.
 * POST:   -
 * PARAM:   tabla: tabla de símbolos
            S: dato ->  la variable a devolver
            E: id -> id de la transicion a consultar
 * RET:    bool, a true si existe la transicion. Si no existe, a false.
 * COMP:   O(n)
 */
bool consultarTransicion (tipo_tabla tabla, int id, tipo_datoTS & dato);

/* DESC:   Devuelve un array, donde, el valor de la posicion, es el valor con el que se codifica el estado cuyo nombre almacena esa posicion.
 * PRE:    Lista creada.
 * POST:   -
 * PARAM:   tabla: tabla de símbolos
            S: estados -> array de los estados
            S: numEstados -> numero de estados que tiene el array
 * RET:    -
 * COMP:   O(n)
 */
void obtenerCodificacionEstados (tipo_tabla tabla, tipo_cadena estados[], int & numEstados);

/* DESC:   Devuelve un array, donde, el valor de la posicion, es el valor con el que se codifica la transicion cuyo nombre almacena esa posicion.
 * PRE:    Lista creada.
 * POST:   -
 * PARAM:   tabla: tabla de símbolos
            S: transiciones -> array de las transiciones
            S: numTransiciones -> numero de transiciones que tiene el array
 * RET:    -
 * COMP:   O(n)
 */
void obtenerCodificacionTransiciones (tipo_tabla tabla, tipo_cadena transiciones[], int & numTransiciones);


/* DESC:   Borra el elemento de la lista con el nombre pasado por parametro
 * PRE:    Lista creada.
 * POST:   Num. elementos en la lista disminuye en 1 si se puede borrar.
 * PARAM:  tabla: tabla de símbolos
            E: nombre -> nombre del dato a eliminar
 * RET:    -
 * COMP:   O(n)
 */
void borrar (tipo_tabla &tabla, tipo_cadena nombre);

/* DESC:   Muestra los datos de todas las variables almacenadas en la tabla por salida estandar,
 * PRE:    Lista creada.
 * POST:   -
 * PARAM:  tabla: tabla de símbolos
 * RET:    -
 * COMP:   O(n)
 */
void mostrar (tipo_tabla tabla);

/* DESC:   Muestra los datos de todas las variables almacenadas en la tabla por el flujo de salida 'fsal'
 * PRE:    Lista creada.
 * POST:   -
 * PARAM: tabla: tabla de símbolos
          fsal -> flujo de salida asociado al fichero donde debe mostrarse la información
 * RET:    -
 * COMP:   O(n)
 */
void mostrar (tipo_tabla tabla, ofstream & fsal);

/* DESC:   Libera la memoria ocupada por los nodos de la lista
 * PRE:    Lista creada.
 * POST:   Num. elementos en la lista = 0. Lista vacia == TRUE
 * PARAM:  tabla: tabla de símbolos
 * RET:    -
 * COMP:   O(n)
 */
void borrarMemoria(tipo_tabla &tabla);

#endif /* TADTIPOTABLA_H_ */
