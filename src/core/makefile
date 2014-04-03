PROGRAM	= main
CC	= gcc
LIBS	= -ldl -pthread
INCLUDE	= -I../../include -I../../src
CFLAGS	= -O2 -pipe -flto -w
SOURCE	= *.c ../core_lib/*c

all: $(SOURCE)

	$(CC) $(SOURCE) -o $(PROGRAM) $(INCLUDE) $(CFLAGS) $(LIBS)
	strip --strip-unneeded main
