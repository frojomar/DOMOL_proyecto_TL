%code requires{
	#include "TADArbol.h"
}
%code{

}
%{

#include <iostream>
#include <math.h>
#include <fstream>
#include <sstream>
#include "TADTipoTabla.h"
#include "TADTablaTransiciones.h"
#include "TADTablaComandos.h"
#include "TADEjemplosAux.h"

using namespace std;

//elementos externos al analizador sintácticos por haber sido declarados en el analizador léxico
extern int n_lineas;
extern int yylex();
extern FILE* yyin;

//banderas y variables para almacenar los datos.
int banderaTipo;
int errorModulo;
int errorDefinida;
int estadoComportamiento;
tipo_tabla tabla;
tTrans tTransicion;
tipo_datoTS dato;

//contadores para asignar ids a estados y transiciones.
int ultEstAsign=-1;
int ultTranAsign=-1;

//flujo para escribir los resultados en un fichero
ofstream fsal;


/*******************************/
/**AMPLIACION DE CASOS DE USO**/

//estructuras necesarias para almacenar los casos de uso;
typedef char nombreEstado[50]; //cada nombre de estado, tendrá, como mucho, 50 caracteres (la misma longitud que tipo_cadena)
struct casoUso{
	nombreEstado nomCaso;	//nombre del caso de uso
	nombreEstado estados[20];//cada caso de uso, tendrá, como mucho, 20 estados
	nombreEstado transiciones[20];//cada caso de uso, tendrá, como mucho, 20 transiciones
	int numEstados;
	int numTransiciones;
};
typedef casoUso * casoType;
typedef casoType vectorCasosUso[10]; //cada programa, tendrá, como mucho, 10 casos de uso
int numCasos=0;

//variables necesarias para almacenar los casos de uso;
vectorCasosUso vectorCasos;
casoType caso;
bool errorCasoUso;

//procedimiento para mostrar los casos de uso;

void mostrarCasosUso(ofstream & fsal){
	fsal<<"Comprobacion de casos de uso"<<endl;
        fsal<<"============================"<<endl;
	for(int i=0; i<numCasos; i++){
		casoType casoTemp= vectorCasos[i];
		fsal<<"CASO "<<casoTemp->nomCaso<<endl;
		for(int j=casoTemp->numTransiciones-1; j>=0; j--){
			fsal<<"    "<<casoTemp->transiciones[j]<<endl;
		}
	}
}
/********************************/
/*******************************/


/*******************************/
/**AMPLIACION DE EJEMPLOS**/

//variables necesarias para almacenar los comandos;
tablaComandos tComandos; //tabla de comandos
tipo_instruccion * instruccion; //variable para manejar las instrucciones a almacenar
int ultInstrAsign; // variable para asignar el numero de instruccion a cada instruccion
int ultInstEstadoAnterior; //variable para guardar el numero de la ultima instruccion asignada en el estado anterior

//variables para almacenar la informacion de ejemplos
vectorSensores vSensores;
vectorEjemplos vEjemplos;
EjemploStruct * ejemplo;

/*CONTADORES DE INSTRUCCIONES DE CADA ZONA*/

bool banderaElse;

int contadores [20]; //podremos anidar, hasta 20 zonas

int numContElse; //para contar, cuantos de los contadores, estan asociados a ELSEs (que no crearán una nueva instruccion)

void inicializarContadoresZonas(){
	for(int i=0; i<20; i++){
		contadores[i]=-1;
	}
}

void nuevoContador(){
	int i=0;
	bool creado=false;
	while(!creado && i<20){
		if(contadores[i]==-1){
			contadores[i]=0;
			creado=true;
		}
		i++;
	}
}

int getUltimoContador(){
	bool encontrado=false;
	int contador=-1;

	for(int i=19; i>=0; i--){
		if(!encontrado){
			if(contadores[i]!=-1){
				contador=contadores[i];
				encontrado=true;
			}
		}
	}
	return contador;
}

void eliminarUltimoContador(){
	bool eliminado=false;

	for(int i=19; i>=0; i--){
		if(!eliminado){
			if(contadores[i]!=-1){
				contadores[i]=-1;
				eliminado=true;
			}
		}
	}
}

void incrementarContadores(){
	for(int i=0; i<20; i++){
		if(contadores[i]!=-1){
			contadores[i]++;
		}
	}
}

int cuantosContadores(){
	int i=0;
	while(i<20 && contadores[i]!=-1){
		i++;
	}
	return i;	
} 

/*///////////////////////////////////////*/



/********************************/
/*******************************/


//definición de procedimientos auxiliares
void yyerror(const char* s){         /*    llamada por cada error sintactico de yacc */
	cout << "Error sintáctico en la línea "<< n_lineas+1<<endl;
}

int getTipo(){

	return banderaTipo;
}

void setTipo(int tipo){
	banderaTipo=tipo;
}

void setErrorModulo(int error){
	errorModulo=error;
}

void setErrorDefinida(int error){
	errorDefinida=error;
}
%}

%union{
    int c_entero;
    float c_real;
    char c_cadena[20];
    bool c_logica;
    arbolBinario arbolExpresion;
}

%start programa

%token <c_entero> NUMERO
%token <c_real> REAL
%token <c_cadena> ID
%token <c_logica> LOGICA

%token <c_cadena> TIPO_REAL
%token <c_cadena> TIPO_ENTERO
%token <c_cadena> TIPO_BOOL

%token <c_cadena> VARIABLES
%token <c_cadena> SENSORES
%token <c_cadena> ACTUADORES
%token <c_cadena> ESTADOS
%token <c_cadena> TRANSICIONES
%token <c_cadena> COMPORTAMIENTO
%token <c_cadena> SI
%token <c_cadena> SINO
%token <c_cadena> ACTIVAR
%token <c_cadena> DESACTIVAR
%token <c_cadena> TRANSICION
%token <c_cadena> ASIGNACION
%token <c_cadena> CASO
%token <c_cadena> CASOSUSO
%token <c_cadena> EJEMPLO
%token <c_cadena> EJEMPLOS

%token <c_cadena> INICIOLLAVES
%token <c_cadena> FINLLAVES

