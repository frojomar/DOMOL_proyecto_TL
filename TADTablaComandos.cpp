#include "TADTablaComandos.h"

/* DESC:   Inicializa todas las estructuras necesarias para la tabla de comandos.
*/
void  inicializar (tablaComandos & tComandos){
  tComandos= new tablaComandosEstatico;

  tComandos->instrucciones= NULL;

  for(int i=0; i<NUM_ESTADOS_MAX; i++){
    tComandos->insEst[i].primeraInstruccion=NO_DEFINIDO;
    tComandos->insEst[i].numInstrucciones=NO_DEFINIDO;
  }

}

/* DESC:   Nos devuelve 'true' si no hay ninguna instruccion en la tablaInstrucciones 'instrucciones'.
No es necesario comprobar 'insEst', pues, si la tabla de instrucciones está vacía, esta informacion carece
de importancia.
*/
bool  estaVacia(tablaComandos tComandos){
  return (tComandos->instrucciones==NULL);
}

/* DESC:   Inserta una nueva instrucciona en la tablaInstrucciones de 'tComandos'. Se usa para insertar
  la instruccion en la zona adecuada, en lugar de al final. Para instrucciones: SI_SIMPLE, SI_NO
*/
void insertarInstruccionZona(tablaComandos tComandos, tipo_instruccion instruccionZona){
  if (estaVacia(tComandos)){
    tComandos->instrucciones=new nodo_Instruccion;
    tComandos->instrucciones->next=NULL;
    tComandos->instrucciones->instruccion=instruccionZona;
    tComandos->instrucciones->before=NULL;
  }
  else{

    nodo_Instruccion *nAux= tComandos->instrucciones;

    while(nAux->next!=NULL){ //nos vamos a la ultima instruccion
      nAux=nAux->next;
    }

    int contador=0;
    if(instruccionZona.comando==SI_SIMPLE){
      contador=instruccionZona.param2.instrucciones1.numInstrucciones;
    }
    else{
      if(instruccionZona.comando==SI_NO){
        contador=instruccionZona.param2.instrucciones1.numInstrucciones
              +instruccionZona.param2.instrucciones2.numInstrucciones;
      }
    }
    if(contador==0){  //si la zona tenía 0 instrucciones, insertaremos como siempre
      //podríamos llamar a 'insertarInstruccion',  pero sería menos eficiente.
      nAux->next=new nodo_Instruccion;
      nAux->next->before=nAux;
      nAux=nAux->next;

      nAux->instruccion=instruccionZona;
      nAux->next=NULL;
    }
    else{
      while(contador>1){ //retrocedemos tantas instrucciones como tenga la zona -1 (para quedarnos delante)
        nAux=nAux->before;
        contador--;
      }
      nodo_Instruccion * nAux2= new nodo_Instruccion;
      nAux2->next=nAux;
      nAux2->before=nAux->before;
      nAux->before=nAux2;
      if(nAux2->before!=NULL){
        nAux2->before->next=nAux2;
      }
      else{//hemos insertado al inicio, así que debemos modificar el puntero a la lista
        tComandos->instrucciones=nAux2;
      }

      nAux2->instruccion=instruccionZona;
    }

  }
}


/* DESC:   Inserta una nueva instrucciona en la tablaInstrucciones de 'tComandos'.
*/
void  insertarInstruccion(tablaComandos tComandos, tipo_instruccion instruccion){
  if (estaVacia(tComandos)){
    tComandos->instrucciones=new nodo_Instruccion;
    tComandos->instrucciones->next=NULL;
    tComandos->instrucciones->instruccion=instruccion;
    tComandos->instrucciones->before=NULL;
  }
  else{

    nodo_Instruccion *nAux= tComandos->instrucciones;

    while(nAux->next!=NULL){
      nAux=nAux->next;
    }

    nAux->next=new nodo_Instruccion;
    nAux->next->before=nAux;
    nAux=nAux->next;

    nAux->instruccion=instruccion;
    nAux->next=NULL;

  }
}


/* DESC:   Modifica los parametros de 'instruccionesZona', para la celda 'estado' del vector 'insEst'
*/
void actualizarInstrEstado(tablaComandos tComandos, int estado, int primeraInstruccion, int numInstrucciones){
  tComandos->insEst[estado].primeraInstruccion=primeraInstruccion;
  tComandos->insEst[estado].numInstrucciones=numInstrucciones;
}



