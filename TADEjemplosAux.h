/*
 * TADEjemplosAux.h
 *
 *  Created on: 17 may. 2018
 *      Author: javier3rm
 */

#ifndef TADEJEMPLOSAUX_H_
#define TADEJEMPLOSAUX_H_

#include <cstdlib>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "TADTipoTabla.h"

using namespace std;

typedef char tipo_cadena[50];

/*************************************************************/
/*TAD PARA ALMACENAR LAS SOLUCIONES DE LOS DISTINTOS EJEMPLOS*/
/*************************************************************/

struct EjemploStruct{
	tipo_cadena transicciones[50];
	int numTransicciones;
	tipo_cadena nomEjemplo;
};
typedef EjemploStruct * EjemploPuntero;

struct vectorEjemplosEstatico{
	EjemploStruct ejemplos[20];
	int numEjemplos;
};
typedef vectorEjemplosEstatico * vectorEjemplos;

/* DESC:   Inicializa el vector de ejemplos
 * PRE:    vEjemplos apuntando a basura
 * POST:   vEjemplos válido y numEjemplos=0
 * PARAM:  -
 * RET:    -
 * COMP:   O(1)
 */
void inicializarVectorEjemplos(vectorEjemplos & vEjemplos);

/* DESC:   Inserta una transicion a la lista de transiciones de un ejemplo
 * PRE:    vEjemplos inicializado
 * POST:   ejemplo con una transicion más
 * PARAM: ejemplo -> puntero al ejemplo sobre el que insertar
 					transiccion-> transicion a insertar
 * RET:    -
 * COMP:   O(1)
 */
void insertarTransiccion(EjemploPuntero ejemplo, tipo_cadena transiccion);

/* DESC:   Inserta un ejemplo a la lista de ejemplos
 * PRE:    vEjemplos inicializado
 * POST:   vEjemplos con un ejemplo más
 * PARAM:  ejemplo-> puntero al ejemplo a insertar
 * RET:    -
 * COMP:   O(1)
 */
void insertarEjemplo(vectorEjemplos vEjemplos, EjemploStruct ejemplo);

/* DESC:   Muestra todos los ejemplos almacenados en el vector de ejemplos
 * PRE:    vEjemplos inicializado
 * POST:   -
 * PARAM:  fsal-> flujo de salida para mostrar los ejemplos
 * RET:    -
 * COMP:   O(n²)
 */
void mostrarEjemplos(vectorEjemplos vEjemplos,  ofstream & fsal);

/*****************************************************************************/
/*TAD PARA ALMACENAR LOS VALORES TEMPORALES DE LOS SENSORES PARA CADA EJEMPLO*/
/******************************************************************************/

struct vectorSensoresEstatico{
	tipo_datoTS sensores[20];
	int numSensores;
};

typedef vectorSensoresEstatico * vectorSensores;


/* DESC:   Inicializa el vector de sensores
 * PRE:    vSensores apuntando a basura
 * POST:   vSensores válido y numSensores=0
 * PARAM:  -
 * RET:    -
 * COMP:   O(1)
 */
void inicializarSensoresEjemplo(vectorSensores & vSensores);

/*DESC: Inserta un sensor en la lista de sensores. Si ya existe un sensor con el
		mismo nombre, lo sobreescribe
* PRE:    vSensores inicializado
* POST:   vSensores con un sensor más, o con un sensor modificado
* PARAM:  sensor -> sensor a insertar
* RET:    -
* COMP:   O(n)
*/
void insertarSensor(vectorSensores vSensores, tipo_datoTS sensor);

/*DESC: Elimina todos los sensores del vector de sensores
* PRE:    vSensores inicilizado
* POST:   numSensores=0;
* PARAM:  -
* RET:    -
* COMP:   O(1)
*/
void resetSensores (vectorSensores vSensores);

/*DESC: Devuelve true si ese sensor está en la lista. Si no lo esta devuelve falso,
		y el campo sensor no tiene un valor válido
* PRE:    vSensores inicilizado
* POST:   -
* PARAM:  S:sensor -> sensor a devolver
					nomSensor -> nombre del sensor a consultar
* RET:    True si se ha encontrado un sensor con nombre 'nomSensor'. False si no
* COMP:   O(n)
*/
bool obtenerSensor(vectorSensores vSensores, tipo_cadena nomSensor, tipo_datoTS & sensor);


#endif /* TADEJEMPLOSAUX_H_ */
