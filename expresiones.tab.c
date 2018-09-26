/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 7 "expresiones.y" /* yacc.c:339  */


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
	nombreEstado nomCaso;
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

#line 254 "expresiones.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif


/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 1 "expresiones.y" /* yacc.c:355  */

	#include "TADArbol.h"

#line 285 "expresiones.tab.c" /* yacc.c:355  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    NUMERO = 258,
    REAL = 259,
    ID = 260,
    LOGICA = 261,
    TIPO_REAL = 262,
    TIPO_ENTERO = 263,
    TIPO_BOOL = 264,
    VARIABLES = 265,
    SENSORES = 266,
    ACTUADORES = 267,
    ESTADOS = 268,
    TRANSICIONES = 269,
    COMPORTAMIENTO = 270,
    SI = 271,
    SINO = 272,
    ACTIVAR = 273,
    DESACTIVAR = 274,
    TRANSICION = 275,
    ASIGNACION = 276,
    CASO = 277,
    CASOSUSO = 278,
    EJEMPLO = 279,
    EJEMPLOS = 280,
    INICIOLLAVES = 281,
    FINLLAVES = 282,
    S_MENORIGUAL = 283,
    S_MAYORIGUAL = 284,
    S_MENOR = 285,
    S_MAYOR = 286,
    S_IGUAL = 287,
    S_DISTINTO = 288,
    S_AND = 289,
    S_OR = 290,
    S_NOT = 291,
    menosunario = 292,
    masunario = 293
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 195 "expresiones.y" /* yacc.c:355  */

    int c_entero;
    float c_real;
    char c_cadena[20];
    bool c_logica;
    arbolBinario arbolExpresion;

#line 344 "expresiones.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);



/* Copy the second part of user declarations.  */

#line 361 "expresiones.tab.c" /* yacc.c:358  */
/* Unqualified %code blocks.  */
#line 4 "expresiones.y" /* yacc.c:359  */



#line 367 "expresiones.tab.c" /* yacc.c:359  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   341

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  52
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  36
/* YYNRULES -- Number of rules.  */
#define YYNRULES  120
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  249

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   293

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      46,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    41,     2,     2,
      45,    48,    39,    37,    49,    38,     2,    40,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    50,    51,
       2,    47,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    42,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    43,    44
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   273,   273,   274,   277,   281,   282,   285,   348,   401,
     406,   407,   408,   409,   410,   411,   412,   413,   414,   415,
     416,   417,   418,   421,   441,   442,   443,   467,   468,   469,
     470,   471,   472,   473,   474,   475,   480,   483,   484,   485,
     486,   487,   490,   500,   512,   522,   534,   544,   557,   560,
     561,   571,   574,   577,   578,   590,   593,   603,   604,   653,
     656,   657,   660,   661,   678,   681,   704,   738,   741,   742,
     742,   775,   778,   778,   802,   827,   852,   882,   883,   884,
     886,   887,   887,   894,   903,   904,   907,   908,   919,   923,
     924,   943,   957,   977,  1019,  1064,  1065,  1066,  1067,  1068,
    1069,  1070,  1071,  1072,  1073,  1074,  1075,  1076,  1079,  1102,
    1103,  1104,  1123,  1124,  1125,  1126,  1127,  1128,  1129,  1130,
    1131
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "NUMERO", "REAL", "ID", "LOGICA",
  "TIPO_REAL", "TIPO_ENTERO", "TIPO_BOOL", "VARIABLES", "SENSORES",
  "ACTUADORES", "ESTADOS", "TRANSICIONES", "COMPORTAMIENTO", "SI", "SINO",
  "ACTIVAR", "DESACTIVAR", "TRANSICION", "ASIGNACION", "CASO", "CASOSUSO",
  "EJEMPLO", "EJEMPLOS", "INICIOLLAVES", "FINLLAVES", "S_MENORIGUAL",
  "S_MAYORIGUAL", "S_MENOR", "S_MAYOR", "S_IGUAL", "S_DISTINTO", "S_AND",
  "S_OR", "S_NOT", "'+'", "'-'", "'*'", "'/'", "'%'", "'^'", "menosunario",
  "masunario", "'('", "'\\n'", "'='", "')'", "','", "':'", "';'",
  "$accept", "programa", "zona_variables", "zona_variables_instrucciones",
  "operacion_asignacion", "exprLogica", "id_logica", "expr",
  "zona_sensores", "zona_sensores_instrucciones",
  "identificadores_sensores_enteros", "identificadores_sensores_reales",
  "identificadores_sensores_bool", "zona_actuadores",
  "zona_actuadores_instrucciones", "zona_estados",
  "zona_estados_instrucciones", "zona_transiciones",
  "zona_transiciones_instrucciones", "zona_casos_uso",
  "zona_casos_uso_instrucciones", "caso_uso2", "zona_comportamiento",
  "zona_comportamiento_instrucciones", "$@1", "comportamiento2", "$@2",
  "comportamiento3", "$@3", "zona_ejemplos", "zona_ejemplos_instrucciones",
  "ejemplos2", "operacion_asignacion_arbol", "exprLogica_arbol",
  "id_logica_arbol", "expr_arbol", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,    43,    45,    42,
      47,    37,    94,   292,   293,    40,    10,    61,    41,    44,
      58,    59
};
# endif

#define YYPACT_NINF -235

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-235)))

