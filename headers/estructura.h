#ifndef __ESTRUCTURA_H
    #define __ESTRUCTURA_H

#define SER_PORT 1200

int GblflgImpLin = true;

struct infoCliente {
	int numlinea;
	unsigned int id;
    unsigned long int cui;
	char *nombre;
	char *apellidos;
	char *domicilio;
	unsigned long int movil;
	unsigned long int telefono;
	char *correoe;
};

struct infoBuscarCliente {
	int numlinea;
	unsigned int id;
    unsigned long int cui;
};

typedef struct infoMensaje {
	int numlinea;
    unsigned long int idOrigen;
	unsigned long int idDestino;
	char *mensaje;
	unsigned int estado;
}InfoMensaje;

typedef struct infoCliente InfoCliente;
typedef struct infoBuscarCliente InfoBuscarCliente;

enum { BUFFER_LEN = 20 };
enum { ENVIADO = 0, RECIBIDO = 1, TRASLADADO = 2 };

int div_and_ceil( int n, int d ) {
    return ( n%d == 0 ) ? (n/d) : (n/d + 1);
}

int calcPack( size_t len ) {
    return div_and_ceil( len, BUFFER_LEN - 1 );
}

int compararInfoCliente( void *dato, void *ptrClave ) {
    InfoCliente *inf = (InfoCliente*)dato;
	iClave clave = *((iClave*)ptrClave);

	//printf( "%lu == %lu\n", inf -> cui, clave );

    if( inf -> cui == clave || inf -> id == clave )
		return true;
    
	return false;
}

int compararInfoClienteConId( void *dato, void *ptrClave ) {
    InfoCliente *inf = (InfoCliente*)dato;
	iClave clave = *((iClave*)ptrClave);

	//printf( "%lu == %lu\n", inf -> cui, clave );

    if( inf -> id == clave )
		return true;
    
	return false;
}

int compararInfoMensaje( void *dato, void *ptrClave ) {
    InfoMensaje *inf = (InfoMensaje*)dato;
	iClave clave = *((iClave*)ptrClave);
    return ( inf -> idDestino == clave ) ? true : false;
}

void imprimirInfoCliente( void *dato ) {
    InfoCliente *inf = (InfoCliente*)dato;
    printf( "\n\t--------------------------" );

	if( true == GblflgImpLin )
		printf( "\n\tLínea:\t\t%d", inf -> numlinea );

	printf( "\n\tId:\t\t%d", 		inf -> id );
    printf( "\n\tNo. Dpi:\t%lu", 	inf -> cui );
    printf( "\n\tNombre:\t\t%s", 	inf -> nombre );
	printf( "\n\tApellidos:\t%s", 	inf -> apellidos );
	printf( "\n\tDomicilio:\t%s", 	inf -> domicilio );
	printf( "\n\tMóvil:\t\t%lu", 	inf -> movil );
	printf( "\n\tTeléfono:\t%lu", 	inf -> telefono );
	printf( "\n\tCorreo:\t\t%s", 	inf -> correoe );
}

char *serializarInfoCliente( InfoCliente *info ) {
    char rep[] = "#";
    char with[] = "\\#";
	char arrNum[ 10 ], arrId[ 10 ], arrCui[ 15 ];

	zitoa( info -> numlinea, arrNum, 10 );
    zitoa( info -> id, arrId, 10 );
	zitoa( info -> cui, arrCui, 10 );
    char *strMov = zULIntAStr( info -> movil );
	char *strTel = zULIntAStr( info -> telefono );
	char *strDom = zstr_replace( info -> domicilio, rep, with );

    char *str = zstr( arrNum, rep, arrId );
	str = zstr( str, rep, arrCui );
	str = zstr( str, rep, info -> nombre );
    str = zstr( str, rep, info -> apellidos );
    str = zstr( str, rep, strDom );
	str = zstr( str, rep, strMov );
    str = zstr( str, rep, strTel );
	str = zstr( str, rep, info -> correoe );

    return str;
}

void imprimirInfoMensaje( void *dato ) {
    InfoMensaje *inf = (InfoMensaje*)dato;
    printf( "\n\t--------------------------" );

	if( true == GblflgImpLin )
		printf( "\n\tLínea:\t\t%d", inf -> numlinea );

	printf( "\n\tId_origen:\t%lu", 	inf -> idOrigen );
    printf( "\n\tId_destino:\t%lu", inf -> idDestino );
    printf( "\n\tMensaje:\t%s", 	inf -> mensaje );
	printf( "\n\tEstado:\t\t%d", 	inf -> estado );
}

char *serializarInfoMensaje( InfoMensaje *info ) {
	char rep[] = "#";
    char with[] = "\\#";
	char arrNum[ 10 ], arrEst[ 3 ];

	zitoa( info -> numlinea, arrNum, 10 );
	zitoa( info -> estado, arrEst, 10 );
    char *strIdOrigen = zULIntAStr( info -> idOrigen );
	char *strIdDestino = zULIntAStr( info -> idDestino );
	char *strMensaje = zstr_replace( info -> mensaje, rep, with );

    char *str = zstr( arrNum, rep, strIdOrigen );
	str = zstr( str, rep, strIdDestino );
	str = zstr( str, rep, strMensaje );
    str = zstr( str, rep, arrEst );

    return str;
}
#endif