void seleccionarComando(int numComando, tipo_cadena & comando){
  switch(numComando){
    case ASIGNACION_:
        strcpy(comando, "ASIGNACION");
      break;
    case ACTIVAR_:
        strcpy(comando, "ACTIVAR");
      break;
    case DESACTIVAR_:
        strcpy(comando, "DESACTIVAR");
      break;
    case TRANSICION_:
        strcpy(comando, "TRANSICION");
      break;
    case SI_SIMPLE:
        strcpy(comando, "SI_SIMPLE");
      break;
    case SI_NO:
        strcpy(comando, "SI_NO");
      break;
  }
}

void mostrarParametros(int numComando, tipo_parametro param1, tipo_parametro param2, ofstream & fsal){

  tipo_cadena ident;
  strcpy(ident, "                                                    ");

  switch(numComando){
    case ASIGNACION_:
        fsal<<param1.nombre<<ident;mostrarArbol(param2.expresion,fsal);
      break;
    case ACTIVAR_:
        fsal<<param1.nombre<<ident<<"NULO";
      break;
    case DESACTIVAR_:
        fsal<<param1.nombre<<ident<<"NULO";
      break;
    case TRANSICION_:
        fsal<<param1.nombre<<ident<<"NULO";
      break;
    case SI_SIMPLE:
        mostrarArbol(param1.expresion,fsal);fsal<<ident<<param2.instrucciones1.numInstrucciones<<"-0";
      break;
    case SI_NO:
        mostrarArbol(param1.expresion,fsal);fsal<<ident<<param2.instrucciones1.numInstrucciones<<"-"<<param2.instrucciones2.numInstrucciones;
      break;
  }
}

void mostrarComandos (tablaComandos tComandos, ofstream & fsal, int numEstados, tipo_cadena estados []){

  /*MOSTRAMOS LA TABLA CON LA RELACION DE INSTRUCCIONES POR ESTADO*/
  fsal<<endl<<endl;

  fsal<<"TABLA DE CORRESPONDENCIA ESTADO-INSTRUCCIONES"<<endl;
  fsal<<"============================================="<<endl;
  fsal<<"ESTADO           PRIMERA INSTRUCCION       NUMERO INSTRUCCIONES	"<<endl;
  fsal<<"================================================================="<<endl;

  for(int i=0; i<numEstados; i++){
    instruccionesZona ins= tComandos->insEst[i];
    fsal<<estados[i]<<"           "<<ins.primeraInstruccion<<"                            "<<ins.numInstrucciones<<endl;
  }

  fsal<<endl<<endl;


  /*MOSTRAMOS LA TABLA DE INSTRUCCIONES*/
  fsal<<"TABLA DE COMANDOS"<<endl;
  fsal<<"============================================="<<endl;
  fsal<<"NUM. INSTRUCCION       COMANDO       PARAM1                                                          PARAM2"<<endl;
  fsal<<"=========================================================================================================="<<endl;

  tipo_cadena comando;
  if (!estaVacia(tComandos)){
		nodo_Instruccion *nAux= tComandos->instrucciones;
    while(nAux !=NULL){
      tipo_instruccion instruccion= nAux->instruccion;
      seleccionarComando(instruccion.comando, comando);

      fsal<<instruccion.numInstruccion<<"       ";
      fsal<<comando<<" ("<<instruccion.comando<<")"<<"        ";
      mostrarParametros(instruccion.comando, instruccion.param1, instruccion.param2, fsal);
      fsal<<endl;

      nAux=nAux->next;
    }

  }
}





/***********************************************************************/
/******************* METODOS PARA TRATAR LOS EJEMPLOS ******************/
/***********************************************************************/


tipo_instruccion * getInstruccion(tablaComandos tComandos,int numInstruccion){

  if (estaVacia(tComandos)){
    return NULL;
  }
  else{

    tipo_instruccion * ins= new tipo_instruccion;

    nodo_Instruccion *nAux= tComandos->instrucciones;

    while(nAux->next!=NULL && numInstruccion>0){
      nAux=nAux->next;
      numInstruccion--;
    }
    *ins=nAux->instruccion;
    return ins;
  }
}

int obtenerEstadoSalida(tTrans tTransiciones, tipo_tabla variables, int estadoEntrada, tipo_cadena transicion){

  tipo_datoTS tranDato;
  int numTransicion;
  consultar(variables, transicion, tranDato);
  numTransicion=tranDato.valor.valor_entero;
  return estadoSalida(tTransiciones, estadoEntrada, numTransicion);
}

