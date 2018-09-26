#include "TADTipoTabla.h"


void  inicializar (tipo_tabla &tabla){
	tabla=NULL;
}


bool  estaVacia (tipo_tabla tabla){
	return (tabla==NULL);
}

void  copiarTabla (tipo_tabla & tCopia, tipo_tabla tOriginal){
	tCopia= NULL;

	if(!estaVacia(tOriginal)){
		//copiamos el primer elemento para crear un nodo para tCopia
		tCopia= new nodo;
		tCopia->variable=tOriginal->variable;
		tCopia->next=NULL;

		nodo * nAux = tCopia;
		nodo * nAuxOriginal= tOriginal;

		while(nAuxOriginal->next!=NULL){
			nAux->next=new nodo;
			nAux=nAux->next;
			nAuxOriginal=nAuxOriginal->next;
			nAux->variable=nAuxOriginal->variable;
			nAux->next=NULL;
		}
	}

}

void  insertar (tipo_tabla &tabla,const tipo_datoTS dato){

	if (estaVacia(tabla)){
		tabla=new nodo;
		tabla->next=NULL;
		tabla->variable=dato;
	}
	else{

		nodo *nAux= tabla;

		while(nAux->next!=NULL){
			nAux=nAux->next;
		}

		nAux->next=new nodo;
		nAux=nAux->next;

		nAux->variable=dato;
		nAux->next=NULL;

	}
}


bool  existe (tipo_tabla tabla,tipo_cadena nombre){
	bool existe=false;

	if (!estaVacia(tabla)){
		nodo *nAux= tabla;

		while(nAux!=NULL){
			if(strcmp(nAux->variable.nombre, nombre)==0){
				existe=true;
			}
			nAux=nAux->next;
		}
	}

	return existe;
}

bool  modificar (tipo_tabla tabla,tipo_datoTS dato){
	bool modificado=false;

	if (!estaVacia(tabla)){
		nodo *nAux= tabla;

		while(nAux!=NULL){
			if((strcmp(nAux->variable.nombre, dato.nombre)==0) && (nAux->variable.tipo==dato.tipo)){
				nAux->variable=dato;
				modificado=true;
			}
			nAux=nAux->next;
		}
	}

	return modificado;
}


bool consultar (tipo_tabla tabla, tipo_cadena nombre, tipo_datoTS & dato){

	if (!estaVacia(tabla)){
		nodo *nAux= tabla;

		while(nAux!=NULL){
			if(strcmp(nAux->variable.nombre, nombre) ==0){
				dato=nAux->variable;
				return true;
			}
			nAux=nAux->next;
		}
	}

	return false;
}

bool consultarTransicion (tipo_tabla tabla, int id, tipo_datoTS & dato){
	if (!estaVacia(tabla)){
		nodo *nAux= tabla;

		while(nAux!=NULL){
			if(nAux->variable.valor.valor_entero==id && nAux->variable.tipo==5){
				dato=nAux->variable;
				return true;
			}
			nAux=nAux->next;
		}
	}

	return false;
}

void obtenerCodificacionEstados (tipo_tabla tabla, tipo_cadena estados[], int & numEstados){
	numEstados=0;
	if (!estaVacia(tabla)){
		nodo *nAux= tabla;

		while(nAux!=NULL){
			if(nAux->variable.tipo==4){
				strcpy(estados[nAux->variable.valor.valor_entero],nAux->variable.nombre);
				numEstados++;
			}
			nAux=nAux->next;
		}
	}
}

void obtenerCodificacionTransiciones (tipo_tabla tabla, tipo_cadena transiciones[], int & numTransiciones){
	numTransiciones=0;
	if (!estaVacia(tabla)){
		nodo *nAux= tabla;

		while(nAux!=NULL){
			if(nAux->variable.tipo==5){
				strcpy(transiciones[nAux->variable.valor.valor_entero],nAux->variable.nombre);
				numTransiciones++;
			}
			nAux=nAux->next;
		}
	}
}

void borrar (tipo_tabla &tabla, tipo_cadena nombre){

	if (!estaVacia(tabla)){
		nodo *nAux= tabla;
		nodo *nDelete;

		//caso de que el elemento sea el primero
		if(strcmp(nAux->variable.nombre, nombre) ==0){
			nDelete=nAux;
			tabla=nAux->next;
			nAux=tabla;
			delete nDelete;
		}

		//si el elemento es el primero, ya no volverá a estar (se supone que no hay repetidos).
		//Si no lo era, miramos el resto de la lista
		else{
			//caso de que esté en otras posiciones
			while(nAux->next !=NULL){
				if(strcmp(nAux->next->variable.nombre, nombre) ==0){
					nDelete=nAux->next;
					nAux->next=nAux->next->next;
					delete nDelete;
				}
				else{
					nAux=nAux->next;
				}
			}
			cout<<endl;
		}
	}

}


