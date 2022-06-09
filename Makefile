CC 		= cc
SRC		= shell.c builtins.c
LDFLAGS	= -ledit -lm
CFLAGS	= -std=c99 -Wall -I/usr/local/include
PROGRAM	= pzash

default:
	$(CC) $(CFLAGS) $(SRC) -o $(PROGRAM) $(LDFLAGS)
debug:
	$(CC) -g $(CFLAGS) $(SRC) -o $(PROGRAM) $(LDFLAGS)
clean:
	rm $(PROGRAM) *.o
