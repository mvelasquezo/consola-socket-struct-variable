CC=g++
CFLAGS=-Wall -g
#-std=c++11
cliente: cliente.c
	$(CC) $(CFLAGS) -o cliente -Iheaders zstring.c lista.c scanner.c cliente.c
clear:
	rm cliente