%token <c_cadena> S_MENORIGUAL
%token <c_cadena> S_MAYORIGUAL
%token <c_cadena> S_MENOR
%token <c_cadena> S_MAYOR
%token <c_cadena> S_IGUAL
%token <c_cadena> S_DISTINTO
%token <c_cadena> S_AND
%token <c_cadena> S_OR
%token <c_cadena> S_NOT


%type <c_real> expr
%type <c_logica> exprLogica
%type <c_logica> id_logica



%type <arbolExpresion> expr_arbol
%type <arbolExpresion> exprLogica_arbol
%type <arbolExpresion> id_logica_arbol

/*OPERADORES PARA LOGICOS*/
%left S_DISTINTO S_IGUAL S_MENORIGUAL S_MENOR S_MAYORIGUAL S_MAYOR /*asociativo por la izquierda*/
%left S_OR /*asociativo por la izquierda*/
%left S_AND /*asociativo por la izquierda*/
%right S_NOT /*asociativo por la derecha*/

/*OPERADORES PARA ENTEROS Y REALES*/
%left '+' '-'   /* asociativo por la izquierda, misma prioridad */
%left '*' '/' '%'   /* asociativo por la izquierda, prioridad alta */
%right '^' /*asociativo por la derecha*/

%left menosunario masunario /*asociativo por la izquierda, mayor prioridad*/

%left '(' /*asociativo por la izquierda*/



%%
programa:
      | zona_variables zona_sensores zona_actuadores zona_estados zona_transiciones zona_comportamiento  zona_casos_uso zona_ejemplos{}
      ;

zona_variables:	VARIABLES '\n' zona_variables_instrucciones {}
	;


zona_variables_instrucciones:							{}
			|zona_variables_instrucciones operacion_asignacion 	{}
			;

operacion_asignacion: ID '=' expr '\n'		{
							if(errorModulo==1){
								cout<<"Error semántico en la instrucción " <<n_lineas<<": el operador MODULO no se puede usar con datos de tipo real"<<endl;
							}
							else{
								if(errorDefinida==1){
									//nada que hacer. Mostramos mensaje antes.
								}
								else{
									if(existe(tabla,$1)){
										consultar(tabla, $1, dato);
										if(dato.tipo==0 && banderaTipo!=0){
											cout<<"Error semántico en la instruccion "<<n_lineas<<", la variable "<<$1<<" es de tipo entero y no se le puede asignar un valor real"<<endl;
											//asignacion de real a un entero. No controlamos logicos, pues logicos siempre entrarán por ID '=' exprLogica \n
										}
										else{
											if(banderaTipo==0){
												if(dato.tipo==0){//si la expresion es entera y la variable entera
											//fsal << "La instrucción "<<n_lineas<<" hace que la variable "<<$1<<", de tipo entero, tenga el valor "<<$3<<endl;
													dato.valor.valor_entero=$3;
													dato.tipo=0;
												}
												if(dato.tipo==1){//si la expresion es entera y la variable real
											//fsal << "La instrucción "<<n_lineas<<" hace que la variable "<<$1<<", de tipo real, tenga el valor "<<$3<<endl;
													dato.valor.valor_real=$3;
													dato.tipo=1;
												}
											}
											if(banderaTipo==1){
												//fsal << "La instrucción "<<n_lineas<<" hace que la variable "<<$1<<", de tipo real, tenga el valor "<<$3<<endl;
												dato.valor.valor_real=$3;
												dato.tipo=1;
											}
											strcpy(dato.nombre, $1);
											dato.inicializado=true;
											if(!modificar(tabla,dato)){
												cout<<"ERROR en la linea "<<n_lineas<<":No se ha podido modificar la variable "<<dato.nombre<<" - "<<$1<<endl;
											}
										}
									}
									else{
										if(banderaTipo==0){
											//fsal << "La instrucción "<<n_lineas<<" hace que la variable "<<$1<<", de tipo entero, tenga el valor "<<$3<<endl;
											dato.valor.valor_entero=$3;
											dato.tipo=0;
										}
										if(banderaTipo==1){
											//fsal << "La instrucción "<<n_lineas<<" hace que la variable "<<$1<<", de tipo real, tenga el valor "<<$3<<endl;
											dato.valor.valor_real=$3;
											dato.tipo=1;
										}
										strcpy(dato.nombre, $1);
										dato.inicializado=true;
										insertar(tabla,dato);
									}
								//mostrar(tabla,fsal);
								}

							}
							setTipo(0);
							setErrorModulo(0);
							setErrorDefinida(0);
							}
			 	|ID '=' exprLogica '\n' {
							if(errorModulo==1){
								cout<<"Error semántico en la instrucción " <<n_lineas<<": el operador MODULO no se puede usar con datos de tipo real"<<endl;
							}
							else{
								if(errorDefinida==1){
									//nada que hacer. Mostramos mensaje antes.
								}
								else{
									if(existe(tabla,$1)){
										consultar(tabla, $1, dato);
										if(dato.tipo!=2){
											cout<<"Error semántico en la instruccion " <<n_lineas<<", la variable "<<$1<<" no es de tipo logico"<<endl;
										}
										else{
											if($3==0){
												//fsal << "La instrucción "<<n_lineas<<" hace que la variable "<<$1<<", de tipo logico, tenga el valor false"<<endl;
												dato.valor.valor_logico=false;
											}
											if($3==1){
												//fsal << "La instrucción "<<n_lineas<<" hace que la variable "<<$1<<", de tipo logico, tenga el valor true"<<endl;
												dato.valor.valor_logico=true;
											}
											strcpy(dato.nombre, $1);
											dato.tipo=2;
											dato.inicializado=true;
											if(!modificar(tabla,dato)){
												cout<<"ERROR en la linea "<<n_lineas<<":No se ha podido modificar la variable "<<dato.nombre<<" - "<<$1<<endl;
											}
										}
									}
									else{
										if($3==0){
											//fsal << "La instrucción "<<n_lineas<<" hace que la variable "<<$1<<", de tipo logico, tenga el valor false"<<endl;
											dato.valor.valor_logico=false;
										}
										if($3==1){
											//fsal << "La instrucción "<<n_lineas<<" hace que la variable "<<$1<<", de tipo logico, tenga el valor true"<<endl;
											dato.valor.valor_logico=true;
										}
										strcpy(dato.nombre, $1);
										dato.tipo=2;
										dato.inicializado=true;
										insertar(tabla,dato);
									}
								//mostrar(tabla,fsal);
								}
							}

							setTipo(0);
							setErrorModulo(0);
							setErrorDefinida(0);
							}
				|error '\n'	{yyerrok;}
				;



