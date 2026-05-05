run: main
	./main
main: XORLiLs.o textEditor.c
	gcc textEditor.c XORLiLs.o -lcurses -o main
XORLiLs.o: ./libs/XORLiLs.c ./includes/XORLiLs.h
	gcc -c ./libs/XORLiLs.c -o XORLiLs.o
