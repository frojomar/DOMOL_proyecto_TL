#include "TADTablaTransiciones.h"



void  inicializar (tTrans & tabla){
  for(int i=0; i<TAMTABLA; i++){
    for(int j=0; j<TAMTABLA; j++  ){
      tabla->transiciones[i][j]=NO_DEFINIDO;
    }
  }
}


bool nuevaTransicion(tTrans tabla, int estadoEntrada, int estadoSalida, int transicion){
  if(tabla->transiciones[estadoEntrada][estadoSalida]!=NO_DEFINIDO){
    return false;
  }
  else{
    tabla->transiciones[estadoEntrada][estadoSalida]=transicion;
    return true;
  }
}

bool estadosTransicion(tTrans tabla, int transicion, int & estadoEntrada, int & estadoSalida){
  for(int i=0; i<TAMTABLA; i++){
    for(int j=0; j<TAMTABLA; j++){
      if(tabla->transiciones[i][j]==transicion){
        estadoEntrada=i;
	estadoSalida=j;
        return true;
      }
    }
  }
  return false;
}

int estadoSalida(tTrans tabla, int estadoEntrada, int transicion){
  int valor=-1;
  for(int j=0; j<TAMTABLA; j++){
    if(tabla->transiciones[estadoEntrada][j]==transicion){
      valor=j;
      return valor;
    }
  }
  return valor;
}

int consultarTransicion(tTrans tabla, int estadoEntrada, int estadoSalida){
  return tabla->transiciones[estadoEntrada][estadoSalida];
}



void mostrarTransiciones (tTrans tabla, ofstream & fsal){
  fsal<<"TABLA DE TRANSICIONES"<<endl;
  fsal<<"====================="<<endl;
  fsal<<" ***	";
  for(int i=0; i<TAMTABLA; i++){
    fsal<<i<<"	";
  }
  fsal<<endl<<endl;

  for(int i=0; i<TAMTABLA; i++){
    fsal<<"  "<<i<<"	";
    for(int j=0; j<TAMTABLA; j++){
      if(tabla->transiciones[i][j]!=NO_DEFINIDO){
        fsal<<tabla->transiciones[i][j]<<"	";
      }
      else{
        fsal<<"--	";
      }
    }
    fsal<<endl<<endl;
  }
}


void mostrarTransiciones (tTrans tabla, int numEstados, ofstream & fsal){
  fsal<<"TABLA DE TRANSICIONES"<<endl;
  fsal<<"====================="<<endl;
  fsal<<" ***	";
  for(int i=0; i<numEstados; i++){
    fsal<<i<<"	";
  }
  fsal<<endl;
  for(int i=0; i<numEstados; i++){
    fsal<<"=============";
  }
  fsal<<endl<<endl;

  for(int i=0; i<numEstados; i++){
    fsal<<"  "<<i<<" ||	";
    for(int j=0; j<numEstados; j++){
      if(tabla->transiciones[i][j]!=NO_DEFINIDO){
        fsal<<tabla->transiciones[i][j]<<"	";
      }
      else{
        fsal<<"--	";
      }
    }
    fsal<<endl<<endl;
  }
}

void mostrarTransiciones (tTrans tabla, int numEstados, ofstream & fsal, tipo_cadena estados [], tipo_cadena transiciones []){
  fsal<<"TABLA DE TRANSICIONES"<<endl;
  fsal<<"====================="<<endl;
  fsal<<" ****** "<<"       ";
  for(int i=0; i<numEstados; i++){
    fsal<<estados[i]<<"        ";
  }
  fsal<<endl;
  for(int i=0; i<numEstados; i++){
    fsal<<"=============";
  }
  fsal<<endl<<endl;

  for(int i=0; i<numEstados; i++){
    fsal<<estados[i]<<"   ||    ";
    for(int j=0; j<numEstados; j++){
      if(tabla->transiciones[i][j]!=NO_DEFINIDO){
        fsal<<transiciones[tabla->transiciones[i][j]]<<"      ";
      }
      else{
        fsal<<"------     ";
      }
    }
    fsal<<endl<<endl;
  }
}


void InaccesiblesBT(tTrans tabla, bool inaccesibles[],int numEstados, int estadoEntrada){
  for(int opcion=0; opcion<numEstados; opcion++){
    if(tabla->transiciones[estadoEntrada][opcion]!=NO_DEFINIDO){
      if(inaccesibles[opcion]==true){ //si ya sabemos que el estado era accesible, no lo volvemos a evaluar, por eficiencia.
        inaccesibles[opcion]=false;
        InaccesiblesBT(tabla, inaccesibles, numEstados, opcion);

      }
    }
	}
}

bool estadosInaccesibles (tTrans tabla, int numEstados){

    bool vector [numEstados];



  	for(int i=0; i<numEstados; i++){
  		vector[i]=true;
  	}

  	InaccesiblesBT(tabla, vector,numEstados, 0);
    vector[0]=false; //el estado inicial, siempre será accesible
    bool inaccesibles=false;
    for(int i=0; i<numEstados; i++){
      if(vector[i]==true){
        cout<<"Error semántico de transiciones. El estado "<<i<<" es un estado innacesible"<<endl;
        inaccesibles= true;
      }
    }
    return inaccesibles;
}

void MuertosBT(tTrans tabla, bool muertos[],int numEstados, int estadoSalida){
  for(int opcion=0; opcion<numEstados; opcion++){
    if(tabla->transiciones[opcion][estadoSalida]!=NO_DEFINIDO){
      if(muertos[opcion]==true){ //si ya sabemos que el estado no era un estado muerto, no lo volvemos a evaluar, por eficiencia.
        muertos[opcion]=false;
        MuertosBT(tabla, muertos, numEstados, opcion);

      }
    }
	}
}

bool estadosMuertos (tTrans tabla, int numEstados){

  bool vector [numEstados];



	for(int i=0; i<numEstados; i++){
		vector[i]=true;
	}

	MuertosBT(tabla, vector,numEstados, 1);

  vector[1]=false; //el estado final, nunca será muerto
  bool muertos=false;
  for(int i=0; i<numEstados; i++){
    if(vector[i]==true){
      cout<<"Error semántico de transiciones. El estado "<<i<<" es un estado muerto"<<endl;
      muertos= true;
    }
  }
  return muertos;
}
