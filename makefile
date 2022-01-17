all:nim.o

nim: nim.o nim.h alphanim.o nimhelp.o 
	clang nim.o nimhelp.o alphanim.o -Wall -std=c99 -pedantic
nim.o: nim.c nimhelp.c alphanim.c nim.h
	clang nim.c nimhelp.c alphanim.c -Wall -std=c99 -pedantic
alphanim.o: nim.h alphanim.c
	clang alphanim.c -Wall -std=c99 -pedantic
nimhelp.o: nim.h nimhelp.c
	clang nimhelp.c -Wall -std=c99 -pedantic
clean:
	rm *.o nim

