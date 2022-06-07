default:
	cc -std=c99 -Wall shell.c builtins.c history.c -ledit -o pzash
all:
	cc -c shell.c -o shell.o
	cc -c builtins.c -o builtins.o
	cc -c history.c -o history.o
	cc -std=c99 -Wall shell.o builtins.o history.o -ledit -o pzash
clean:
	rm *.o
