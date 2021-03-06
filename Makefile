CC 		= cc
SRC		= shell.c builtins.c
OBJ		= shell.o builtins.o
UNAME_S	= $(shell uname -s)
LDFLAGS	= -lm
CFLAGS	= -std=gnu11 -Wall -I/usr/local/include
PROGRAM	= pzash

ifeq ($(UNAME_S), Linux)
	LDFLAGS	+= -lreadline
else
	LDFLAGS	+= -ledit
endif

default:
	$(CC) $(CFLAGS) $(SRC) -o $(PROGRAM) $(LDFLAGS)
debug:
	$(CC) -g $(CFLAGS) $(SRC) -o $(PROGRAM) $(LDFLAGS)
all:
	$(CC) -c $(SRC)
	$(CC) $(OBJ) -o $(PROGRAM) $(LDFLAGS)
clean:
	rm $(PROGRAM) *.o
