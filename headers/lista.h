#ifndef __LISTA_H
#define __LISTA_H

#include <stdio.h>
#include <stdlib.h>

struct nodo {
	void *dato;
	struct nodo *ptr_sig;
};

typedef struct nodo Nodo;
typedef unsigned long int iClave;

int estaVacia( Nodo* );
Nodo *crearMemoria( void *, Nodo*);
void insertarAlFrente( Nodo**, Nodo**, void* );
void insertarAlFinal ( Nodo**, Nodo**, void* );
void *removerAlFrente( Nodo**, Nodo**, int* );
void imprimirLista( Nodo*, void (*func)( void* ) );
int contarElementosLista( Nodo* );
int buscarDentroDeLista( Nodo*, int (*func)( void*, void* ), void*, void** );
void buscarDentroDeLista( Nodo *, int (*func)( void*, void* ), void*, Nodo**, Nodo** );
//void liberarInfo( void* );

#endif