exprLogica:  	LOGICA		 	{$$=$1;}
       	      | expr S_MENOR expr	 		{$$=$1<$3;}
       	      | expr S_MENORIGUAL expr	 		{$$=$1<=$3;}
       	      | expr S_MAYOR expr	 		{$$=$1>$3;}
       	      | expr S_MAYORIGUAL expr	 		{$$=$1>=$3;}
       	      | expr S_IGUAL expr	 		{$$=$1==$3;}
       	      | expr S_DISTINTO expr	 		{$$=$1!=$3;}
       	      | LOGICA S_IGUAL id_logica		{$$=$1==$3;}
       	      | LOGICA S_DISTINTO id_logica		{$$=$1!=$3;}
       	      | exprLogica S_OR exprLogica	 	{$$=$1||$3;}
       	      | exprLogica S_AND exprLogica	 	{$$=$1&&$3;}
       	      | '('exprLogica')' 	 		{$$=$2;}
       	      | S_NOT exprLogica  			{$$=!$2;}
       	      ;

id_logica:  ID			{if(existe(tabla, $1)){
						consultar(tabla, $1, dato);
						if(dato.tipo==1 || dato.tipo==11){
							cout<<"Error semántico en la línea "<<n_lineas+1<<", no se pueden realizar operaciones lógicas con variables de tipo aritmético"<<endl;
							setErrorDefinida(1);
						}
						if(dato.tipo==0 || dato.tipo==10){
							cout<<"Error semántico en la línea "<<n_lineas+1<<", no se pueden realizar operaciones lógicas con variables de tipo aritmético"<<endl;
							setErrorDefinida(1);
						}
						if(dato.tipo==2 || dato.tipo==12){
							$$=dato.valor.valor_logico;
						}
					}
					else{
						cout<<"Error semántico en la instruccion "<<n_lineas+1<<", la variable "<<$1<<" no ha sido definida"<<endl;
						setErrorDefinida(1);
					}}
	;

expr:  REAL	 		{$$=$1;setTipo(1);}
       | NUMERO 		{$$=$1;}
       | ID			{if(existe(tabla, $1)){
					consultar(tabla, $1, dato);
					if(dato.inicializado==true){
						if(dato.tipo==1 || dato.tipo==11){
							setTipo(1);
							$$=dato.valor.valor_real;
						}
						if(dato.tipo==0 || dato.tipo==10){
							$$=dato.valor.valor_entero;
						}
						if(dato.tipo==2 || dato.tipo==12){
							cout<<"Error semántico en la línea "<<n_lineas+1<<", no se pueden realizar operaciones aritméticas con variables (sensores) de tipo lógico"<<endl;
							setErrorDefinida(1);
						}
					}
					else{
						cout<<"Error semántico en la línea "<<n_lineas+1<<", el sensor "<<$1<<" no ha sido inicializado"<<endl;
						setErrorDefinida(1);
					}
				}
				else{
					cout<<"Error semántico en la instruccion "<<n_lineas+1<<", la variable (o sensor) "<<$1<<" no ha sido definida"<<endl;
					setErrorDefinida(1);
				}}
       | expr '+' expr 		{$$=$1+$3;}
       | expr '-' expr    	{$$=$1-$3;}
       | expr '*' expr          {$$=$1*$3;}
       | expr '/' expr          {if(banderaTipo==0){$$=(int)$1/(int)$3;}else{if(banderaTipo==1){$$=$1/$3;}}}
       | expr '%' expr		{if(banderaTipo==0){$$=(int)$1%(int)$3;}else{setErrorModulo(1);}}
       | expr '^' expr          {$$=pow($1,$3);}
       | '('expr')'		{$$=$2;}
       | '+' expr 		%prec masunario{$$=$2;}
       | '-' expr 		%prec menosunario{$$=-$2;}
       ;



zona_sensores: 	SENSORES '\n' zona_sensores_instrucciones {}
		;

zona_sensores_instrucciones: 											{}
			|zona_sensores_instrucciones TIPO_ENTERO identificadores_sensores_enteros '\n'		{}
			|zona_sensores_instrucciones TIPO_REAL identificadores_sensores_reales '\n'		{}
			|zona_sensores_instrucciones TIPO_BOOL identificadores_sensores_bool '\n'		{}
			|zona_sensores_instrucciones error '\n'							{yyerrok;}
			;

identificadores_sensores_enteros: ID							{
											if(existe(tabla,$1)){cout<<"ERROR l."<<n_lineas<<": El sensor "<<$1<<" ya existe. No se puede volver a crear"<<endl;}
											else{
												dato.valor.valor_entero=0;
												dato.tipo=10;
												strcpy(dato.nombre, $1);
												dato.inicializado=false;
												insertar(tabla,dato);
											}
											}
				| ID ',' identificadores_sensores_enteros		{
											if(existe(tabla,$1)){cout<<"ERROR l."<<n_lineas<<": El sensor "<<$1<<" ya existe. No se puede volver a crear"<<endl;}
											else{
												dato.valor.valor_entero=0;
												dato.tipo=10;
												strcpy(dato.nombre, $1);
												dato.inicializado=false;
												insertar(tabla,dato);
											}
											}
				;

identificadores_sensores_reales:  ID							{
											if(existe(tabla,$1)){cout<<"ERROR l."<<n_lineas<<": El sensor "<<$1<<" ya existe. No se puede volver a crear"<<endl;}
											else{
												dato.valor.valor_real=0;
												dato.tipo=11;
												strcpy(dato.nombre, $1);
												dato.inicializado=false;
												insertar(tabla,dato);
											}
											}
				| ID ',' identificadores_sensores_reales		{
											if(existe(tabla,$1)){cout<<"ERROR l."<<n_lineas<<": El sensor "<<$1<<" ya existe. No se puede volver a crear"<<endl;}
											else{
												dato.valor.valor_real=0;
												dato.tipo=11;
												strcpy(dato.nombre, $1);
												dato.inicializado=false;
												insertar(tabla,dato);
											}
											}
				;

