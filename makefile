PROGRAM	= main
CC	= gcc
LIBS	= -ldl -pthread
INCLUDE	= -Iinclude -Imisc
CFLAGS	= -O2 -pipe -flto -w -g
SOURCE	= src/core/*.c

all: $(SOURCE)

	$(CC) $(SOURCE) -o $(PROGRAM) $(INCLUDE) $(CFLAGS) $(LIBS)
	strip --strip-unneeded main
