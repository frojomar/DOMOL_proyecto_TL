#include "TADEjemplosAux.h"

void inicializarVectorEjemplos(vectorEjemplos & vEjemplos){
  vEjemplos= new vectorEjemplosEstatico;
  vEjemplos->numEjemplos=0;
}

void insertarTransiccion(EjemploPuntero ejemplo, tipo_cadena transiccion){
  strcpy(ejemplo->transicciones[ejemplo->numTransicciones],transiccion);
  ejemplo->numTransicciones++;
}

void insertarEjemplo(vectorEjemplos vEjemplos, EjemploStruct ejemplo){
  vEjemplos->ejemplos[vEjemplos->numEjemplos]=ejemplo;
  vEjemplos->numEjemplos++;
}

void mostrarEjemplos(vectorEjemplos vEjemplos, ofstream &fsal){
  fsal<<endl<<endl;
  fsal<<"SIMULACION DE LOS EJEMPLOS"<<endl;
  for(int i=0; i<vEjemplos->numEjemplos; i++){
      EjemploStruct ej= vEjemplos->ejemplos[i];
      fsal<<"     EJEMPLO "<<ej.nomEjemplo<<endl;
      for(int j=ej.numTransicciones-1; j>=0; j--){
        fsal<<"         "<<ej.transicciones[j]<<endl;
      }
  }

}



void inicializarSensoresEjemplo(vectorSensores & vSensores){
  vSensores= new vectorSensoresEstatico;
  vSensores->numSensores=0;
}

void insertarSensor(vectorSensores vSensores, tipo_datoTS sensor){
  bool encontrado=false;
  for(int i=0; i<vSensores->numSensores; i++){
    if(strcmp(vSensores->sensores[i].nombre, sensor.nombre)==0){
      vSensores->sensores[i]=sensor;
      encontrado=true;
    }
  }
  if(!encontrado){
    vSensores->sensores[vSensores->numSensores]=sensor;
    vSensores->numSensores++;
  }
}

void resetSensores (vectorSensores vSensores){
  vSensores->numSensores=0;
}

bool obtenerSensor(vectorSensores vSensores, tipo_cadena nomSensor, tipo_datoTS & sensor){
  bool encontrado=false;
  for(int i=0; i<vSensores->numSensores; i++){
    if(strcmp(vSensores->sensores[i].nombre, nomSensor)==0){
      sensor=vSensores->sensores[i];
      encontrado=true;
    }
  }
  return encontrado;
}