identificadores_sensores_bool:	  ID							{
											if(existe(tabla,$1)){cout<<"ERROR l."<<n_lineas<<": El sensor "<<$1<<" ya existe. No se puede volver a crear"<<endl;}
											else{
												dato.valor.valor_logico=false;
												dato.tipo=12;
												strcpy(dato.nombre, $1);
												dato.inicializado=false;
												insertar(tabla,dato);
											}
											}
				| ID ',' identificadores_sensores_bool			{

											if(existe(tabla,$1)){cout<<"ERROR l."<<n_lineas<<": El sensor "<<$1<<" ya existe. No se puede volver a crear"<<endl;}
											else{
												dato.valor.valor_logico=false;
												dato.tipo=12;
												strcpy(dato.nombre, $1);
												dato.inicializado=false;
												insertar(tabla,dato);
											}
											}
				;

zona_actuadores: ACTUADORES '\n' zona_actuadores_instrucciones {}
		;

zona_actuadores_instrucciones: 						{}
			|zona_actuadores_instrucciones ID '\n'		{
									if(existe(tabla,$2)){cout<<"ERROR l."<<n_lineas<<": El actuador "<<$2<<" ya existe. No se puede volver a crear"<<endl;}
									else{
										dato.valor.valor_entero=0;
										dato.tipo=3;
										strcpy(dato.nombre, $2);
										dato.inicializado=true;
										insertar(tabla,dato);
									}
									}
			|zona_actuadores_instrucciones error '\n'	{yyerrok;}
			;

zona_estados: ESTADOS '\n' zona_estados_instrucciones {}
	;

zona_estados_instrucciones: 					{}
			|zona_estados_instrucciones ID '\n'	{

								if(existe(tabla,$2)){cout<<"ERROR l."<<n_lineas<<": El estado "<<$2<<" ya existe. No se puede volver a crear"<<endl;}
								else{
									dato.valor.valor_entero=ultEstAsign+1;
									dato.tipo=4;
									strcpy(dato.nombre, $2);
									dato.inicializado=true;
									insertar(tabla,dato);
									ultEstAsign++;
								}
								}
			|zona_estados_instrucciones error '\n'	{yyerrok;}
			;

zona_transiciones:	TRANSICIONES '\n' zona_transiciones_instrucciones 	{
										if(estadosInaccesibles(tTransicion, ultEstAsign)){
											cout<<"Error semántico linea "<<n_lineas<<". Existen estados inaccesibles"<<endl;
										}
										if(estadosMuertos(tTransicion, ultEstAsign)){
											cout<<"Error semántico linea "<<n_lineas<<". Existen estados muertos"<<endl;
										}
										}
	;

zona_transiciones_instrucciones:							{}
		| zona_transiciones_instrucciones ID ':' ID ASIGNACION ID '\n'		{
											if(!existe(tabla,$4)){
												cout<<"ERROR l."<<n_lineas<<": El estado '"<<$4<<"' no existe."<<endl;
											}
											else{
												if(!existe(tabla,$6)){
													cout<<"ERROR l."<<n_lineas<<": El estado '"<<$6<<"' no existe."<<endl;
												}
												else{
													tipo_datoTS estado1, estado2, transicion;
													consultar(tabla, $4, estado1);
													consultar(tabla, $6, estado2);
													if(estado1.tipo!=4){
														cout<<"ERROR l."<<n_lineas<<": '"<<$4<<"' no es un estado."<<endl;
													}
													else{
														if(estado2.tipo!=4){
															cout<<"ERROR l."<<n_lineas<<": '"<<$6<<"' no es un estado."<<endl;
														}
														else{
															if(estado1.valor.valor_entero==1){
															cout<<"ERROR l."<<n_lineas<<": El estado final ('"<<$4<<"') no puede ser entrada."<<endl;
															}
															else{
																if(estado2.valor.valor_entero==0){
															cout<<"ERROR l."<<n_lineas<<": El estado inicio ('"<<$6<<"') no puede ser salida."<<endl;
																}
																else{
																	if(existe(tabla, $2)){
																		cout<<"ERROR l."<<n_lineas<<": La transicion '"<<$2<<"' ya existe"<<endl;
																	}
																	else{
																		transicion.valor.valor_entero=ultTranAsign+1;
																		transicion.tipo=5;
																		strcpy(transicion.nombre, $2);
																		transicion.inicializado=true;
																		insertar(tabla,transicion);
																		ultTranAsign++;

											if(!nuevaTransicion(tTransicion, estado1.valor.valor_entero, estado2.valor.valor_entero, transicion.valor.valor_entero)){
												cout<<"ERROR l."<<n_lineas<<": Ya existe una transicion de '"<<$4<<"' a '"<<$6<<"'"<<endl;
											}
																	}
																}
															}
														}
													}
												}
											}}
			| zona_transiciones_instrucciones error '\n'			{yyerrok;}
			;

zona_casos_uso:	CASOSUSO '\n' zona_casos_uso_instrucciones			{}
		|								{} //zona de casos de uso será opcional
		;

zona_casos_uso_instrucciones:								{}
			|zona_casos_uso_instrucciones CASO ID ':' caso_uso2		{
											if(existe(tabla, $3)){
												cout<<"Error semántico en la linea "<<n_lineas<<". El id "<<$3<<" ya está siendo usado."<<endl;
												errorCasoUso=true;
											}
											else{
												if(!errorCasoUso){												
													strcpy(caso->nomCaso, $3);
													vectorCasos[numCasos]=caso;
													numCasos++;
												}
											}
											caso=new casoUso;
											caso->numEstados=0;
											caso->numTransiciones=0;
											errorCasoUso=false;
											}
			|zona_casos_uso_instrucciones error '\n'			{yyerrok;}
			;

