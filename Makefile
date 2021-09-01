CC=g++
CFLAGS=-Wall -g -std=c++11

all: cliente servidor

servidor: servidor.c
	$(CC) $(CFLAGS) -o servidor -Ilibs bin/zstring.c bin/lista.c servidor.c

cliente: cliente.c
	$(CC) $(CFLAGS) -o cliente -Ilibs bin/* cliente.c

clear:
	rm -f cliente servidor

#./servidor
#./cliente localhost -f entrada0