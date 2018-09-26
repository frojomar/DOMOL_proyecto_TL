/*
 * TADArbol.h
 *
 *  Created on: 17 may. 2018
 *      Author: javier3rm
 */

#ifndef TADARBOL_H
#define TADARBOL_H

#include <cstdlib>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "TADArbol.h"
#include "TADTipoTabla.h"
#include <math.h>

using namespace std;

typedef char tipo_cadena[50];


/*SI EL NODO ES UNA HOJA, EL VALOR SERÁ TOMADO COMO TAL*/
/*SI EL NODO NO ES UNA HOJA, EL VALOR HARÁ APLICAR LA OPERACION APROPIADA*/
#define SUMA            0
#define RESTA           1
#define MULTIPLICACION  2
#define DIVISION        3
#define MODULO          4
#define POTENCIA        5
#define MAS_UNARIO      6
#define MENOS_UNARIO    7

#define PAR_IZQ       10
#define PAR_DER       11



#define MENOR         20
#define MENOR_IGUAL   21
#define MAYOR         22
#define MAYOR_IGUAL   23
#define IGUAL         24
#define DISTINTO      25
#define OR            26
#define AND           27
#define NOT           28

typedef char tipo_cadena[50];

struct tipo_valorArbol{
    float valor;
    bool valorLogico;
    tipo_cadena nombreID;
};

struct nodoArbol{
  nodoArbol * hijoIzq;
  tipo_valorArbol dato;
  bool esID;
  bool esLogico;
  nodoArbol * hijoDer;
};

typedef nodoArbol * arbolBinario;



/* DESC:   Crea un arbol juntando los dos arboles pasados, y poniendo en el nodo
    el valor entero pasado.
 * PRE:    -
 * POST:   nodo creado
 * PARAM:   hijoIzq -> arbol que ocupará la posicion de hijo izquierdo
            hijoDer -> arbol que ocupara la posicion de hijo derecho
            valor -> valor que tomará el nuevo nodo
 * RET:    arbol resultante
 * COMP:   O(1)
 */
arbolBinario crearEntero(arbolBinario hijoIzq, int valor, arbolBinario hijoDer);

/* DESC:   Crea un arbol juntando los dos arboles pasados, y poniendo en el nodo
    el valor real pasado.
 * PRE:    -
 * POST:   nodo creado
 * PARAM:   hijoIzq -> arbol que ocupará la posicion de hijo izquierdo
            hijoDer -> arbol que ocupara la posicion de hijo derecho
            valor -> valor que tomará el nuevo nodo
 * RET:    arbol resultante
 * COMP:   O(1)
 */
arbolBinario crearReal(arbolBinario hijoIzq, float valor, arbolBinario hijoDer);

/* DESC:   Crea un arbol juntando los dos arboles pasados, y poniendo en el nodo
    el nombre de la variable pasado.
 * PRE:    -
 * POST:   nodo creado
 * PARAM:   hijoIzq -> arbol que ocupará la posicion de hijo izquierdo
            hijoDer -> arbol que ocupara la posicion de hijo derecho
            id -> nombre de la variable
 * RET:    arbol resultante
 * COMP:   O(1)
 */
arbolBinario crearCadena(arbolBinario hijoIzq, tipo_cadena id, arbolBinario hijoDer );

/* DESC:   Crea un arbol juntando los dos arboles pasados, y poniendo en el nodo
    el valor logico pasado.
 * PRE:    -
 * POST:   nodo creado
 * PARAM:   hijoIzq -> arbol que ocupará la posicion de hijo izquierdo
            hijoDer -> arbol que ocupara la posicion de hijo derecho
            logico -> valor que tomará el nuevo nodo
 * RET:    arbol resultante
 * COMP:   O(1)
 */
arbolBinario crearBool(arbolBinario hijoIzq, bool logico, arbolBinario hijoDer );


/* DESC:   Comprueba si un arbol es una hoja (sus hijos son NULL)
 * PRE:    arbol creado, no a NULL
 * POST:   -
 * PARAM:
 * RET:    True si es hoja y false si no lo es
 * COMP:   O(1)
 */
bool esHoja(arbolBinario arbol);


/* DESC:   Muestra el arbol por el fichero de salida
 * PRE:    arbol creado, no a NULL
 * POST:   -
 * PARAM: fsal-> flujo de salida asociado al fichero para mostrar el arbol
 * RET:   -
 * COMP:   O(n)
 */
void mostrarArbol(arbolBinario arbol, ofstream & fsal);

/* DESC:   Muestra el arbol por consola
 * PRE:    arbol creado, no a NULL
 * POST:   -
 * PARAM:
 * RET:   -
 * COMP:   O(n)
 */
void mostrarArbol(arbolBinario arbol);

/* DESC:   Obtiene el resultado de un arbol con valor lógico
 * PRE:    arbol creado, no a NULL
 * POST:   -
 * PARAM: variables -> tabla de simbolos
          expresion -> arbol a resolver
          S: correcto -> indica si el arbol, o alguno de los arboles hijo, no
            se ha podido resolver correctamente, por encontrarse con una variable
            sin inicializar o un árbol mal formado.
 * RET:   el valor logico resultante de resolver el arbol
 * COMP:   O(n)
 */
bool resolverArbolLogico(tipo_tabla variables, arbolBinario expresion, bool &correcto);

/* DESC:   Obtiene el resultado de un arbol con valor numérico
 * PRE:    arbol creado, no a NULL
 * POST:   -
 * PARAM: variables -> tabla de simbolos
          expresion -> arbol a resolver
          S: correcto -> indica si el arbol, o alguno de los arboles hijo, no
            se ha podido resolver correctamente, por encontrarse con una variable
            sin inicializar o un árbol mal formado.
 * RET:   el valor numérico resultante de resolver el arbol
 * COMP:   O(n)
 */
float resolverArbolFloat(tipo_tabla variables, arbolBinario expresion, bool &correcto);





#endif /* TADARBOL_H_ */
