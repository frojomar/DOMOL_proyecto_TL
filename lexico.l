%{
#include "expresiones.h"
#include <math.h>
int n_lineas;

%}

%option noyywrap
 
%x  COM1 COM2

%%

^"//"				BEGIN(COM2);
"//"                            BEGIN(COM1);

<COM1>\n                        {BEGIN(0);n_lineas++;return (yytext[0]);}
<COM2>\n			{BEGIN(0);n_lineas++;}
	
<COM1>.              	      	;
<COM2>.				;

" "					; //espacios
\t					; //tabulador	
^\n					n_lineas++; //lineas en blanco
^([ ]|[\t])+\n				n_lineas++; //lineas de espacios y tabuladores
\n					n_lineas++; return (yytext[0]);


(([0-9]+[.][0-9]*)|([0-9]*[.][0-9]+))[eE][0-9]+		yylval.c_real = atof(yytext);return REAL;//notacion cientifica
[0-9]+[.][0-9]+       	yylval.c_real = atof(yytext);return REAL; //notacion clasica
[.][0-9]+	yylval.c_real = atof(yytext);return REAL;  //sin parte entera
[0-9]+[.]	yylval.c_real = atof(yytext);return REAL; //sin parte decimal

verdadero		yylval.c_logica=true; return LOGICA; //valor logico 'true'
falso		yylval.c_logica=false; return LOGICA; //valor logico 'false'

[0-9]+					yylval.c_entero = atoi(yytext);return NUMERO;

si		return SI;
sino		return SINO;

int			strcpy(yylval.c_cadena, yytext);return TIPO_ENTERO;
float			strcpy(yylval.c_cadena, yytext);return TIPO_REAL;
bool			strcpy(yylval.c_cadena, yytext);return TIPO_BOOL;

activar			strcpy(yylval.c_cadena, yytext);return ACTIVAR;
desactivar		strcpy(yylval.c_cadena, yytext);return DESACTIVAR;
transicion		strcpy(yylval.c_cadena, yytext);return TRANSICION;
->	return ASIGNACION;


VARIABLES 	return VARIABLES;
SENSORES	return SENSORES;
ACTUADORES	return ACTUADORES;
ESTADOS		return ESTADOS;
TRANSICIONES	return TRANSICIONES;
COMPORTAMIENTO	return COMPORTAMIENTO;
EJEMPLOS	return EJEMPLOS;
EJEMPLO		return EJEMPLO;

CASOS[ ]DE[ ]USO	return CASOSUSO;

CASO 		return CASO;

"["		return INICIOLLAVES;
"]"		return FINLLAVES;

[a-zA-ZñÑáéíóúÁÉÍÓÚ_][a-zA-ZñÑáéíóúÁÉÍÓÚ_0-9]*		strcpy(yylval.c_cadena, yytext);return ID;


"<="	return S_MENORIGUAL; //SIMBOLOS DE LAS EXPRESIONES LOGICAS
"<"	return S_MENOR;
">="	return S_MAYORIGUAL;
">"	return S_MAYOR;
"=="	return S_IGUAL;
"!="	return S_DISTINTO;
"||"	return S_OR;
"&&"	return S_AND;
"!"	return S_NOT;



. 					return (yytext[0]);
%%