caso_uso2:  ID '\n'				{
						if(existe(tabla, $1)){
							consultar(tabla, $1, dato);
							if(dato.tipo!=4){
							cout<<"Error semántico en la linea "<<n_lineas<<". "<<$1<<" no es un estado."<<endl;
							errorCasoUso=true;
							}
							else{ //si existe un estado con el nombre indicado por ID...
								if(dato.valor.valor_entero=1){//y si es el estado final...
									strcpy(caso->estados[caso->numEstados],$1);
									caso->numEstados++;
								}
								else{
									cout<<"Error semántico en la linea "<<n_lineas<<". El caso de uso debe acabar con un estado final."<<endl;
									errorCasoUso=true;	
								}						
							}
						}
						else{
							cout<<"Error semántico en la linea "<<n_lineas<<". No existe el estado "<<$1<<"."<<endl;
							errorCasoUso=true;
						}
						}
	| ID';' caso_uso2			{
						if(!errorCasoUso){
							if(existe(tabla, $1)){
								consultar(tabla, $1, dato);
								if(dato.tipo!=4){
								cout<<"Error semántico en la linea "<<n_lineas<<". "<<$1<<" no es un estado."<<endl;
								errorCasoUso=true;
								}
								else{//si existe un estado con el nombre indicado por ID...
									strcpy(caso->estados[caso->numEstados],$1);
									caso->numEstados++;
									consultar(tabla, caso->estados[caso->numEstados-2],dato);
									int estadoSalida= dato.valor.valor_entero;									
									consultar(tabla, caso->estados[caso->numEstados-1],dato);
									int estadoEntrada= dato.valor.valor_entero;
									int transicion= consultarTransicion(tTransicion, estadoEntrada, estadoSalida);
									if(transicion==NO_DEFINIDO){ //si no existe transicion entre los dos estados...
										cout<<"Error semántico en la linea "<<n_lineas<<". No existe transicion de "<<caso->estados[caso->numEstados-1]<<" a "<<caso->estados[caso->numEstados-2]<<"."<<endl;							
									}
									else{
										consultarTransicion(tabla, transicion, dato);
										strcpy(caso->transiciones[caso->numTransiciones],dato.nombre);
										caso->numTransiciones++;
									}						
								}
							}
							else{
								cout<<"Error semántico en la linea "<<n_lineas<<". No existe el estado "<<$1<<"."<<endl;
								errorCasoUso=true;
							}
						}
						}
	;

zona_comportamiento:	COMPORTAMIENTO '\n' zona_comportamiento_instrucciones {}
	;

zona_comportamiento_instrucciones:										{}
				| zona_comportamiento_instrucciones ID{	nuevoContador();
									if(existe(tabla, $2)){
										consultar(tabla, $2, dato);
										if(dato.tipo!=4){
											cout<<"Error semántico en la linea "<<n_lineas<<". "<<$2<<" no es un estado."<<endl;
										}
										else{										
											estadoComportamiento=dato.valor.valor_entero;	
										}
									}
									else{
										cout<<"Error semántico en la linea "<<n_lineas<<". El estado "<<$2<<" no existe."<<endl;
									}
				} INICIOLLAVES '\n' comportamiento2 FINLLAVES '\n' 	{
											if(existe(tabla, $2)){
												consultar(tabla, $2, dato);
												if(dato.tipo!=4){
													//cout<<"Error semántico en la linea "<<n_lineas<<". "<<$2<<" no es un estado."<<endl;
												}
												else{
														int contador=getUltimoContador();

														actualizarInstrEstado(tComandos, dato.valor.valor_entero, ultInstrAsign+1-contador, contador);

														eliminarUltimoContador();

														//no incrementamos contadores, pues no debería quedar ninguno en este punto.
												}
											}
											else{
												//cout<<"Error semántico en la linea "<<n_lineas<<". El estado "<<$2<<"no existe."<<endl;
											}
											}
				|zona_comportamiento_instrucciones error '\n'  							{yyerrok;}
				;

