#include "lista.h"

int estaVacia( Nodo *ini ){ return NULL == ini; }

Nodo *crearMemoria( void *dato, Nodo *ptr_sig ) {
    Nodo *tmp = (Nodo*) malloc( sizeof(Nodo) );
    if( tmp != NULL ) {
        tmp -> dato = dato;
        tmp -> ptr_sig = ptr_sig;
    }
    return tmp;
}

void insertarAlFrente( Nodo **ini, Nodo **fin, void *inf ) {
    if( estaVacia( *ini ) == 1 )
        *ini = *fin = crearMemoria( inf, NULL );
    else {
        Nodo *nuevo = crearMemoria( inf, *ini );
        *ini = nuevo;
    }
}

void insertarAlFinal( Nodo **ini, Nodo **fin, void *inf ) {
    if( estaVacia( *ini ) == 1 )
        *ini = *fin = crearMemoria( inf, NULL );
    else {
        Nodo *nuevo = crearMemoria( inf, NULL );
        (*fin) -> ptr_sig = nuevo;
        *fin = nuevo;
    }
}

void *removerAlFrente( Nodo **ini, Nodo **fin, int *errno ) {
    if( 1 == estaVacia(*ini) ){
        (*errno) = 1;
        return NULL;
    }
    
    void *dato = (*ini) -> dato;
    
    if(*ini==*fin)
        *ini = *fin = NULL;
    else
        *ini = (*ini) -> ptr_sig;
    
    (*errno) = 0;
    return dato;
}

void imprimirLista( Nodo *ini, void (*func)( void *inf ) ) {
    Nodo *actual = ini;

    if( NULL == ini ) {
        printf( "La lista esta vacia\n" );
        return;
    }
    
    while( actual != NULL ) {
        func( actual -> dato );
        actual = actual -> ptr_sig;
    }

    printf( "\n" );
}

int contarElementosLista( Nodo *ini ) {
    int contador = 0;
    Nodo *actual = ini;

    if( NULL == ini ) {
        contador = -1;
        return contador;
    }
    
    while( actual != NULL ) {
        contador++;
        actual = actual -> ptr_sig;
    }

    return contador;
}

int buscarDentroDeLista( Nodo *ini
        , int (*func)( void *inf, void *clave )
        , void *clave, void **i ) {

    Nodo *actual = ini;
    if( NULL == ini ) {
        //printf( "La lista esta vacia\n" );
        return -1;
    }
    
    while( actual != NULL ) {
        if( func( actual -> dato, clave ) == 1 ) {
            *i = actual -> dato;
            return 1;
        }
        actual = actual -> ptr_sig;
    }

    return 0;
}

void buscarDentroDeLista( Nodo *ini
        , int (*func)( void *inf, void *clave )
        , void *clave, Nodo **iniB, Nodo **finB ) {

    Nodo *actual = ini;
    if( NULL == ini ) {
        //printf( "La lista esta vacia\n" );
    }
    
    while( actual != NULL ) {
        if( func( actual -> dato, clave ) == 1 )
            insertarAlFinal( iniB, finB, (actual -> dato) );
        
        actual = actual -> ptr_sig;
    }
}

//void liberarInfo( void *dato ) {
//    Info *inf = (Info*) dato;
//    free( inf );
//}