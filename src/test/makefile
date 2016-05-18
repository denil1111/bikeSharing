colorflag = -fdiagnostics-color=auto
CC = g++ $(colorflag)
all: testBSP.o bspbase.o capacitated.o commenhead.o ktimescapacitatedBSP.o noZeroCapacitatedBSP.o supernode.o testBSP.o tspbase.o uncapacitatedBSP.o
	$(CC) -lemon -o testbike tspbase.o bspbase.o capacitated.o commenhead.o ktimescapacitatedBSP.o noZeroCapacitatedBSP.o supernode.o testBSP.o uncapacitatedBSP.o

bspbase.o : bspbase.cpp bspbase.h tspbase.h supernode.h commenhead.h
	$(CC) -c bspbase.cpp

capacitated.o : capacitated.cpp capacitated.h bspbase.h
	$(CC) -c capacitated.cpp

commenhead.o : commenhead.cpp commenhead.h
	$(CC) -c commenhead.cpp

ktimescapacitatedBSP.o : ktimescapacitatedBSP.cpp ktimescapacitatedBSP.h bspbase.h
	$(CC) -c ktimescapacitatedBSP.cpp

noZeroCapacitatedBSP.o : noZeroCapacitatedBSP.cpp noZeroCapacitatedBSP.h bspbase.h
	$(CC) -c noZeroCapacitatedBSP.cpp

supernode.o : supernode.cpp supernode.h
	$(CC) -c supernode.cpp

testBSP.o : testBSP.cpp uncapacitatedBSP.h capacitated.h ktimescapacitatedBSP.h noZeroCapacitatedBSP.h tspbase.h
	$(CC) -c testBSP.cpp

uncapacitatedBSP.o : uncapacitatedBSP.h uncapacitatedBSP.cpp bspbase.h
	$(CC) -c uncapacitatedBSP.cpp

tspbase.o : tspbase.cpp tspbase.h debug.h
	$(CC) -c tspbase.cpp

clean :
	rm *.o