comportamiento2: comportamiento2 SI exprLogica_arbol INICIOLLAVES{nuevoContador();} '\n' comportamiento2 FINLLAVES '\n' comportamiento3 	
														{

														instruccion->numInstruccion=ultInstrAsign+1;
														if(!banderaElse){
															instruccion->comando=SI_SIMPLE;
															instruccion->param2.instrucciones2.numInstrucciones=0;			
														}
														instruccion->param1.expresion=$3;
														instruccion->param2.instrucciones1.numInstrucciones=getUltimoContador() - instruccion->param2.instrucciones2.numInstrucciones; //a las instrucciones totales del SINO, le restamos las del NO para obtener las del SI
														
														instruccion->numInstruccion-=getUltimoContador(); //decrementamos el numero de instruccion tantas posiciones como instrucciones tenga la zona, pues se insertará realmente delante de estas.
														instruccion->numInstruccion+=cuantosContadores()-2; //incrementamos el numero de instruccion tantas posiciones como contadores tengamos en uso, sin contar el suyo mismo y el del estado. Esto se debe a que, el resto de contadores, introducirán su instrucción delante de esta, y hay que dejarle suficientes posiciones.
														ultInstrAsign++;
														eliminarUltimoContador();
														banderaElse=false;
										
														insertarInstruccionZona(tComandos, *instruccion);

	
														incrementarContadores();

														instruccion= new tipo_instruccion;
														}
		| comportamiento2 ACTIVAR ID '\n'	 							{
														if(existe(tabla, $3)){
															consultar(tabla, $3, dato);
															if(dato.tipo!=3){
															cout<<"Error semántico en la linea "<<n_lineas<<". "<<$3<<" no es un actuador"<<endl;
															}
															else{
																instruccion->numInstruccion=ultInstrAsign+1;
																instruccion->numInstruccion+=cuantosContadores()-1-numContElse; //incrementamos el numero de instrucciones tanto como contadores activos tengamos en ese momento, debido a que las instrucciones de estos contadores se insertarán delante. Se resta 1 por el contador asociado al estado, que no genera ninguna instrucción, así como los contadores de else (que solos, tampoco crearán instrucciones).
													
																instruccion->comando=ACTIVAR_;
																strcpy(instruccion->param1.nombre,$3);

																ultInstrAsign++;
											
																insertarInstruccion(tComandos, *instruccion);

																instruccion= new tipo_instruccion;
																incrementarContadores();
															}
														}
														else{
															cout<<"Error semántico en la linea "<<n_lineas<<". El actuador "<<$3<<" no existe"<<endl;
														}
														}
		| comportamiento2 DESACTIVAR ID '\n'	 							{
														if(existe(tabla, $3)){
															consultar(tabla, $3, dato);
															if(dato.tipo!=3){
															cout<<"Error semántico en la linea "<<n_lineas<<". "<<$3<<" no es un actuador"<<endl;
															}
															else{
																instruccion->numInstruccion=ultInstrAsign+1;
																instruccion->numInstruccion+=cuantosContadores()-1-numContElse; //incrementamos el numero de instrucciones tanto como contadores activos tengamos en ese momento, debido a que las instrucciones de estos contadores se insertarán delante. Se resta 1 por el contador asociado al estado, que no genera ninguna instrucción, así como los contadores de else (que solos, tampoco crearán instrucciones).
													
																instruccion->comando=DESACTIVAR_;
																strcpy(instruccion->param1.nombre,$3);

																ultInstrAsign++;
											
																insertarInstruccion(tComandos, *instruccion);

																instruccion= new tipo_instruccion;
																incrementarContadores();
															}
														}
														else{
															cout<<"Error semántico en la linea "<<n_lineas<<". El actuador "<<$3<<" no existe"<<endl;
														}
														}
		| comportamiento2 TRANSICION ID '\n'	 							{
														if(existe(tabla, $3)){
															consultar(tabla, $3, dato);
															if(dato.tipo!=5){
															cout<<"Error semántico en la linea "<<n_lineas<<". "<<$3<<" no es una transicion"<<endl;
															}
															else{
																int salida=estadoSalida(tTransicion, estadoComportamiento, dato.valor.valor_entero);
																if(salida==-1){
															cout<<"Error semántico en la linea "<<n_lineas<<". La transicion "<<$3<<" no esta definida en el ámbito"<<endl;
																}
																else{
																	instruccion->numInstruccion=ultInstrAsign+1;
																instruccion->numInstruccion+=cuantosContadores()-1-numContElse; //incrementamos el numero de instrucciones tanto como contadores activos tengamos en ese momento, debido a que las instrucciones de estos contadores se insertarán delante. Se resta 1 por el contador asociado al estado, que no genera ninguna instrucción, así como los contadores de else (que solos, tampoco crearán instrucciones).
																	instruccion->comando=TRANSICION_;
																	strcpy(instruccion->param1.nombre,$3);

																	ultInstrAsign++;
											
																	insertarInstruccion(tComandos, *instruccion);

																	instruccion= new tipo_instruccion;
																	incrementarContadores();
																}
															}
														}
														else{
															cout<<"Error semántico en la linea "<<n_lineas<<". La transicion "<<$3<<" no existe"<<endl;
														}
														}
		|comportamiento2 operacion_asignacion_arbol 	 						{}
		|												{}
		|comportamiento2 error '\n'									{yyerrok;}
		;
comportamiento3: 						{}
		| SINO INICIOLLAVES {nuevoContador();numContElse++;}'\n' comportamiento2 FINLLAVES '\n'	{
												instruccion->comando=SI_NO;
												instruccion->param2.instrucciones2.numInstrucciones=getUltimoContador();
												banderaElse=true;
												eliminarUltimoContador();
												numContElse--;
												}
		| SINO error						{yyerrok;}
		;



/************************************************************************************************************************/
/************************************************************************************************************************/
/************************************* AMPLIACION 2 - ARBOLES DE EXPRESIONES ********************************************/

zona_ejemplos:	EJEMPLOS '\n' zona_ejemplos_instrucciones 	{}
		|						{} //zona de ejemplos es opcional (igual que zona de casos de uso)
		;

zona_ejemplos_instrucciones:										{}
				| zona_ejemplos_instrucciones EJEMPLO ID '\n' ejemplos2 	{
												//tratar ejemplo e insertarlo
												strcpy(ejemplo->nomEjemplo, $3);
												if(!tratarEjemplo(tComandos, tTransicion, tabla, vSensores, ejemplo)){
													cout<<"NOTA: EJEMPLO "<<$3<<" no ha podido llegar a estado final. Hay sensores sin inicializar"<<endl;
												}
												insertarEjemplo(vEjemplos, *ejemplo);
	
												ejemplo= new EjemploStruct;	
												resetSensores(vSensores);										
												}
				|zona_ejemplos_instrucciones error '\n'  							{yyerrok;}
				;


ejemplos2: 					{}
	|ejemplos2 ID LOGICA '\n'		{if(existe(tabla,$2)){
							consultar(tabla, $2, dato);
							if(dato.tipo==12){
								if($3==0){
									dato.valor.valor_logico=false;
								}
								if($3==1){
									dato.valor.valor_logico=true;
								}
								dato.inicializado=true;
								insertarSensor(vSensores, dato);
							}
							else{
								cout<<"ERROR l."<<n_lineas<<", el sensor "<<$2<<" no es de tipo logico."<<endl;
							}
						}
						else{
							cout<<"ERROR l."<<n_lineas<<", el sensor "<<$2<<" no existe."<<endl;
						}}
	|ejemplos2 ID NUMERO '\n'		{if(existe(tabla,$2)){
							consultar(tabla, $2, dato);
							if(dato.tipo==10){
								dato.valor.valor_entero=$3;
								dato.inicializado=true;
								insertarSensor(vSensores, dato);
							}
							else{
								cout<<"ERROR l."<<n_lineas<<", el sensor "<<$2<<" no es de tipo entero."<<endl;
							}
						}
						else{
							cout<<"ERROR l."<<n_lineas<<", el sensor "<<$2<<" no existe."<<endl;
						}}
	|ejemplos2 ID REAL '\n'			{if(existe(tabla,$2)){
							consultar(tabla, $2, dato);
							if(dato.tipo==11){
								dato.valor.valor_real=$3;
								dato.inicializado=true;
								insertarSensor(vSensores, dato);
							}
							else{
								cout<<"ERROR l."<<n_lineas<<", el sensor "<<$2<<" no es de tipo real."<<endl;
							}
						}
						else{
							cout<<"ERROR l."<<n_lineas<<", el sensor "<<$2<<" no existe."<<endl;
						}}
	;