void realizarAsignacion(tipo_tabla variables, tipo_cadena nombreVar, arbolBinario expresion, bool & correcto){
  tipo_datoTS dato;
  consultar(variables, nombreVar, dato);
  dato.inicializado=true;
  switch (dato.tipo) {
    case 0:
    dato.valor.valor_entero=(int)resolverArbolFloat(variables,expresion, correcto);
    break;
    case 1:
    dato.valor.valor_real=resolverArbolFloat(variables,expresion, correcto);
    break;
    case 2:
    dato.valor.valor_logico=resolverArbolLogico(variables,expresion, correcto);
    break;
    case 10:
    dato.valor.valor_entero=(int)resolverArbolFloat(variables,expresion, correcto);
    break;
    case 11:
    dato.valor.valor_real=resolverArbolFloat(variables,expresion, correcto);
    break;
    case 12:
    dato.valor.valor_logico=resolverArbolLogico(variables,expresion, correcto);
    break;
  }
  modificar(variables, dato);
}

bool evaluarExpresion(tipo_tabla variables, arbolBinario expresion, bool & correcto){
  return resolverArbolLogico(variables,expresion, correcto);
}

bool tratarEstado (tablaComandos tComandos, tTrans tTransiciones, int numEstado, tipo_tabla variables, EjemploPuntero ejemplo){
  bool estadoTratado=false;
  bool correcto=true;
  int instActual= tComandos->insEst[numEstado].primeraInstruccion;
  int numInstrucciones= tComandos->insEst[numEstado].numInstrucciones;
  bool saltoElse=false;
  int instElse=0;
  int cuantoElse=0;
  tipo_instruccion ins;
  tipo_datoTS dato;
  int numEstadoSalida=0;


  for(int i=0; i<numInstrucciones; i++){
    if(!estadoTratado && correcto){
      ins=*(getInstruccion(tComandos,instActual));
      if(saltoElse==true && instElse==instActual){
        instActual+=cuantoElse;
        i+=cuantoElse;
      }
      else{
        switch (ins.comando) {
          case ASIGNACION_:
            realizarAsignacion(variables, ins.param1.nombre, ins.param2.expresion, correcto);
            break;
          case ACTIVAR_:
            consultar(variables, ins.param1.nombre, dato);
            dato.inicializado=true;
            dato.valor.valor_entero=1;
            modificar(variables, dato);
            break;
          case DESACTIVAR_:
            consultar(variables, ins.param1.nombre, dato);
            dato.inicializado=true;
            dato.valor.valor_entero=0;
            modificar(variables, dato);
            break;
          case TRANSICION_:
            numEstadoSalida=obtenerEstadoSalida(tTransiciones, variables, numEstado, ins.param1.nombre);
            correcto=tratarEstado(tComandos, tTransiciones, numEstadoSalida, variables, ejemplo);
            insertarTransiccion(ejemplo, ins.param1.nombre);

            estadoTratado=true;
            break;
          case SI_SIMPLE:
            if(evaluarExpresion(variables, ins.param1.expresion, correcto)){
              //nada que hacer. Seguimos el flujo original
            }
            else{
              //saltar el numero de instrucciones adecuado
              instActual+=ins.param2.instrucciones1.numInstrucciones;
              i+=ins.param2.instrucciones1.numInstrucciones;
            }
            break;
          case SI_NO:
            if(evaluarExpresion(variables, ins.param1.expresion, correcto)){
              //indicar que hay que saltar 'cuantoElse' instrucciones cuando lleguemos a 'instElse'
              saltoElse=true;
              instElse=instActual+ins.param2.instrucciones1.numInstrucciones+1;
              cuantoElse=ins.param2.instrucciones2.numInstrucciones;
            }
            else{
              //saltar el numero de instrucciones adecuado para el IF
              instActual+=ins.param2.instrucciones1.numInstrucciones;
              i+=ins.param2.instrucciones1.numInstrucciones;
            }
            break;
        }

        instActual++;
      }
    }
  }

  return correcto;
}


bool tratarEjemplo (tablaComandos tComandos, tTrans tTransiciones, tipo_tabla variables, vectorSensores sensoresEjemplo, EjemploPuntero ejemplo){
  //creamos una copia de la tabla de variables, con la que trabajaremos sin
  //modificar la tabla original para otros ejemplos.
  tipo_tabla variablesCopia;
  copiarTabla(variablesCopia, variables);
  for (int i = 0; i < sensoresEjemplo->numSensores; i++) {
    modificar(variablesCopia, sensoresEjemplo->sensores[i]);
  }

  return tratarEstado(tComandos, tTransiciones, 0, variablesCopia, ejemplo);
}
