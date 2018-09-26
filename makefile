#fichero Makefile

OBJ = expresiones.o lexico.o TADTipoTabla.o TADTablaTransiciones.o TADTablaComandos.o TADArbol.o TADEjemplosAux.o

domol : $(OBJ)     
	g++ -odomol $(OBJ)

TADEjemplosAux.o : TADEjemplosAux.cpp
	g++ -c -oTADEjemplosAux.o TADEjemplosAux.cpp

TADTipoTabla.o : TADTipoTabla.cpp
	g++ -c -oTADTipoTabla.o TADTipoTabla.cpp

TADTablaTransiciones.o : TADTablaTransiciones.cpp
	g++ -c -oTADTablaTransiciones.o TADTablaTransiciones.cpp

TADTablaComandos.o : TADTablaComandos.cpp
	g++ -c -TADTablaComandos.o TADTablaComandos.cpp

TADArbol.o : TADArbol.cpp
	g++ -c -oTADArbol.o TADArbol.cpp

expresiones.o : expresiones.c        #primera fase de la traducción del analizador sintáctico
	g++ -c -oexpresiones.o  expresiones.c
	
lexico.o : lex.yy.c		#primera fase de la traducción del analizador léxico
	g++ -c -olexico.o  lex.yy.c 	

expresiones.c : expresiones.y       #obtenemos el analizador sintáctico en C
	bison -d -oexpresiones.c expresiones.y

lex.yy.c: lexico.l	#obtenemos el analizador léxico en C
	flex lexico.l

clean : 
	rm  -f  *.c *.o 

ayuda : 
	bison -v expresiones.y

ejecuta:
	make
	./calculadora entrada.txt salida.txt