/************************************* AMPLIACION 2 - ARBOLES DE EXPRESIONES ********************************************/



operacion_asignacion_arbol: ID '=' expr_arbol '\n'		{
							if(errorModulo==1){
								cout<<"Error semántico en la instrucción " <<n_lineas<<": el operador MODULO no se puede usar con datos de tipo real"<<endl;
							}
							else{
								if(errorDefinida==1){
									//nada que hacer. Mostramos mensaje antes.
								}
								else{
									if(existe(tabla,$1)){
										consultar(tabla, $1, dato);
										if((dato.tipo==0 || dato.tipo==10) && banderaTipo!=0){
											cout<<"Error semántico en la instruccion "<<n_lineas<<", la variable "<<$1<<" es de tipo entero y no se le puede asignar un valor real"<<endl;
											//asignacion de real a un entero. No controlamos logicos, pues logicos siempre entrarán por ID '=' exprLogica \n
										}
										else{
											instruccion->numInstruccion=ultInstrAsign+1;
											instruccion->numInstruccion+=cuantosContadores()-1-numContElse; //incrementamos el numero de instrucciones tanto como contadores activos tengamos en ese momento, debido a que las instrucciones de estos contadores se insertarán delante. Se resta 1 por el contador asociado al estado, que no genera ninguna instrucción, así como los contadores de else (que solos, tampoco crearán instrucciones).
													
											instruccion->comando=ASIGNACION_;
											strcpy(instruccion->param1.nombre,$1);
											instruccion->param2.expresion=$3;

											ultInstrAsign++;
											
											insertarInstruccion(tComandos, *instruccion);

											instruccion= new tipo_instruccion;
											incrementarContadores();
										}
									}
									else{
										cout<<"Error semántico en la instruccion "<<n_lineas<<", '"<<$1<<"' no ha sido definido previamente."<<endl;
									}
								//mostrar(tabla,fsal);
								}

							}
							setTipo(0);
							setErrorModulo(0);
							setErrorDefinida(0);
							}
			 	|ID '=' exprLogica_arbol'\n'{
							if(errorModulo==1){
								cout<<"Error semántico en la instrucción " <<n_lineas<<": el operador MODULO no se puede usar con datos de tipo real"<<endl;
							}
							else{
								if(errorDefinida==1){
									//nada que hacer. Mostramos mensaje antes.
								}
								else{
									if(existe(tabla,$1)){
										consultar(tabla, $1, dato);
										if(dato.tipo!=2 && dato.tipo!=12){
											cout<<"Error semántico en la instruccion " <<n_lineas<<", la variable "<<$1<<" no es de tipo logico"<<endl;
										}
										else{
											instruccion->numInstruccion=ultInstrAsign+1;
											instruccion->numInstruccion+=cuantosContadores()-1; //incrementamos el numero de instrucciones tanto como contadores activos tengamos en ese momento, debido a que las instrucciones de estos contadores se insertarán delante. Se resta 1 por el contador asociado al estado, que no genera ninguna instrucción.
											instruccion->comando=ASIGNACION_;
											strcpy(instruccion->param1.nombre,$1);
											instruccion->param2.expresion=$3;

											ultInstrAsign++;
											
											insertarInstruccion(tComandos, *instruccion);


											instruccion= new tipo_instruccion;
											incrementarContadores();
										}
									}
									else{
										cout<<"Error semántico en la instruccion "<<n_lineas<<", '"<<$1<<"' no ha sido definido previamente."<<endl;
									}
								//mostrar(tabla,fsal);
								}
							}

							setTipo(0);
							setErrorModulo(0);
							setErrorDefinida(0);
							}
				;



exprLogica_arbol:  	LOGICA		 				{$$=crearBool(NULL, $1, NULL);}
       	      | expr_arbol S_MENOR expr_arbol	 			{$$=crearEntero($1, MENOR, $3);/*mostrarArbol($$);*/}
       	      | expr_arbol S_MENORIGUAL expr_arbol	 		{$$=crearEntero($1, MENOR_IGUAL, $3);/*mostrarArbol($$);*/}
       	      | expr_arbol S_MAYOR expr_arbol	 			{$$=crearEntero($1, MAYOR, $3);/*mostrarArbol($$);*/}
       	      | expr_arbol S_MAYORIGUAL expr_arbol	 		{$$=crearEntero($1, MAYOR_IGUAL, $3);/*mostrarArbol($$);*/}
       	      | expr_arbol S_IGUAL expr_arbol	 			{$$=crearEntero($1, IGUAL, $3);/*mostrarArbol($$);*/}
       	      | expr_arbol S_DISTINTO expr_arbol	 		{$$=crearEntero($1, DISTINTO, $3);/*mostrarArbol($$);*/}
       	      | exprLogica_arbol S_IGUAL id_logica_arbol		{$$=crearEntero($1, IGUAL, $3);/*mostrarArbol($$);*/}
       	      | exprLogica_arbol S_DISTINTO id_logica_arbol		{$$=crearEntero($1, DISTINTO, $3);/*mostrarArbol($$);*/}
       	      | exprLogica_arbol S_OR exprLogica_arbol	 		{$$=crearEntero($1, OR, $3);/*mostrarArbol($$);*/}
       	      | exprLogica_arbol S_AND exprLogica_arbol	 		{$$=crearEntero($1, AND, $3);/*mostrarArbol($$);*/}
       	      | '('exprLogica_arbol')' 	 				{arbolBinario temp=crearEntero($2, PAR_DER, NULL);$$=crearEntero(NULL, PAR_IZQ, temp);/*mostrarArbol($$);*/}
       	      | S_NOT exprLogica_arbol 	 				{$$=crearEntero(NULL, NOT, $2);/*mostrarArbol($$);*/}
       	      ;

