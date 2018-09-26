#include "TADArbol.h"



arbolBinario crearEntero(arbolBinario hijoIzq, int valor, arbolBinario hijoDer){

  arbolBinario ab = new nodoArbol;

  ab->hijoIzq=hijoIzq;
  ab->hijoDer=hijoDer;
  ab->dato.valor=valor;
  ab->esID=false;
  ab->esLogico=false;

  return ab;
}


arbolBinario crearReal(arbolBinario hijoIzq, float valor, arbolBinario hijoDer){

  arbolBinario ab = new nodoArbol;

  ab->hijoIzq=hijoIzq;
  ab->hijoDer=hijoDer;
  ab->dato.valor=valor;
  ab->esID=false;
  ab->esLogico=false;

  return ab;
}


arbolBinario crearCadena(arbolBinario hijoIzq, tipo_cadena id, arbolBinario hijoDer ){

  arbolBinario ab = new nodoArbol;

  ab->hijoIzq=hijoIzq;
  ab->hijoDer=hijoDer;
  strcpy(ab->dato.nombreID,id);
  ab->esID= true;
  ab->esLogico=false;

  return ab;
}

arbolBinario crearBool(arbolBinario hijoIzq, bool logico, arbolBinario hijoDer ){

  arbolBinario ab = new nodoArbol;

  ab->hijoIzq=hijoIzq;
  ab->hijoDer=hijoDer;
  ab->dato.valorLogico=logico;
  ab->esID= false;
  ab->esLogico=true;

  return ab;
}

bool esHoja(arbolBinario ab){
  if (ab->hijoIzq==NULL && ab->hijoDer==NULL){
    return true;
  }
  else{
    return false;
  }
}


void mostrarValor(arbolBinario arbol, ofstream & fsal){
  if(esHoja(arbol)){//en los nodos hoja mostraremos el valor o id asociado
    if(arbol->esID==false){ //si no es un ID
      if(arbol->esLogico==false){ //si no es tampoco un bool
        fsal<<arbol->dato.valor;
      }
      else{ //si es un bool
        if(arbol->dato.valorLogico==true){
          fsal<<"verdadero";
        }
        else{
          fsal<<"falso";
        }
      }
    }
    else{ //si es un ID
      fsal<<arbol->dato.nombreID;
    }
  }
  else{   //en los nodos internos mostraremos el simbolo de la operacion asociada al valor_real
      switch ((int)arbol->dato.valor) {
        case SUMA: fsal<<"+"; break;
        case RESTA: fsal<<"-"; break;
        case MULTIPLICACION: fsal<<"*"; break;
        case DIVISION: fsal<<"/"; break;
        case MODULO: fsal<<"%"; break;
        case POTENCIA: fsal<<"^"; break;
        case MAS_UNARIO: fsal<<"+"; break;
        case MENOS_UNARIO: fsal<<"-"; break;

        case PAR_IZQ: fsal<<"("; break;
        case PAR_DER: fsal<<")"; break;

        case MENOR: fsal<<"<"; break;
        case MENOR_IGUAL: fsal<<"<="; break;
        case MAYOR: fsal<<">"; break;
        case MAYOR_IGUAL: fsal<<">="; break;
        case IGUAL: fsal<<"=="; break;
        case DISTINTO: fsal<<"!="; break;
        case OR: fsal<<"||"; break;
        case AND: fsal<<"&&"; break;
        case NOT: fsal<<"!"; break;
      }
  }
}


void recorridoInOrden(arbolBinario arbol, ofstream & fsal){
  if(arbol!=NULL){
    recorridoInOrden(arbol->hijoIzq, fsal);
    mostrarValor(arbol, fsal);
    recorridoInOrden(arbol->hijoDer, fsal);
  }
}


void mostrarArbol(arbolBinario arbol, ofstream & fsal){
  recorridoInOrden(arbol, fsal);
}


void mostrarValor(arbolBinario arbol){
  if(esHoja(arbol)){//en los nodos hoja mostraremos el valor o id asociado
    if(arbol->esID==false){
      if(arbol->esLogico==false){
        cout<<arbol->dato.valor;
      }
      else{
        if(arbol->dato.valorLogico==true){
          cout<<"verdadero";
        }
        else{
          cout<<"falso";
        }
      }
    }
    else{
      cout<<arbol->dato.nombreID;
    }
  }
  else{   //en los nodos internos mostraremos el simbolo de la operacion asociada al valor_real
      switch ((int)arbol->dato.valor) {
        case SUMA: cout<<"+"; break;
        case RESTA: cout<<"-"; break;
        case MULTIPLICACION: cout<<"*"; break;
        case DIVISION: cout<<"/"; break;
        case MODULO: cout<<"%"; break;
        case POTENCIA: cout<<"^"; break;
        case MAS_UNARIO: cout<<"+"; break;
        case MENOS_UNARIO: cout<<"-"; break;

        case PAR_IZQ: cout<<"("; break;
        case PAR_DER: cout<<")"; break;

        case MENOR: cout<<"<"; break;
        case MENOR_IGUAL: cout<<"<="; break;
        case MAYOR: cout<<">"; break;
        case MAYOR_IGUAL: cout<<">="; break;
        case IGUAL: cout<<"=="; break;
        case DISTINTO: cout<<"!="; break;
        case OR: cout<<"||"; break;
        case AND: cout<<"&&"; break;
        case NOT: cout<<"!"; break;
      }
  }
}


