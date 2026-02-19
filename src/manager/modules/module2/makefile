all: src/*.c

	gcc src/*.c -o module.o -I../../include -w -pipe -flto -fPIC -c
	gcc *.o -o module.so -w -shared
	rm -f *.o