id_logica_arbol:  ID			{$$=NULL;
					if(existe(tabla, $1)){
						consultar(tabla, $1, dato);
						if(dato.tipo==1 || dato.tipo==11){
							cout<<"Error semántico en la línea "<<n_lineas+1<<", no se pueden realizar operaciones lógicas con variables de tipo aritmético"<<endl;
							setErrorDefinida(1);

						}
						if(dato.tipo==0 || dato.tipo==10){
							cout<<"Error semántico en la línea "<<n_lineas+1<<", no se pueden realizar operaciones lógicas con variables de tipo aritmético"<<endl;
							setErrorDefinida(1);

						}
						if(dato.tipo==2 || dato.tipo==12){
							$$=crearCadena(NULL, dato.nombre, NULL);
						}
					}
					else{
						cout<<"Error semántico en la instruccion "<<n_lineas+1<<", la variable "<<$1<<" no ha sido definida"<<endl;
						setErrorDefinida(1);
					}}
	;

expr_arbol:  REAL	 	{$$=crearReal(NULL, $1, NULL); setTipo(1);}
       | NUMERO 		{$$=crearEntero(NULL, $1, NULL);}
       | ID			{$$=NULL;
				if(existe(tabla, $1)){
					consultar(tabla, $1, dato);
					if(dato.tipo==1 || dato.tipo==11){
						setTipo(1);
						$$=crearCadena(NULL, dato.nombre, NULL);
					}
					if(dato.tipo==0 || dato.tipo==10){
						$$=crearCadena(NULL, dato.nombre, NULL);
					}
					if(dato.tipo==2 || dato.tipo==12){
						cout<<"Error semántico en la línea "<<n_lineas+1<<", no se pueden realizar operaciones aritméticas con variables (sensores) de tipo lógico"<<endl;
						setErrorDefinida(1);
					}
				}
				else{
					cout<<"Error semántico en la instruccion "<<n_lineas+1<<", la variable (o sensor) "<<$1<<" no ha sido definida"<<endl;
					setErrorDefinida(1);
				}}
       | expr_arbol '+' expr_arbol 		{$$=crearEntero($1, SUMA, $3);}
       | expr_arbol '-' expr_arbol    		{$$=crearEntero($1, RESTA, $3);}
       | expr_arbol '*' expr_arbol          	{$$=crearEntero($1, MULTIPLICACION, $3);}
       | expr_arbol '/' expr_arbol          	{$$=crearEntero($1, DIVISION, $3);} //no nos importa el tipo de operacion para formar el arbol. Si para resolverlo
       | expr_arbol '%' expr_arbol		{if(banderaTipo==0){$$=crearEntero($1, MODULO, $3);}else{setErrorModulo(1);}}
       | expr_arbol '^' expr_arbol          	{$$=crearEntero($1, POTENCIA, $3);}
       | '('expr_arbol')'			{arbolBinario temp=crearEntero($2, PAR_DER, NULL);$$=crearEntero(NULL, PAR_IZQ, temp);}
       | '+' expr_arbol 			%prec masunario{$$=crearEntero	(NULL, MAS_UNARIO, $2);} //los operadores unarios solo tendrán hijo derecho (pero no serán hojas por eso)
       | '-' expr_arbol 			%prec menosunario{$$=crearEntero(NULL, MENOS_UNARIO, $2);}
       ;



/************************************************************************************************************************/
/************************************************************************************************************************/
/************************************************************************************************************************/
%%

int main( int argc, char *argv[]){

	if (argc != 2){
		cout <<"Error en los argumentos. Debes lanzar la aplicación siguiendo el formato './domol <fichero entrada>"<<endl;
		cout <<"	Ejemplo: ./domol ejemplo1.dml"<<endl;
	}
	else {

	     char nomEntrada [50];
	     strcpy(nomEntrada, argv[1]);
	     char nomSalida [50];
	     memset(nomSalida,'\0',50);
	     strncpy(nomSalida, argv[1], strlen(argv[1])-4);
	     strcat(nomSalida,".mch\0");


	     //asociamos un flujo de entrada del fichero a la entrada por defecto de yacc
     	     yyin=fopen(nomEntrada,"rt");

	     //creamos un flujo de salida hacia el fichero de salida pasado por parametro, para imprimir todo lo que no sean errores.
	     fsal.open(nomSalida);


	     if(!fsal.is_open()){cout<<"No se ha podido abrir el flujo de salida sobre el fichero '"<<nomSalida<<"'"<<endl;}
	     else{
		     tabla=NULL;
		     n_lineas = 0;
		     banderaTipo=0;
		     errorModulo=0;
		     errorDefinida=0;
		
		/*AMPLIACION 1*/
		     caso= new casoUso;
		     caso->numEstados=0;
		     caso->numTransiciones=0;
		     errorCasoUso=false;
		/*FIN AMPLIACION 1*/


		/*AMPLIACION 2*/
		     inicializar(tComandos);
		     instruccion=new tipo_instruccion;
		     ultInstrAsign=-1;
		     ultInstEstadoAnterior=-1;

		     inicializarContadoresZonas();
		     banderaElse=false;
		     numContElse=0;

		     inicializarSensoresEjemplo(vSensores);
		     inicializarVectorEjemplos(vEjemplos);
		     ejemplo= new EjemploStruct;

		/*FIN AMPLIACION 2*/


		     tTransicion= new tablaTransiciones;
		     inicializar(tTransicion);

		     ultEstAsign=-1;
		     ultTranAsign=-1;

		     yyparse();

		     mostrar(tabla,fsal);

		     tipo_cadena estados [ultEstAsign+1];//= new tipo_cadena[ultEstAsign];
		     int numEstados=0;
		     obtenerCodificacionEstados (tabla,estados, numEstados);
			
		     tipo_cadena transiciones[ultTranAsign+1];// []= new tipo_cadena;
		     int numTransiciones=0;
		     obtenerCodificacionTransiciones(tabla,transiciones, numTransiciones);

		     //mostrarTransiciones(tTransicion, ultEstAsign, fsal);
		     mostrarTransiciones(tTransicion, ultEstAsign+1, fsal, estados, transiciones);
		     mostrarCasosUso(fsal);
		     mostrarComandos(tComandos,fsal,ultEstAsign+1,estados);
		     mostrarEjemplos(vEjemplos, fsal);
	     }
	     cout<<endl<<endl;
	}
	return 0;
}