void recorridoInOrden(arbolBinario arbol){
  if(arbol!=NULL){
    recorridoInOrden(arbol->hijoIzq);
    mostrarValor(arbol);
    recorridoInOrden(arbol->hijoDer);
  }
}


void mostrarArbol(arbolBinario arbol){
  recorridoInOrden(arbol);
  cout<<endl;
}

bool resolverArbolLogico(tipo_tabla variables, arbolBinario expresion, bool &correcto){
  bool valorIzq= false;
  bool valorDer= false;
  int valorIzqEntero=0;
  int valorDerEntero=0;
  bool esEntero=false;



  if(esHoja(expresion)){//en los nodos hoja mostraremos el valor o id asociado
    if(expresion->esID==false){
      if(expresion->esLogico==false){
        correcto=false;
        return false;
      }
      else{
        return expresion->dato.valorLogico;
      }
    }
    else{
      tipo_datoTS dato;
      consultar(variables, expresion->dato.nombreID, dato);
      if(dato.tipo==2 || dato.tipo==12){
        if(dato.inicializado==true){
          return dato.valor.valor_logico;
        }
        else{
          correcto=false;
          return false;
        }
      }
      else{
        correcto=false;
        return false;
      }
    }
  }
  else{  //si no es un nodo hoja, sino una operacion

    int valorNodo=(int)expresion->dato.valor;
    if(valorNodo==DISTINTO || valorNodo==IGUAL){
      if(expresion->hijoIzq!=NULL && expresion->hijoDer!=NULL){
        if(expresion->hijoIzq->esLogico){
          valorIzq=resolverArbolLogico(variables, expresion->hijoIzq, correcto);
          valorDer=resolverArbolLogico(variables, expresion->hijoDer, correcto);
        }
        else{
          valorIzqEntero=resolverArbolFloat(variables, expresion->hijoIzq, correcto);
          valorDerEntero=resolverArbolFloat(variables, expresion->hijoDer, correcto);
          esEntero=true;
        }
      }
    }
    else{
      if(valorNodo==MENOR ||valorNodo==MENOR_IGUAL ||valorNodo==MAYOR ||
          valorNodo==MAYOR_IGUAL){
            if(expresion->hijoIzq!=NULL){
              valorIzqEntero=resolverArbolFloat(variables, expresion->hijoIzq, correcto);
            }
            if(expresion->hijoDer!=NULL){
              valorDerEntero=resolverArbolFloat(variables, expresion->hijoDer, correcto);
            }
            esEntero=true;
      }
      else{
        if(expresion->hijoIzq!=NULL){
          valorIzq=resolverArbolLogico(variables, expresion->hijoIzq, correcto);
        }
        if(expresion->hijoDer!=NULL){
          valorDer=resolverArbolLogico(variables, expresion->hijoDer, correcto);
        }
      }
    }



      switch ((int)expresion->dato.valor) {
        case PAR_IZQ: return valorDer; break;
        case PAR_DER: return valorIzq; break;

        case MENOR: return valorIzqEntero<valorDerEntero; break;
        case MENOR_IGUAL: return valorIzqEntero<=valorDerEntero; break;
        case MAYOR: return valorIzqEntero>valorDerEntero; break;
        case MAYOR_IGUAL: return valorIzqEntero>=valorDerEntero; break;
        case IGUAL:
          if(esEntero)return valorIzqEntero==valorDerEntero;
          else return valorIzq==valorDer;
          break;
        case DISTINTO:
          if(esEntero)return valorIzqEntero!=valorDerEntero;
          else return valorIzq!=valorDer;
          break;
        case OR: return valorIzq||valorDer; break;
        case AND: return valorIzq&&valorDer; break;
      case NOT: return !valorDer; break;
      }
  }

}



float resolverArbolFloat(tipo_tabla variables, arbolBinario expresion, bool &correcto){
  int valorIzq= 0;
  int valorDer= 0;

  if(expresion->hijoIzq!=NULL){
    valorIzq=resolverArbolFloat(variables, expresion->hijoIzq, correcto);
  }
  if(expresion->hijoDer!=NULL){
    valorDer=resolverArbolFloat(variables, expresion->hijoDer, correcto);
  }

  if(esHoja(expresion)){//en los nodos hoja mostraremos el valor o id asociado
    if(expresion->esID==false){
      if(expresion->esLogico==false){
        return expresion->dato.valor;
      }
      else{
        correcto=false;
        return 0;
      }
    }
    else{
      tipo_datoTS dato;
      consultar(variables, expresion->dato.nombreID, dato);
      if(dato.tipo==0 || dato.tipo==10){
        if(dato.inicializado==true){
          return dato.valor.valor_entero;
        }
        else{
          correcto=false;
          return 0;
        }
      }
      else{
        if(dato.inicializado==true){
          return dato.valor.valor_real;
        }
        else{
          correcto=false;
          return 0;
        }
      }
    }
  }
  else{   //en los nodos internos mostraremos el simbolo de la operacion asociada al valor_real
      switch ((int)expresion->dato.valor) {
        case SUMA: return valorIzq+valorDer; break;
        case RESTA: return valorIzq-valorDer; break;
        case MULTIPLICACION: return valorIzq*valorDer; break;
        case DIVISION: return valorIzq/valorDer; break;
        case MODULO: return (int)valorIzq%(int)valorDer; break;
        case POTENCIA: return pow(valorIzq,valorDer); break;
        case MAS_UNARIO: return valorDer; break;
        case MENOS_UNARIO: return -valorDer; break;

        case PAR_IZQ: return valorDer; break;
        case PAR_DER: return valorIzq; break;
      }
  }

}
