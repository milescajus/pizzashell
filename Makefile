CC 		= cc
SRC		= shell.c builtins.c
OBJ		= shell.o builtins.o
LDFLAGS	= -ledit -lm
CFLAGS	= -std=gnu11 -Wall -I/usr/local/include
PROGRAM	= pzash

default:
	$(CC) $(CFLAGS) $(SRC) -o $(PROGRAM) $(LDFLAGS)
debug:
	$(CC) -g $(CFLAGS) $(SRC) -o $(PROGRAM) $(LDFLAGS)
all:
	$(CC) -c $(SRC)
	$(CC) $(OBJ) -o $(PROGRAM) $(LDFLAGS)
clean:
	rm $(PROGRAM) *.o