#define YYTABLE_NINF -85

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      12,   -11,    43,    48,  -235,  -235,    27,    70,   296,  -235,
      37,    62,    45,    50,  -235,   287,  -235,    75,    78,  -235,
      20,    79,   125,   139,   146,     0,  -235,   112,   144,  -235,
    -235,  -235,   -29,    20,    29,    29,    20,    81,   166,  -235,
     111,   133,   122,   134,   132,   142,   154,   155,   121,  -235,
     156,   168,   205,   205,  -235,   212,    29,  -235,  -235,    46,
     124,    20,    20,  -235,    29,    29,    29,    29,    29,    29,
      29,    29,    29,    29,    29,    29,  -235,   125,  -235,   139,
    -235,   146,  -235,  -235,  -235,   163,   165,    84,  -235,   173,
     195,  -235,  -235,  -235,   233,  -235,  -235,  -235,   187,   271,
     271,   271,   271,   271,   271,   107,   107,   219,   219,   219,
     219,  -235,  -235,  -235,  -235,  -235,   182,   179,    63,  -235,
     201,  -235,  -235,   241,   216,  -235,    95,  -235,   242,  -235,
     250,   231,   273,    18,   274,   234,  -235,   239,   244,   286,
     246,  -235,   293,  -235,   254,  -235,   113,    47,  -235,  -235,
     256,   257,    24,   298,   300,   301,   275,  -235,  -235,   293,
     315,  -235,    24,  -235,  -235,  -235,  -235,    24,    34,    34,
      24,   204,   227,   276,   278,   279,  -235,  -235,   186,   -25,
     185,  -235,    34,  -235,  -235,   200,   145,  -235,   318,   318,
      24,    24,    34,    34,    34,    34,    34,    34,    34,    34,
      34,    34,    34,    34,  -235,  -235,  -235,   280,   281,   282,
    -235,  -235,   245,  -235,  -235,   283,  -235,  -235,  -235,  -235,
     297,   277,   277,   277,   277,   277,   277,   128,   128,   288,
     288,   288,   288,  -235,  -235,  -235,  -235,   118,   289,   316,
       5,  -235,  -235,  -235,   290,  -235,   123,   291,  -235
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     0,     0,     5,     1,     0,     0,     0,    37,
       0,     0,     0,     0,     6,     0,    49,     0,     0,     9,
       0,     0,     0,     0,     0,     0,    53,     0,     0,    25,
      24,    26,    10,     0,     0,     0,     0,     0,     0,    41,
      44,     0,    42,     0,    46,     0,     0,     0,     0,    57,
       0,    61,     0,     0,    22,     0,     0,    34,    35,     0,
       0,     0,     0,     8,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     7,     0,    39,     0,
      38,     0,    40,    51,    50,     0,     0,     0,    68,     0,
      85,    23,    17,    18,     0,    21,    33,    20,    19,    12,
      14,    11,    13,    15,    16,    27,    28,    29,    30,    31,
      32,    45,    43,    47,    55,    54,     0,     0,     0,    62,
       0,     3,    59,     0,     0,    69,     0,    86,     0,    71,
       0,     0,     0,     0,     0,     0,    64,     0,     0,     0,
       0,    78,     0,    88,     0,    58,     0,     0,    63,    89,
       0,     0,     0,     0,     0,     0,     0,    77,    65,     0,
      87,    79,     0,   110,   109,   111,    95,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    70,    66,     0,     0,
       0,   107,     0,   119,   120,     0,     0,    72,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    74,    75,    76,     0,     0,     0,
      94,    93,     0,   106,   118,     0,   108,   102,   103,   105,
     104,    97,    99,    96,    98,   100,   101,   112,   113,   114,
     115,   116,   117,    91,    92,    90,    78,     0,     0,    80,
       0,    73,    83,    81,     0,    78,     0,     0,    82
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -235,  -235,  -235,  -235,  -235,    51,   285,   -20,  -235,  -235,
     253,   262,   259,  -235,  -235,  -235,  -235,  -235,  -235,  -235,
    -235,   175,  -235,  -235,  -235,  -234,  -235,  -235,  -235,  -235,
    -235,  -235,  -235,  -150,   152,   -92
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,     8,    14,    37,    92,    55,     7,    15,
      43,    41,    45,    11,    25,    18,    48,    28,    87,    90,
     126,   148,    51,   118,   130,   146,   215,   241,   244,   121,
     133,   160,   157,   171,   217,   172
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      38,    46,   237,    52,    53,    47,   242,   188,   189,   190,
     191,   246,   179,   -48,    57,    58,    60,   181,   -84,   138,
     185,   210,     1,    29,    30,    31,    32,   163,   164,   165,
     166,   243,    29,    30,    31,     4,    94,   163,   164,   165,
     219,   220,   139,     5,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,    33,    34,    35,     6,
     167,   168,   169,   -67,   124,    36,    34,    35,   125,   170,
     180,   168,   169,     9,    56,    17,   183,   184,   186,   182,
      61,    62,    10,    16,    54,   116,   -67,    59,   -67,   117,
     212,    19,    27,   158,    95,   -60,   131,    20,   159,   -56,
     221,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,    97,    98,   150,    61,    62,   132,   151,   150,
     -60,    26,    85,   151,   150,    39,    86,    63,   151,   152,
      40,   153,   154,   155,   152,   -52,   153,   154,   155,   152,
     156,   153,   154,   155,    42,   238,    72,    73,    74,    75,
     247,    44,    64,    65,    66,    67,    68,    69,    49,    50,
      77,    70,    71,    72,    73,    74,    75,   200,   201,   202,
     203,    79,    96,   192,   193,   194,   195,   196,   197,    78,
      80,    81,   198,   199,   200,   201,   202,   203,    82,   207,
     208,    89,   209,   214,    64,    65,    66,    67,    68,    69,
      83,    84,    88,    70,    71,    72,    73,    74,    75,   114,
      91,   115,    76,   192,   193,   194,   195,   196,   197,   119,
     120,    61,   198,   199,   200,   201,   202,   203,   122,   123,
     187,   211,   188,   189,   190,   191,   188,   189,   190,   191,
      64,    65,    66,    67,    68,    69,   128,   127,   213,    70,
      71,    72,    73,    74,    75,   192,   193,   194,   195,   196,
     197,    75,   129,   134,   198,   199,   200,   201,   202,   203,
      70,    71,    72,    73,    74,    75,   135,   136,   137,   140,
     141,    96,   198,   199,   200,   201,   202,   203,    21,   142,
     143,   144,   145,   214,    22,    23,    24,    12,   147,   -36,
     149,    13,   161,   173,   162,   174,   175,    -4,    70,    71,
      72,    73,    74,    75,   198,   199,   200,   201,   202,   203,
     178,   176,   204,   216,   205,   206,   233,   234,   235,   236,
     203,   190,   112,   240,   177,   239,   245,   248,    93,   111,
     113,   218
};