void mostrar (tipo_tabla tabla){

	if (!estaVacia(tabla)){
		nodo *nAux= tabla;
		nodo *nDelete;

		cout<<"****************************************************"<<endl;
		cout<<"****************************************************"<<endl;
		cout<<"**      NOMBRE       **     TIPO     **   VALOR   **"<<endl;
		cout<<"****************************************************"<<endl;

		while(nAux !=NULL){
			switch(nAux->variable.tipo){
				case 0:
						cout<<"**	 "<<nAux->variable.nombre<<"	 **   entero    **  "<<nAux->variable.valor.valor_entero<<"          **"<<endl;
						break;
				case 1:
						cout<<"**	 "<<nAux->variable.nombre<<"	 **    real     **  "<<nAux->variable.valor.valor_real<<"        **"<<endl;
						break;
				case 2:
						if (nAux->variable.valor.valor_logico==0) {
							cout<<"**	 "<<nAux->variable.nombre<<"	 **   logico    **  false       **"<<endl;
						}
						else{
							if(nAux->variable.valor.valor_logico==1){
								cout<<"**	 "<<nAux->variable.nombre<<"	 **   logico    **  true        **"<<endl;
							}
							else{
								cout<<"**	 "<<nAux->variable.nombre<<"	 **   logico    **  "<<nAux->variable.valor.valor_logico<<"           **"<<endl;
							}
						}
						break;
			}
			nAux=nAux->next;
		}
		cout<<"****************************************************"<<endl;
		cout<<"****************************************************"<<endl;
	}

}


void mostrar (tipo_tabla tabla, ofstream & fsal){

  fsal<<"TABLA DE SIMBOLOS"<<endl;
  fsal<<"================="<<endl;
	fsal<<"NOMBRE				TIPO			VALOR	"<<endl;
	fsal<<"================================================================="<<endl;
	if (!estaVacia(tabla)){
		nodo *nAux= tabla;
		nodo *nDelete;

		while(nAux !=NULL){
			switch(nAux->variable.tipo){
				case 0:
						fsal<<nAux->variable.nombre<<"			entero			"<<nAux->variable.valor.valor_entero<<endl;
						break;
				case 1:
						fsal<<nAux->variable.nombre<<"			real			"<<nAux->variable.valor.valor_real<<endl;
						break;
				case 2:
						if (nAux->variable.valor.valor_logico==0) {
							fsal<<nAux->variable.nombre<<"			logico			"<<"falso"<<endl;
						}
						else{
							if(nAux->variable.valor.valor_logico==1){
								fsal<<nAux->variable.nombre<<"			logico			"<<"verdadero"<<endl;
							}
							else{
								fsal<<nAux->variable.nombre<<"			entero			"<<nAux->variable.valor.valor_logico<<endl;
							}
						}
						break;
				case 10:
						if (nAux->variable.inicializado==true) {
							fsal<<nAux->variable.nombre<<"			sensor entero			"<<nAux->variable.valor.valor_entero<<endl;
						}
						else{
							fsal<<nAux->variable.nombre<<"			sensor entero			"<<"???"<<endl;
						}
						break;
				case 11:
						if (nAux->variable.inicializado==true) {
							fsal<<nAux->variable.nombre<<"			sensor real			"<<nAux->variable.valor.valor_real<<endl;
						}
						else{
							fsal<<nAux->variable.nombre<<"			sensor real			"<<"???"<<endl;
						}
						break;
				case 12:
						if (nAux->variable.inicializado==true) {
							if (nAux->variable.valor.valor_logico==0) {
								fsal<<nAux->variable.nombre<<"			sensor logico			"<<"falso"<<endl;
							}
							else{
								if(nAux->variable.valor.valor_logico==1){
									fsal<<nAux->variable.nombre<<"			sensor logico			"<<"verdadero"<<endl;
								}
								else{
									fsal<<nAux->variable.nombre<<"			sensor logico			"<<nAux->variable.valor.valor_logico<<endl;
								}
							}
							break;						}
						else{
							fsal<<nAux->variable.nombre<<"			sensor logico			"<<"???"<<endl;
						}
						break;
				case 3:
						fsal<<nAux->variable.nombre<<"			actuador			"<<nAux->variable.valor.valor_entero<<endl;
						break;
				case 4:
						fsal<<nAux->variable.nombre<<"			estado			"<<nAux->variable.valor.valor_entero<<endl;
						break;
				case 5:
						fsal<<nAux->variable.nombre<<"			transicion			"<<nAux->variable.valor.valor_entero<<endl;
						break;
				}
			nAux=nAux->next;
		}
		fsal<<endl<<endl;

	}

}



 void borrarMemoria(tipo_tabla &tabla){

	 nodo * pAux;

	 while(!estaVacia(tabla)){
		 if(tabla->next == NULL){
			 delete tabla;
			 tabla=NULL;
		 }
		 else{
			 pAux=tabla;
			 tabla=tabla->next;
			 delete pAux;
		 }

	 }

 }
