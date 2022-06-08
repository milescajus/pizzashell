default:
	cc -std=c99 -Wall shell.c builtins.c history.c -ledit -o pzash -I/usr/local/include
all:
	cc -c shell.c -o shell.o
	cc -c builtins.c -o builtins.o
	cc -c history.c -o history.o
	cc -std=c99 -Wall shell.o builtins.o history.o -ledit -o pzash -I/usr/local/include
clean:
	rm *.o