static const yytype_uint8 yycheck[] =
{
      20,     1,   236,    32,    33,     5,     1,    32,    33,    34,
      35,   245,   162,    13,    34,    35,    36,   167,     0,     1,
     170,    46,    10,     3,     4,     5,     6,     3,     4,     5,
       6,    26,     3,     4,     5,    46,    56,     3,     4,     5,
     190,   191,    24,     0,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    36,    37,    38,    11,
      36,    37,    38,     0,     1,    45,    37,    38,     5,    45,
     162,    37,    38,    46,    45,    13,   168,   169,   170,    45,
      34,    35,    12,    46,    33,     1,    23,    36,    25,     5,
     182,    46,    14,    46,    48,     0,     1,    47,    51,    15,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,    61,    62,     1,    34,    35,    22,     5,     1,
      25,    46,     1,     5,     1,    46,     5,    46,     5,    16,
       5,    18,    19,    20,    16,    14,    18,    19,    20,    16,
      27,    18,    19,    20,     5,    27,    39,    40,    41,    42,
      27,     5,    28,    29,    30,    31,    32,    33,    46,    15,
      49,    37,    38,    39,    40,    41,    42,    39,    40,    41,
      42,    49,    48,    28,    29,    30,    31,    32,    33,    46,
      46,    49,    37,    38,    39,    40,    41,    42,    46,     3,
       4,    23,     6,    48,    28,    29,    30,    31,    32,    33,
      46,    46,    46,    37,    38,    39,    40,    41,    42,    46,
       5,    46,    46,    28,    29,    30,    31,    32,    33,    46,
      25,    34,    37,    38,    39,    40,    41,    42,    46,    50,
      26,    46,    32,    33,    34,    35,    32,    33,    34,    35,
      28,    29,    30,    31,    32,    33,     5,    46,    48,    37,
      38,    39,    40,    41,    42,    28,    29,    30,    31,    32,
      33,    42,    46,    21,    37,    38,    39,    40,    41,    42,
      37,    38,    39,    40,    41,    42,    26,    46,     5,     5,
      46,    48,    37,    38,    39,    40,    41,    42,     1,    50,
      46,     5,    46,    48,     7,     8,     9,     1,     5,    12,
      46,     5,    46,     5,    47,     5,     5,    11,    37,    38,
      39,    40,    41,    42,    37,    38,    39,    40,    41,    42,
       5,    46,    46,     5,    46,    46,    46,    46,    46,    46,
      42,    34,    79,    17,   159,    46,    46,    46,    53,    77,
      81,   189
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    10,    53,    54,    46,     0,    11,    60,    55,    46,
      12,    65,     1,     5,    56,    61,    46,    13,    67,    46,
      47,     1,     7,     8,     9,    66,    46,    14,    69,     3,
       4,     5,     6,    36,    37,    38,    45,    57,    59,    46,
       5,    63,     5,    62,     5,    64,     1,     5,    68,    46,
      15,    74,    32,    33,    57,    59,    45,    59,    59,    57,
      59,    34,    35,    46,    28,    29,    30,    31,    32,    33,
      37,    38,    39,    40,    41,    42,    46,    49,    46,    49,
      46,    49,    46,    46,    46,     1,     5,    70,    46,    23,
      71,     5,    58,    58,    59,    48,    48,    57,    57,    59,
      59,    59,    59,    59,    59,    59,    59,    59,    59,    59,
      59,    63,    62,    64,    46,    46,     1,     5,    75,    46,
      25,    81,    46,    50,     1,     5,    72,    46,     5,    46,
      76,     1,    22,    82,    21,    26,    46,     5,     1,    24,
       5,    46,    50,    46,     5,    46,    77,     5,    73,    46,
       1,     5,    16,    18,    19,    20,    27,    84,    46,    51,
      83,    46,    47,     3,     4,     5,     6,    36,    37,    38,
      45,    85,    87,     5,     5,     5,    46,    73,     5,    85,
      87,    85,    45,    87,    87,    85,    87,    26,    32,    33,
      34,    35,    28,    29,    30,    31,    32,    33,    37,    38,
      39,    40,    41,    42,    46,    46,    46,     3,     4,     6,
      46,    46,    87,    48,    48,    78,     5,    86,    86,    85,
      85,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    87,    87,    46,    46,    46,    46,    77,    27,    46,
      17,    79,     1,    26,    80,    46,    77,    27,    46
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    52,    53,    53,    54,    55,    55,    56,    56,    56,
      57,    57,    57,    57,    57,    57,    57,    57,    57,    57,
      57,    57,    57,    58,    59,    59,    59,    59,    59,    59,
      59,    59,    59,    59,    59,    59,    60,    61,    61,    61,
      61,    61,    62,    62,    63,    63,    64,    64,    65,    66,
      66,    66,    67,    68,    68,    68,    69,    70,    70,    70,
      71,    71,    72,    72,    72,    73,    73,    74,    75,    76,
      75,    75,    78,    77,    77,    77,    77,    77,    77,    77,
      79,    80,    79,    79,    81,    81,    82,    82,    82,    83,
      83,    83,    83,    84,    84,    85,    85,    85,    85,    85,
      85,    85,    85,    85,    85,    85,    85,    85,    86,    87,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      87
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     8,     3,     0,     2,     4,     4,     2,
       1,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     1,     1,     1,     1,     3,     3,     3,
       3,     3,     3,     3,     2,     2,     3,     0,     4,     4,
       4,     3,     1,     3,     1,     3,     1,     3,     3,     0,
       3,     3,     3,     0,     3,     3,     3,     0,     7,     3,
       3,     0,     0,     5,     3,     2,     3,     3,     0,     0,
       8,     3,     0,    10,     4,     4,     4,     2,     0,     3,
       0,     0,     7,     2,     3,     0,     0,     5,     3,     0,
       4,     4,     4,     4,     4,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     2,     1,     1,
       1,     1,     3,     3,     3,     3,     3,     3,     3,     2,
       2
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:
#line 274 "expresiones.y" /* yacc.c:1646  */
    {}
#line 1642 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 277 "expresiones.y" /* yacc.c:1646  */
    {}
#line 1648 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 281 "expresiones.y" /* yacc.c:1646  */
    {}
#line 1654 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 282 "expresiones.y" /* yacc.c:1646  */
    {}
#line 1660 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 285 "expresiones.y" /* yacc.c:1646  */
    {
							if(errorModulo==1){
								cout<<"Error semántico en la instrucción " <<n_lineas<<": el operador MODULO no se puede usar con datos de tipo real"<<endl;
							}
							else{
								if(errorDefinida==1){
									//nada que hacer. Mostramos mensaje antes.
								}
								else{
									if(existe(tabla,(yyvsp[-3].c_cadena))){
										consultar(tabla, (yyvsp[-3].c_cadena), dato);
										if(dato.tipo==0 && banderaTipo!=0){
											cout<<"Error semántico en la instruccion "<<n_lineas<<", la variable "<<(yyvsp[-3].c_cadena)<<" es de tipo entero y no se le puede asignar un valor real"<<endl;
											//asignacion de real a un entero. No controlamos logicos, pues logicos siempre entrarán por ID '=' exprLogica \n
										}
										else{
											if(banderaTipo==0){
												if(dato.tipo==0){//si la expresion es entera y la variable entera
											//fsal << "La instrucción "<<n_lineas<<" hace que la variable "<<$1<<", de tipo entero, tenga el valor "<<$3<<endl;
													dato.valor.valor_entero=(yyvsp[-1].c_real);
													dato.tipo=0;
												}
												if(dato.tipo==1){//si la expresion es entera y la variable real
											//fsal << "La instrucción "<<n_lineas<<" hace que la variable "<<$1<<", de tipo real, tenga el valor "<<$3<<endl;
													dato.valor.valor_real=(yyvsp[-1].c_real);
													dato.tipo=1;
												}
											}
											if(banderaTipo==1){
												//fsal << "La instrucción "<<n_lineas<<" hace que la variable "<<$1<<", de tipo real, tenga el valor "<<$3<<endl;
												dato.valor.valor_real=(yyvsp[-1].c_real);
												dato.tipo=1;
											}
											strcpy(dato.nombre, (yyvsp[-3].c_cadena));
											dato.inicializado=true;
											if(!modificar(tabla,dato)){
												cout<<"ERROR en la linea "<<n_lineas<<":No se ha podido modificar la variable "<<dato.nombre<<" - "<<(yyvsp[-3].c_cadena)<<endl;
											}
										}
									}
									else{
										if(banderaTipo==0){
											//fsal << "La instrucción "<<n_lineas<<" hace que la variable "<<$1<<", de tipo entero, tenga el valor "<<$3<<endl;
											dato.valor.valor_entero=(yyvsp[-1].c_real);
											dato.tipo=0;
										}
										if(banderaTipo==1){
											//fsal << "La instrucción "<<n_lineas<<" hace que la variable "<<$1<<", de tipo real, tenga el valor "<<$3<<endl;
											dato.valor.valor_real=(yyvsp[-1].c_real);
											dato.tipo=1;
										}
										strcpy(dato.nombre, (yyvsp[-3].c_cadena));
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
#line 1728 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 348 "expresiones.y" /* yacc.c:1646  */
    {
							if(errorModulo==1){
								cout<<"Error semántico en la instrucción " <<n_lineas<<": el operador MODULO no se puede usar con datos de tipo real"<<endl;
							}
							else{
								if(errorDefinida==1){
									//nada que hacer. Mostramos mensaje antes.
								}
								else{
									if(existe(tabla,(yyvsp[-3].c_cadena))){
										consultar(tabla, (yyvsp[-3].c_cadena), dato);
										if(dato.tipo!=2){
											cout<<"Error semántico en la instruccion " <<n_lineas<<", la variable "<<(yyvsp[-3].c_cadena)<<" no es de tipo logico"<<endl;
										}
										else{
											if((yyvsp[-1].c_logica)==0){
												//fsal << "La instrucción "<<n_lineas<<" hace que la variable "<<$1<<", de tipo logico, tenga el valor false"<<endl;
												dato.valor.valor_logico=false;
											}
											if((yyvsp[-1].c_logica)==1){
												//fsal << "La instrucción "<<n_lineas<<" hace que la variable "<<$1<<", de tipo logico, tenga el valor true"<<endl;
												dato.valor.valor_logico=true;
											}
											strcpy(dato.nombre, (yyvsp[-3].c_cadena));
											dato.tipo=2;
											dato.inicializado=true;
											if(!modificar(tabla,dato)){
												cout<<"ERROR en la linea "<<n_lineas<<":No se ha podido modificar la variable "<<dato.nombre<<" - "<<(yyvsp[-3].c_cadena)<<endl;
											}
										}
									}
									else{
										if((yyvsp[-1].c_logica)==0){
											//fsal << "La instrucción "<<n_lineas<<" hace que la variable "<<$1<<", de tipo logico, tenga el valor false"<<endl;
											dato.valor.valor_logico=false;
										}
										if((yyvsp[-1].c_logica)==1){
											//fsal << "La instrucción "<<n_lineas<<" hace que la variable "<<$1<<", de tipo logico, tenga el valor true"<<endl;
											dato.valor.valor_logico=true;
										}
										strcpy(dato.nombre, (yyvsp[-3].c_cadena));
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
#line 1786 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 401 "expresiones.y" /* yacc.c:1646  */
    {yyerrok;}
#line 1792 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 406 "expresiones.y" /* yacc.c:1646  */
    {(yyval.c_logica)=(yyvsp[0].c_logica);}
#line 1798 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 407 "expresiones.y" /* yacc.c:1646  */
    {(yyval.c_logica)=(yyvsp[-2].c_real)<(yyvsp[0].c_real);}
#line 1804 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 408 "expresiones.y" /* yacc.c:1646  */
    {(yyval.c_logica)=(yyvsp[-2].c_real)<=(yyvsp[0].c_real);}
#line 1810 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 409 "expresiones.y" /* yacc.c:1646  */
    {(yyval.c_logica)=(yyvsp[-2].c_real)>(yyvsp[0].c_real);}
#line 1816 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 410 "expresiones.y" /* yacc.c:1646  */
    {(yyval.c_logica)=(yyvsp[-2].c_real)>=(yyvsp[0].c_real);}
#line 1822 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 411 "expresiones.y" /* yacc.c:1646  */
    {(yyval.c_logica)=(yyvsp[-2].c_real)==(yyvsp[0].c_real);}
#line 1828 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 412 "expresiones.y" /* yacc.c:1646  */
    {(yyval.c_logica)=(yyvsp[-2].c_real)!=(yyvsp[0].c_real);}
#line 1834 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 413 "expresiones.y" /* yacc.c:1646  */
    {(yyval.c_logica)=(yyvsp[-2].c_logica)==(yyvsp[0].c_logica);}
#line 1840 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 414 "expresiones.y" /* yacc.c:1646  */
    {(yyval.c_logica)=(yyvsp[-2].c_logica)!=(yyvsp[0].c_logica);}
#line 1846 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 415 "expresiones.y" /* yacc.c:1646  */
    {(yyval.c_logica)=(yyvsp[-2].c_logica)||(yyvsp[0].c_logica);}
#line 1852 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 416 "expresiones.y" /* yacc.c:1646  */
    {(yyval.c_logica)=(yyvsp[-2].c_logica)&&(yyvsp[0].c_logica);}
#line 1858 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 417 "expresiones.y" /* yacc.c:1646  */
    {(yyval.c_logica)=(yyvsp[-1].c_logica);}
#line 1864 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 418 "expresiones.y" /* yacc.c:1646  */
    {(yyval.c_logica)=!(yyvsp[0].c_logica);}
#line 1870 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 421 "expresiones.y" /* yacc.c:1646  */
    {if(existe(tabla, (yyvsp[0].c_cadena))){
						consultar(tabla, (yyvsp[0].c_cadena), dato);
						if(dato.tipo==1 || dato.tipo==11){
							cout<<"Error semántico en la línea "<<n_lineas+1<<", no se pueden realizar operaciones lógicas con variables de tipo aritmético"<<endl;
							setErrorDefinida(1);
						}
						if(dato.tipo==0 || dato.tipo==10){
							cout<<"Error semántico en la línea "<<n_lineas+1<<", no se pueden realizar operaciones lógicas con variables de tipo aritmético"<<endl;
							setErrorDefinida(1);
						}
						if(dato.tipo==2 || dato.tipo==12){
							(yyval.c_logica)=dato.valor.valor_logico;
						}
					}
					else{
						cout<<"Error semántico en la instruccion "<<n_lineas+1<<", la variable "<<(yyvsp[0].c_cadena)<<" no ha sido definida"<<endl;
						setErrorDefinida(1);
					}}
#line 1893 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 441 "expresiones.y" /* yacc.c:1646  */
    {(yyval.c_real)=(yyvsp[0].c_real);setTipo(1);}
#line 1899 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 442 "expresiones.y" /* yacc.c:1646  */
    {(yyval.c_real)=(yyvsp[0].c_entero);}
#line 1905 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 443 "expresiones.y" /* yacc.c:1646  */
    {if(existe(tabla, (yyvsp[0].c_cadena))){
					consultar(tabla, (yyvsp[0].c_cadena), dato);
					if(dato.inicializado==true){
						if(dato.tipo==1 || dato.tipo==11){
							setTipo(1);
							(yyval.c_real)=dato.valor.valor_real;
						}
						if(dato.tipo==0 || dato.tipo==10){
							(yyval.c_real)=dato.valor.valor_entero;
						}
						if(dato.tipo==2 || dato.tipo==12){
							cout<<"Error semántico en la línea "<<n_lineas+1<<", no se pueden realizar operaciones aritméticas con variables (sensores) de tipo lógico"<<endl;
							setErrorDefinida(1);
						}
					}
					else{
						cout<<"Error semántico en la línea "<<n_lineas+1<<", el sensor "<<(yyvsp[0].c_cadena)<<" no ha sido inicializado"<<endl;
						setErrorDefinida(1);
					}
				}
				else{
					cout<<"Error semántico en la instruccion "<<n_lineas+1<<", la variable (o sensor) "<<(yyvsp[0].c_cadena)<<" no ha sido definida"<<endl;
					setErrorDefinida(1);
				}}
#line 1934 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 467 "expresiones.y" /* yacc.c:1646  */
    {(yyval.c_real)=(yyvsp[-2].c_real)+(yyvsp[0].c_real);}
#line 1940 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 468 "expresiones.y" /* yacc.c:1646  */
    {(yyval.c_real)=(yyvsp[-2].c_real)-(yyvsp[0].c_real);}
#line 1946 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 469 "expresiones.y" /* yacc.c:1646  */
    {(yyval.c_real)=(yyvsp[-2].c_real)*(yyvsp[0].c_real);}
#line 1952 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 470 "expresiones.y" /* yacc.c:1646  */
    {if(banderaTipo==0){(yyval.c_real)=(int)(yyvsp[-2].c_real)/(int)(yyvsp[0].c_real);}else{if(banderaTipo==1){(yyval.c_real)=(yyvsp[-2].c_real)/(yyvsp[0].c_real);}}}
#line 1958 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 471 "expresiones.y" /* yacc.c:1646  */
    {if(banderaTipo==0){(yyval.c_real)=(int)(yyvsp[-2].c_real)%(int)(yyvsp[0].c_real);}else{setErrorModulo(1);}}
#line 1964 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 472 "expresiones.y" /* yacc.c:1646  */
    {(yyval.c_real)=pow((yyvsp[-2].c_real),(yyvsp[0].c_real));}
#line 1970 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 473 "expresiones.y" /* yacc.c:1646  */
    {(yyval.c_real)=(yyvsp[-1].c_real);}
#line 1976 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 474 "expresiones.y" /* yacc.c:1646  */
    {(yyval.c_real)=(yyvsp[0].c_real);}
#line 1982 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 475 "expresiones.y" /* yacc.c:1646  */
    {(yyval.c_real)=-(yyvsp[0].c_real);}
#line 1988 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 480 "expresiones.y" /* yacc.c:1646  */
    {}
#line 1994 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 483 "expresiones.y" /* yacc.c:1646  */
    {}
#line 2000 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 484 "expresiones.y" /* yacc.c:1646  */
    {}
#line 2006 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 485 "expresiones.y" /* yacc.c:1646  */
    {}
#line 2012 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 486 "expresiones.y" /* yacc.c:1646  */
    {}
#line 2018 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 487 "expresiones.y" /* yacc.c:1646  */
    {yyerrok;}
#line 2024 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 490 "expresiones.y" /* yacc.c:1646  */
    {
											if(existe(tabla,(yyvsp[0].c_cadena))){cout<<"ERROR l."<<n_lineas<<": El sensor "<<(yyvsp[0].c_cadena)<<" ya existe. No se puede volver a crear"<<endl;}
											else{
												dato.valor.valor_entero=0;
												dato.tipo=10;
												strcpy(dato.nombre, (yyvsp[0].c_cadena));
												dato.inicializado=false;
												insertar(tabla,dato);
											}
											}
#line 2039 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 500 "expresiones.y" /* yacc.c:1646  */
    {
											if(existe(tabla,(yyvsp[-2].c_cadena))){cout<<"ERROR l."<<n_lineas<<": El sensor "<<(yyvsp[-2].c_cadena)<<" ya existe. No se puede volver a crear"<<endl;}
											else{
												dato.valor.valor_entero=0;
												dato.tipo=10;
												strcpy(dato.nombre, (yyvsp[-2].c_cadena));
												dato.inicializado=false;
												insertar(tabla,dato);
											}
											}
#line 2054 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 512 "expresiones.y" /* yacc.c:1646  */
    {
											if(existe(tabla,(yyvsp[0].c_cadena))){cout<<"ERROR l."<<n_lineas<<": El sensor "<<(yyvsp[0].c_cadena)<<" ya existe. No se puede volver a crear"<<endl;}
											else{
												dato.valor.valor_real=0;
												dato.tipo=11;
												strcpy(dato.nombre, (yyvsp[0].c_cadena));
												dato.inicializado=false;
												insertar(tabla,dato);
											}
											}
#line 2069 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 522 "expresiones.y" /* yacc.c:1646  */
    {
											if(existe(tabla,(yyvsp[-2].c_cadena))){cout<<"ERROR l."<<n_lineas<<": El sensor "<<(yyvsp[-2].c_cadena)<<" ya existe. No se puede volver a crear"<<endl;}
											else{
												dato.valor.valor_real=0;
												dato.tipo=11;
												strcpy(dato.nombre, (yyvsp[-2].c_cadena));
												dato.inicializado=false;
												insertar(tabla,dato);
											}
											}
#line 2084 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 534 "expresiones.y" /* yacc.c:1646  */
    {
											if(existe(tabla,(yyvsp[0].c_cadena))){cout<<"ERROR l."<<n_lineas<<": El sensor "<<(yyvsp[0].c_cadena)<<" ya existe. No se puede volver a crear"<<endl;}
											else{
												dato.valor.valor_logico=false;
												dato.tipo=12;
												strcpy(dato.nombre, (yyvsp[0].c_cadena));
												dato.inicializado=false;
												insertar(tabla,dato);
											}
											}
#line 2099 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 544 "expresiones.y" /* yacc.c:1646  */
    {

											if(existe(tabla,(yyvsp[-2].c_cadena))){cout<<"ERROR l."<<n_lineas<<": El sensor "<<(yyvsp[-2].c_cadena)<<" ya existe. No se puede volver a crear"<<endl;}
											else{
												dato.valor.valor_logico=false;
												dato.tipo=12;
												strcpy(dato.nombre, (yyvsp[-2].c_cadena));
												dato.inicializado=false;
												insertar(tabla,dato);
											}
											}
#line 2115 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 557 "expresiones.y" /* yacc.c:1646  */
    {}
#line 2121 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 560 "expresiones.y" /* yacc.c:1646  */
    {}
#line 2127 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 561 "expresiones.y" /* yacc.c:1646  */
    {
									if(existe(tabla,(yyvsp[-1].c_cadena))){cout<<"ERROR l."<<n_lineas<<": El actuador "<<(yyvsp[-1].c_cadena)<<" ya existe. No se puede volver a crear"<<endl;}
									else{
										dato.valor.valor_entero=0;
										dato.tipo=3;
										strcpy(dato.nombre, (yyvsp[-1].c_cadena));
										dato.inicializado=true;
										insertar(tabla,dato);
									}
									}
#line 2142 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 571 "expresiones.y" /* yacc.c:1646  */
    {yyerrok;}
#line 2148 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 574 "expresiones.y" /* yacc.c:1646  */
    {}
#line 2154 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 577 "expresiones.y" /* yacc.c:1646  */
    {}
#line 2160 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 578 "expresiones.y" /* yacc.c:1646  */
    {

								if(existe(tabla,(yyvsp[-1].c_cadena))){cout<<"ERROR l."<<n_lineas<<": El estado "<<(yyvsp[-1].c_cadena)<<" ya existe. No se puede volver a crear"<<endl;}
								else{
									dato.valor.valor_entero=ultEstAsign+1;
									dato.tipo=4;
									strcpy(dato.nombre, (yyvsp[-1].c_cadena));
									dato.inicializado=true;
									insertar(tabla,dato);
									ultEstAsign++;
								}
								}
#line 2177 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 590 "expresiones.y" /* yacc.c:1646  */
    {yyerrok;}
#line 2183 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 593 "expresiones.y" /* yacc.c:1646  */
    {
										if(estadosInaccesibles(tTransicion, ultEstAsign)){
											cout<<"Error semántico linea "<<n_lineas<<". Existen estados inaccesibles"<<endl;
										}
										if(estadosMuertos(tTransicion, ultEstAsign)){
											cout<<"Error semántico linea "<<n_lineas<<". Existen estados muertos"<<endl;
										}
										}
#line 2196 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 603 "expresiones.y" /* yacc.c:1646  */
    {}
#line 2202 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 604 "expresiones.y" /* yacc.c:1646  */
    {
											if(!existe(tabla,(yyvsp[-3].c_cadena))){
												cout<<"ERROR l."<<n_lineas<<": El estado '"<<(yyvsp[-3].c_cadena)<<"' no existe."<<endl;
											}
											else{
												if(!existe(tabla,(yyvsp[-1].c_cadena))){
													cout<<"ERROR l."<<n_lineas<<": El estado '"<<(yyvsp[-1].c_cadena)<<"' no existe."<<endl;
												}
												else{
													tipo_datoTS estado1, estado2, transicion;
													consultar(tabla, (yyvsp[-3].c_cadena), estado1);
													consultar(tabla, (yyvsp[-1].c_cadena), estado2);
													if(estado1.tipo!=4){
														cout<<"ERROR l."<<n_lineas<<": '"<<(yyvsp[-3].c_cadena)<<"' no es un estado."<<endl;
													}
													else{
														if(estado2.tipo!=4){
															cout<<"ERROR l."<<n_lineas<<": '"<<(yyvsp[-1].c_cadena)<<"' no es un estado."<<endl;
														}
														else{
															if(estado1.valor.valor_entero==1){
															cout<<"ERROR l."<<n_lineas<<": El estado final ('"<<(yyvsp[-3].c_cadena)<<"') no puede ser entrada."<<endl;
															}
															else{
																if(estado2.valor.valor_entero==0){
															cout<<"ERROR l."<<n_lineas<<": El estado inicio ('"<<(yyvsp[-1].c_cadena)<<"') no puede ser salida."<<endl;
																}
																else{
																	if(existe(tabla, (yyvsp[-5].c_cadena))){
																		cout<<"ERROR l."<<n_lineas<<": La transicion '"<<(yyvsp[-5].c_cadena)<<"' ya existe"<<endl;
																	}
																	else{
																		transicion.valor.valor_entero=ultTranAsign+1;
																		transicion.tipo=5;
																		strcpy(transicion.nombre, (yyvsp[-5].c_cadena));
																		transicion.inicializado=true;
																		insertar(tabla,transicion);
																		ultTranAsign++;

											if(!nuevaTransicion(tTransicion, estado1.valor.valor_entero, estado2.valor.valor_entero, transicion.valor.valor_entero)){
												cout<<"ERROR l."<<n_lineas<<": Ya existe una transicion de '"<<(yyvsp[-3].c_cadena)<<"' a '"<<(yyvsp[-1].c_cadena)<<"'"<<endl;
											}
																	}
																}
															}
														}
													}
												}
											}}
#line 2256 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 653 "expresiones.y" /* yacc.c:1646  */
    {yyerrok;}
#line 2262 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 656 "expresiones.y" /* yacc.c:1646  */
    {}
#line 2268 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 657 "expresiones.y" /* yacc.c:1646  */
    {}
#line 2274 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 660 "expresiones.y" /* yacc.c:1646  */
    {}
#line 2280 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 661 "expresiones.y" /* yacc.c:1646  */
    {
											if(existe(tabla, (yyvsp[-2].c_cadena))){
												cout<<"Error semántico en la linea "<<n_lineas<<". El id "<<(yyvsp[-2].c_cadena)<<" ya está siendo usado."<<endl;
												errorCasoUso=true;
											}
											else{
												if(!errorCasoUso){												
													strcpy(caso->nomCaso, (yyvsp[-2].c_cadena));
													vectorCasos[numCasos]=caso;
													numCasos++;
												}
											}
											caso=new casoUso;
											caso->numEstados=0;
											caso->numTransiciones=0;
											errorCasoUso=false;
											}
#line 2302 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 678 "expresiones.y" /* yacc.c:1646  */
    {yyerrok;}
#line 2308 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 681 "expresiones.y" /* yacc.c:1646  */
    {
						if(existe(tabla, (yyvsp[-1].c_cadena))){
							consultar(tabla, (yyvsp[-1].c_cadena), dato);
							if(dato.tipo!=4){
							cout<<"Error semántico en la linea "<<n_lineas<<". "<<(yyvsp[-1].c_cadena)<<" no es un estado."<<endl;
							errorCasoUso=true;
							}
							else{ //si existe un estado con el nombre indicado por ID...
								if(dato.valor.valor_entero=1){//y si es el estado final...
									strcpy(caso->estados[caso->numEstados],(yyvsp[-1].c_cadena));
									caso->numEstados++;
								}
								else{
									cout<<"Error semántico en la linea "<<n_lineas<<". El caso de uso debe acabar con un estado final."<<endl;
									errorCasoUso=true;	
								}						
							}
						}
						else{
							cout<<"Error semántico en la linea "<<n_lineas<<". No existe el estado "<<(yyvsp[-1].c_cadena)<<"."<<endl;
							errorCasoUso=true;
						}
						}
#line 2336 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 704 "expresiones.y" /* yacc.c:1646  */
    {
						if(!errorCasoUso){
							if(existe(tabla, (yyvsp[-2].c_cadena))){
								consultar(tabla, (yyvsp[-2].c_cadena), dato);
								if(dato.tipo!=4){
								cout<<"Error semántico en la linea "<<n_lineas<<". "<<(yyvsp[-2].c_cadena)<<" no es un estado."<<endl;
								errorCasoUso=true;
								}
								else{//si existe un estado con el nombre indicado por ID...
									strcpy(caso->estados[caso->numEstados],(yyvsp[-2].c_cadena));
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
								cout<<"Error semántico en la linea "<<n_lineas<<". No existe el estado "<<(yyvsp[-2].c_cadena)<<"."<<endl;
								errorCasoUso=true;
							}
						}
						}
#line 2373 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 738 "expresiones.y" /* yacc.c:1646  */
    {}
#line 2379 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 741 "expresiones.y" /* yacc.c:1646  */
    {}
#line 2385 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 742 "expresiones.y" /* yacc.c:1646  */
    {	nuevoContador();
									if(existe(tabla, (yyvsp[0].c_cadena))){
										consultar(tabla, (yyvsp[0].c_cadena), dato);
										if(dato.tipo!=4){
											cout<<"Error semántico en la linea "<<n_lineas<<". "<<(yyvsp[0].c_cadena)<<" no es un estado."<<endl;
										}
										else{										
											estadoComportamiento=dato.valor.valor_entero;	
										}
									}
									else{
										cout<<"Error semántico en la linea "<<n_lineas<<". El estado "<<(yyvsp[0].c_cadena)<<" no existe."<<endl;
									}
				}
#line 2404 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 755 "expresiones.y" /* yacc.c:1646  */
    {
											if(existe(tabla, (yyvsp[-6].c_cadena))){
												consultar(tabla, (yyvsp[-6].c_cadena), dato);
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
#line 2429 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 775 "expresiones.y" /* yacc.c:1646  */
    {yyerrok;}
#line 2435 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 778 "expresiones.y" /* yacc.c:1646  */
    {nuevoContador();}
#line 2441 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 779 "expresiones.y" /* yacc.c:1646  */
    {

														instruccion->numInstruccion=ultInstrAsign+1;
														if(!banderaElse){
															instruccion->comando=SI_SIMPLE;
															instruccion->param2.instrucciones2.numInstrucciones=0;			
														}
														instruccion->param1.expresion=(yyvsp[-7].arbolExpresion);
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
#line 2469 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 802 "expresiones.y" /* yacc.c:1646  */
    {
														if(existe(tabla, (yyvsp[-1].c_cadena))){
															consultar(tabla, (yyvsp[-1].c_cadena), dato);
															if(dato.tipo!=3){
															cout<<"Error semántico en la linea "<<n_lineas<<". "<<(yyvsp[-1].c_cadena)<<" no es un actuador"<<endl;
															}
															else{
																instruccion->numInstruccion=ultInstrAsign+1;
																instruccion->numInstruccion+=cuantosContadores()-1-numContElse; //incrementamos el numero de instrucciones tanto como contadores activos tengamos en ese momento, debido a que las instrucciones de estos contadores se insertarán delante. Se resta 1 por el contador asociado al estado, que no genera ninguna instrucción, así como los contadores de else (que solos, tampoco crearán instrucciones).
													
																instruccion->comando=ACTIVAR_;
																strcpy(instruccion->param1.nombre,(yyvsp[-1].c_cadena));

																ultInstrAsign++;
											
																insertarInstruccion(tComandos, *instruccion);

																instruccion= new tipo_instruccion;
																incrementarContadores();
															}
														}
														else{
															cout<<"Error semántico en la linea "<<n_lineas<<". El actuador "<<(yyvsp[-1].c_cadena)<<" no existe"<<endl;
														}
														}
#line 2499 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 827 "expresiones.y" /* yacc.c:1646  */
    {
														if(existe(tabla, (yyvsp[-1].c_cadena))){
															consultar(tabla, (yyvsp[-1].c_cadena), dato);
															if(dato.tipo!=3){
															cout<<"Error semántico en la linea "<<n_lineas<<". "<<(yyvsp[-1].c_cadena)<<" no es un actuador"<<endl;
															}
															else{
																instruccion->numInstruccion=ultInstrAsign+1;
																instruccion->numInstruccion+=cuantosContadores()-1-numContElse; //incrementamos el numero de instrucciones tanto como contadores activos tengamos en ese momento, debido a que las instrucciones de estos contadores se insertarán delante. Se resta 1 por el contador asociado al estado, que no genera ninguna instrucción, así como los contadores de else (que solos, tampoco crearán instrucciones).
													
																instruccion->comando=DESACTIVAR_;
																strcpy(instruccion->param1.nombre,(yyvsp[-1].c_cadena));

																ultInstrAsign++;
											
																insertarInstruccion(tComandos, *instruccion);

																instruccion= new tipo_instruccion;
																incrementarContadores();
															}
														}
														else{
															cout<<"Error semántico en la linea "<<n_lineas<<". El actuador "<<(yyvsp[-1].c_cadena)<<" no existe"<<endl;
														}
														}
#line 2529 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 852 "expresiones.y" /* yacc.c:1646  */
    {
														if(existe(tabla, (yyvsp[-1].c_cadena))){
															consultar(tabla, (yyvsp[-1].c_cadena), dato);
															if(dato.tipo!=5){
															cout<<"Error semántico en la linea "<<n_lineas<<". "<<(yyvsp[-1].c_cadena)<<" no es una transicion"<<endl;
															}
															else{
																int salida=estadoSalida(tTransicion, estadoComportamiento, dato.valor.valor_entero);
																if(salida==-1){
															cout<<"Error semántico en la linea "<<n_lineas<<". La transicion "<<(yyvsp[-1].c_cadena)<<" no esta definida en el ámbito"<<endl;
																}
																else{
																	instruccion->numInstruccion=ultInstrAsign+1;
																instruccion->numInstruccion+=cuantosContadores()-1-numContElse; //incrementamos el numero de instrucciones tanto como contadores activos tengamos en ese momento, debido a que las instrucciones de estos contadores se insertarán delante. Se resta 1 por el contador asociado al estado, que no genera ninguna instrucción, así como los contadores de else (que solos, tampoco crearán instrucciones).
																	instruccion->comando=TRANSICION_;
																	strcpy(instruccion->param1.nombre,(yyvsp[-1].c_cadena));

																	ultInstrAsign++;
											
																	insertarInstruccion(tComandos, *instruccion);

																	instruccion= new tipo_instruccion;
																	incrementarContadores();
																}
															}
														}
														else{
															cout<<"Error semántico en la linea "<<n_lineas<<". La transicion "<<(yyvsp[-1].c_cadena)<<" no existe"<<endl;
														}
														}
#line 2564 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 882 "expresiones.y" /* yacc.c:1646  */
    {}
#line 2570 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 883 "expresiones.y" /* yacc.c:1646  */
    {}
#line 2576 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 884 "expresiones.y" /* yacc.c:1646  */
    {yyerrok;}
#line 2582 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 886 "expresiones.y" /* yacc.c:1646  */
    {}
#line 2588 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 887 "expresiones.y" /* yacc.c:1646  */
    {nuevoContador();numContElse++;}
#line 2594 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 887 "expresiones.y" /* yacc.c:1646  */
    {
												instruccion->comando=SI_NO;
												instruccion->param2.instrucciones2.numInstrucciones=getUltimoContador();
												banderaElse=true;
												eliminarUltimoContador();
												numContElse--;
												}
#line 2606 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 894 "expresiones.y" /* yacc.c:1646  */
    {yyerrok;}
#line 2612 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 903 "expresiones.y" /* yacc.c:1646  */
    {}
#line 2618 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 904 "expresiones.y" /* yacc.c:1646  */
    {}
#line 2624 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 907 "expresiones.y" /* yacc.c:1646  */
    {}
#line 2630 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 908 "expresiones.y" /* yacc.c:1646  */
    {
												//tratar ejemplo e insertarlo
												strcpy(ejemplo->nomEjemplo, (yyvsp[-2].c_cadena));
												if(!tratarEjemplo(tComandos, tTransicion, tabla, vSensores, ejemplo)){
													cout<<"NOTA: EJEMPLO "<<(yyvsp[-2].c_cadena)<<" no ha podido llegar a estado final. Hay sensores sin inicializar"<<endl;
												}
												insertarEjemplo(vEjemplos, *ejemplo);
	
												ejemplo= new EjemploStruct;	
												resetSensores(vSensores);										
												}
#line 2646 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 919 "expresiones.y" /* yacc.c:1646  */
    {yyerrok;}
#line 2652 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 923 "expresiones.y" /* yacc.c:1646  */
    {}
#line 2658 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 924 "expresiones.y" /* yacc.c:1646  */
    {if(existe(tabla,(yyvsp[-2].c_cadena))){
							consultar(tabla, (yyvsp[-2].c_cadena), dato);
							if(dato.tipo==12){
								if((yyvsp[-1].c_logica)==0){
									dato.valor.valor_logico=false;
								}
								if((yyvsp[-1].c_logica)==1){
									dato.valor.valor_logico=true;
								}
								dato.inicializado=true;
								insertarSensor(vSensores, dato);
							}
							else{
								cout<<"ERROR l."<<n_lineas<<", el sensor "<<(yyvsp[-2].c_cadena)<<" no es de tipo logico."<<endl;
							}
						}
						else{
							cout<<"ERROR l."<<n_lineas<<", el sensor "<<(yyvsp[-2].c_cadena)<<" no existe."<<endl;
						}}
#line 2682 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 943 "expresiones.y" /* yacc.c:1646  */
    {if(existe(tabla,(yyvsp[-2].c_cadena))){
							consultar(tabla, (yyvsp[-2].c_cadena), dato);
							if(dato.tipo==10){
								dato.valor.valor_entero=(yyvsp[-1].c_entero);
								dato.inicializado=true;
								insertarSensor(vSensores, dato);
							}
							else{
								cout<<"ERROR l."<<n_lineas<<", el sensor "<<(yyvsp[-2].c_cadena)<<" no es de tipo entero."<<endl;
							}
						}
						else{
							cout<<"ERROR l."<<n_lineas<<", el sensor "<<(yyvsp[-2].c_cadena)<<" no existe."<<endl;
						}}
#line 2701 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 957 "expresiones.y" /* yacc.c:1646  */
    {if(existe(tabla,(yyvsp[-2].c_cadena))){
							consultar(tabla, (yyvsp[-2].c_cadena), dato);
							if(dato.tipo==11){
								dato.valor.valor_real=(yyvsp[-1].c_real);
								dato.inicializado=true;
								insertarSensor(vSensores, dato);
							}
							else{
								cout<<"ERROR l."<<n_lineas<<", el sensor "<<(yyvsp[-2].c_cadena)<<" no es de tipo real."<<endl;
							}
						}
						else{
							cout<<"ERROR l."<<n_lineas<<", el sensor "<<(yyvsp[-2].c_cadena)<<" no existe."<<endl;
						}}
#line 2720 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 977 "expresiones.y" /* yacc.c:1646  */
    {
							if(errorModulo==1){
								cout<<"Error semántico en la instrucción " <<n_lineas<<": el operador MODULO no se puede usar con datos de tipo real"<<endl;
							}
							else{
								if(errorDefinida==1){
									//nada que hacer. Mostramos mensaje antes.
								}
								else{
									if(existe(tabla,(yyvsp[-3].c_cadena))){
										consultar(tabla, (yyvsp[-3].c_cadena), dato);
										if((dato.tipo==0 || dato.tipo==10) && banderaTipo!=0){
											cout<<"Error semántico en la instruccion "<<n_lineas<<", la variable "<<(yyvsp[-3].c_cadena)<<" es de tipo entero y no se le puede asignar un valor real"<<endl;
											//asignacion de real a un entero. No controlamos logicos, pues logicos siempre entrarán por ID '=' exprLogica \n
										}
										else{
											instruccion->numInstruccion=ultInstrAsign+1;
											instruccion->numInstruccion+=cuantosContadores()-1-numContElse; //incrementamos el numero de instrucciones tanto como contadores activos tengamos en ese momento, debido a que las instrucciones de estos contadores se insertarán delante. Se resta 1 por el contador asociado al estado, que no genera ninguna instrucción, así como los contadores de else (que solos, tampoco crearán instrucciones).
													
											instruccion->comando=ASIGNACION_;
											strcpy(instruccion->param1.nombre,(yyvsp[-3].c_cadena));
											instruccion->param2.expresion=(yyvsp[-1].arbolExpresion);

											ultInstrAsign++;
											
											insertarInstruccion(tComandos, *instruccion);

											instruccion= new tipo_instruccion;
											incrementarContadores();
										}
									}
									else{
										cout<<"Error semántico en la instruccion "<<n_lineas<<", '"<<(yyvsp[-3].c_cadena)<<"' no ha sido definido previamente."<<endl;
									}
								//mostrar(tabla,fsal);
								}

							}
							setTipo(0);
							setErrorModulo(0);
							setErrorDefinida(0);
							}
#line 2767 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 1019 "expresiones.y" /* yacc.c:1646  */
    {
							if(errorModulo==1){
								cout<<"Error semántico en la instrucción " <<n_lineas<<": el operador MODULO no se puede usar con datos de tipo real"<<endl;
							}
							else{
								if(errorDefinida==1){
									//nada que hacer. Mostramos mensaje antes.
								}
								else{
									if(existe(tabla,(yyvsp[-3].c_cadena))){
										consultar(tabla, (yyvsp[-3].c_cadena), dato);
										if(dato.tipo!=2 && dato.tipo!=12){
											cout<<"Error semántico en la instruccion " <<n_lineas<<", la variable "<<(yyvsp[-3].c_cadena)<<" no es de tipo logico"<<endl;
										}
										else{
											instruccion->numInstruccion=ultInstrAsign+1;
											instruccion->numInstruccion+=cuantosContadores()-1; //incrementamos el numero de instrucciones tanto como contadores activos tengamos en ese momento, debido a que las instrucciones de estos contadores se insertarán delante. Se resta 1 por el contador asociado al estado, que no genera ninguna instrucción.
											instruccion->comando=ASIGNACION_;
											strcpy(instruccion->param1.nombre,(yyvsp[-3].c_cadena));
											instruccion->param2.expresion=(yyvsp[-1].arbolExpresion);

											ultInstrAsign++;
											
											insertarInstruccion(tComandos, *instruccion);


											instruccion= new tipo_instruccion;
											incrementarContadores();
										}
									}
									else{
										cout<<"Error semántico en la instruccion "<<n_lineas<<", '"<<(yyvsp[-3].c_cadena)<<"' no ha sido definido previamente."<<endl;
									}
								//mostrar(tabla,fsal);
								}
							}

							setTipo(0);
							setErrorModulo(0);
							setErrorDefinida(0);
							}
#line 2813 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 1064 "expresiones.y" /* yacc.c:1646  */
    {(yyval.arbolExpresion)=crearBool(NULL, (yyvsp[0].c_logica), NULL);}
#line 2819 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 1065 "expresiones.y" /* yacc.c:1646  */
    {(yyval.arbolExpresion)=crearEntero((yyvsp[-2].arbolExpresion), MENOR, (yyvsp[0].arbolExpresion));/*mostrarArbol($$);*/}
#line 2825 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 1066 "expresiones.y" /* yacc.c:1646  */
    {(yyval.arbolExpresion)=crearEntero((yyvsp[-2].arbolExpresion), MENOR_IGUAL, (yyvsp[0].arbolExpresion));/*mostrarArbol($$);*/}
#line 2831 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 1067 "expresiones.y" /* yacc.c:1646  */
    {(yyval.arbolExpresion)=crearEntero((yyvsp[-2].arbolExpresion), MAYOR, (yyvsp[0].arbolExpresion));/*mostrarArbol($$);*/}
#line 2837 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 1068 "expresiones.y" /* yacc.c:1646  */
    {(yyval.arbolExpresion)=crearEntero((yyvsp[-2].arbolExpresion), MAYOR_IGUAL, (yyvsp[0].arbolExpresion));/*mostrarArbol($$);*/}
#line 2843 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 1069 "expresiones.y" /* yacc.c:1646  */
    {(yyval.arbolExpresion)=crearEntero((yyvsp[-2].arbolExpresion), IGUAL, (yyvsp[0].arbolExpresion));/*mostrarArbol($$);*/}
#line 2849 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 1070 "expresiones.y" /* yacc.c:1646  */
    {(yyval.arbolExpresion)=crearEntero((yyvsp[-2].arbolExpresion), DISTINTO, (yyvsp[0].arbolExpresion));/*mostrarArbol($$);*/}
#line 2855 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 1071 "expresiones.y" /* yacc.c:1646  */
    {(yyval.arbolExpresion)=crearEntero((yyvsp[-2].arbolExpresion), IGUAL, (yyvsp[0].arbolExpresion));/*mostrarArbol($$);*/}
#line 2861 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 1072 "expresiones.y" /* yacc.c:1646  */
    {(yyval.arbolExpresion)=crearEntero((yyvsp[-2].arbolExpresion), DISTINTO, (yyvsp[0].arbolExpresion));/*mostrarArbol($$);*/}
#line 2867 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 1073 "expresiones.y" /* yacc.c:1646  */
    {(yyval.arbolExpresion)=crearEntero((yyvsp[-2].arbolExpresion), OR, (yyvsp[0].arbolExpresion));/*mostrarArbol($$);*/}
#line 2873 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 1074 "expresiones.y" /* yacc.c:1646  */
    {(yyval.arbolExpresion)=crearEntero((yyvsp[-2].arbolExpresion), AND, (yyvsp[0].arbolExpresion));/*mostrarArbol($$);*/}
#line 2879 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 1075 "expresiones.y" /* yacc.c:1646  */
    {arbolBinario temp=crearEntero((yyvsp[-1].arbolExpresion), PAR_DER, NULL);(yyval.arbolExpresion)=crearEntero(NULL, PAR_IZQ, temp);/*mostrarArbol($$);*/}
#line 2885 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 1076 "expresiones.y" /* yacc.c:1646  */
    {(yyval.arbolExpresion)=crearEntero(NULL, NOT, (yyvsp[0].arbolExpresion));/*mostrarArbol($$);*/}
#line 2891 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 1079 "expresiones.y" /* yacc.c:1646  */
    {(yyval.arbolExpresion)=NULL;
					if(existe(tabla, (yyvsp[0].c_cadena))){
						consultar(tabla, (yyvsp[0].c_cadena), dato);
						if(dato.tipo==1 || dato.tipo==11){
							cout<<"Error semántico en la línea "<<n_lineas+1<<", no se pueden realizar operaciones lógicas con variables de tipo aritmético"<<endl;
							setErrorDefinida(1);

						}
						if(dato.tipo==0 || dato.tipo==10){
							cout<<"Error semántico en la línea "<<n_lineas+1<<", no se pueden realizar operaciones lógicas con variables de tipo aritmético"<<endl;
							setErrorDefinida(1);

						}
						if(dato.tipo==2 || dato.tipo==12){
							(yyval.arbolExpresion)=crearCadena(NULL, dato.nombre, NULL);
						}
					}
					else{
						cout<<"Error semántico en la instruccion "<<n_lineas+1<<", la variable "<<(yyvsp[0].c_cadena)<<" no ha sido definida"<<endl;
						setErrorDefinida(1);
					}}
#line 2917 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 1102 "expresiones.y" /* yacc.c:1646  */
    {(yyval.arbolExpresion)=crearReal(NULL, (yyvsp[0].c_real), NULL); setTipo(1);}
#line 2923 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 1103 "expresiones.y" /* yacc.c:1646  */
    {(yyval.arbolExpresion)=crearEntero(NULL, (yyvsp[0].c_entero), NULL);}
#line 2929 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 1104 "expresiones.y" /* yacc.c:1646  */
    {(yyval.arbolExpresion)=NULL;
				if(existe(tabla, (yyvsp[0].c_cadena))){
					consultar(tabla, (yyvsp[0].c_cadena), dato);
					if(dato.tipo==1 || dato.tipo==11){
						setTipo(1);
						(yyval.arbolExpresion)=crearCadena(NULL, dato.nombre, NULL);
					}
					if(dato.tipo==0 || dato.tipo==10){
						(yyval.arbolExpresion)=crearCadena(NULL, dato.nombre, NULL);
					}
					if(dato.tipo==2 || dato.tipo==12){
						cout<<"Error semántico en la línea "<<n_lineas+1<<", no se pueden realizar operaciones aritméticas con variables (sensores) de tipo lógico"<<endl;
						setErrorDefinida(1);
					}
				}
				else{
					cout<<"Error semántico en la instruccion "<<n_lineas+1<<", la variable (o sensor) "<<(yyvsp[0].c_cadena)<<" no ha sido definida"<<endl;
					setErrorDefinida(1);
				}}
#line 2953 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 1123 "expresiones.y" /* yacc.c:1646  */
    {(yyval.arbolExpresion)=crearEntero((yyvsp[-2].arbolExpresion), SUMA, (yyvsp[0].arbolExpresion));}
#line 2959 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 1124 "expresiones.y" /* yacc.c:1646  */
    {(yyval.arbolExpresion)=crearEntero((yyvsp[-2].arbolExpresion), RESTA, (yyvsp[0].arbolExpresion));}
#line 2965 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 1125 "expresiones.y" /* yacc.c:1646  */
    {(yyval.arbolExpresion)=crearEntero((yyvsp[-2].arbolExpresion), MULTIPLICACION, (yyvsp[0].arbolExpresion));}
#line 2971 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 1126 "expresiones.y" /* yacc.c:1646  */
    {(yyval.arbolExpresion)=crearEntero((yyvsp[-2].arbolExpresion), DIVISION, (yyvsp[0].arbolExpresion));}
#line 2977 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 1127 "expresiones.y" /* yacc.c:1646  */
    {if(banderaTipo==0){(yyval.arbolExpresion)=crearEntero((yyvsp[-2].arbolExpresion), MODULO, (yyvsp[0].arbolExpresion));}else{setErrorModulo(1);}}
#line 2983 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 1128 "expresiones.y" /* yacc.c:1646  */
    {(yyval.arbolExpresion)=crearEntero((yyvsp[-2].arbolExpresion), POTENCIA, (yyvsp[0].arbolExpresion));}
#line 2989 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 1129 "expresiones.y" /* yacc.c:1646  */
    {arbolBinario temp=crearEntero((yyvsp[-1].arbolExpresion), PAR_DER, NULL);(yyval.arbolExpresion)=crearEntero(NULL, PAR_IZQ, temp);}
#line 2995 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 1130 "expresiones.y" /* yacc.c:1646  */
    {(yyval.arbolExpresion)=crearEntero	(NULL, MAS_UNARIO, (yyvsp[0].arbolExpresion));}
#line 3001 "expresiones.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 1131 "expresiones.y" /* yacc.c:1646  */
    {(yyval.arbolExpresion)=crearEntero(NULL, MENOS_UNARIO, (yyvsp[0].arbolExpresion));}
#line 3007 "expresiones.tab.c" /* yacc.c:1646  */
    break;


#line 3011 "expresiones.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 1139 "expresiones.y" /* yacc.c:1906  */


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
