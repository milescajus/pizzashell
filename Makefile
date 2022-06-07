default:
	cc shell.c builtins.c input.c -o pzash
all:
	cc -c shell.c -o shell.o
	cc -c builtins.c -o builtins.o
	cc -c input.c -o input.o
	cc -std=c99 shell.o builtins.o input.o -o pzash
clean:
	rm *.o
