default:
	cc shell.c builtins.c history.c -o pzash
all:
	cc -c shell.c -o shell.o
	cc -c builtins.c -o builtins.o
	cc -c history.c -o history.o
	cc -std=c99 shell.o builtins.o history.o -o pzash
clean:
	rm *.o
