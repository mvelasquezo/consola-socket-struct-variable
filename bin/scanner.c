#include "zstring.h"
#include "scanner.h"
#include "cola.h"

struct Reservadas Rec_Res[ 7 ] = {
	{ "CREAR_CLIENTE", 50 }
	, { "ENVIAR_MENSAJE", 51 }
	, { "ESTRESAR", 52 }
	, { "OBTENER_MENSAJES", 53 }
	, { "LISTA", 54 }
	, { "LIMPIAR", 55 }
	, { "BUSCAR_CLIENTE", 56 }
};

//variables globales del programa
char lexema[ 150 ];	// Guarda el componente lexico
char codigo[ 15 ];
char letra[ 2 ];
char token;     	// Caracter leído desde el teclado
char *linea = zstr( (char*)"" );

int ilinea = -1;
int numlinea = 1;
int estado;     	// Estado actual del scaner
int salir;      	// Bandera de Lectura
int Fin = false;
FILE *file_in;  	// Archivo de entrada

Nodo *iniMsjErr = NULL;
Nodo *finMsjErr = NULL;

int abrir_archivo( const char *nombre ) {
  	   
	if( (file_in = fopen( nombre, "r") ) == NULL )
		return false;
   	else {
   		//fseek( file_in, 0, SEEK_SET );
		return true;
   	}
}

void cerrar_archivo() {
	//fflush( file_in );
	fclose( file_in );
	file_in = NULL;
}

void recuperarse(void) {
	int len		= ( linea != NULL ) ? strlen( linea ) : -1;
	do {
		Token(1);
		//printf( "token:'%d'\n", (int)token );
		//if( (int)token <= 0 ) break;
		if( ilinea >= len ) break;
	} while( !eol() && estado != E_puncoma );
	//} while( estado != E_puncoma && ( token != '\n'||token != EOF ) );

	if( !estaVacia( iniMsjErr ) ) {
		int errno = -1;
		do {
			char *msj = (char*)descolar( &iniMsjErr, &finMsjErr, &errno );
			if( errno == 0 ) mensaje_error( msj );
		} while( errno == 0 );
		printf( "\n" );
	}
}

void lee_car() {
	if( NULL == file_in )
		//token = getc( stdin );
		token = linea[ ++ilinea ];
	else
   		token = getc( file_in );
	
	//if (token == 255)
	//	strcpy(lexema,"caracter 255");

   	if( token == '\n' )
		numlinea++;
}

void ReturnFile2() {
	if( NULL == file_in ) {
		//ilinea--;
		--ilinea;
		//ungetc( token, stdin );
	} else
   		ungetc( token, file_in );  //retrocede un caracter en el archivo de entrada

   	if( token == '\n' )
		numlinea--;    // decrementar l¡nea leida
}

//estado letra del scanner
void Identificador() {
	if( isalpha(token) || isdigit(token) || token=='_'|| token=='-' )
	  estado = E_identificador;
   	else {
   		ReturnFile2();
		salir = true;
		lexema[ strlen( lexema ) - 1 ] = '\0';
	}
}

void ReturnfileLexema() {
	int i = ( strlen( lexema ) - 1 );
	while( i >= 0 ) {
		token = lexema[ i ];
		i--;
		ReturnFile2();
	}//while
}

void Inicial() {
    if((token==' ')||(token=='\n')||(token=='\t'))
	 	lexema[strlen(lexema)-1]='\0';
    else if(isalpha(token))
		estado=E_identificador;
    else if(isdigit(token))
		estado=E_num;
    /*else if(token=='-')
		estado=E_guion;*/
    else if(token=='+')
		estado=E_mas;
    else if(token=='(')
		estado=E_parA;
    else if(token==')')
		estado=E_parC;
    /*else if(token==' ')
		estado=E_space;*/
    else if(token=='\"')
		estado=E_com;
    else if(token==',')
		estado=E_coma;
    else if(token==';')
		estado=E_puncoma;
    else if (token == '[')
	     estado = E_corcheteA;
    else if (token == ']')
	     estado = E_corcheteC;
    else if (token == '{')
	     estado = E_LlaveA;
    else if (token == '}')
	     estado = E_LlaveC;
    else if (token == '?')
	     estado = E_preg;
    else if (token == '.')
	     estado = E_punto;
    else if (token == '|')
	     estado = E_or;
    else if (token == 39)
	     estado = E_apos;
    else if (token == '=')
	     estado = E_igual;
    else if (token==EOF)
	     estado=E_FinArchivo;
    else {
		estado=E_ascii;
	 	salir=true;
    }//else
}

void numero() {
	if (isdigit(token))  // Busca el siguiente digito
       estado= E_num;
   else {
		if((token==' ')||(token=='\n')||(token=='\t')) //fin de linea
			lexema[ strlen( lexema ) - 1 ] = '\0';
		else {
	    	ReturnFile2();    // siguiente lexema
	    	salir = true;
	    	lexema[ strlen( lexema ) - 1 ] = '\0';
		}
	}
}

int identifica_reservada() {
   	char lexema2[ 150 ];
   	strcpy( lexema2, lexema );
   	unsigned int i = 0;
   	for( i = 0; i < strlen( lexema ); i++ )
		lexema2[ i ] = toupper( lexema2[ i ] );  // Convierte a mayusculas el ID //

   	i = 0;

   	while( i < 30 ) {
   		//sustituyendo: stricmp -> strcasecmp
    	if( !strcasecmp( Rec_Res[i].reservada,lexema2 ) ) {
	  		strcpy( lexema, lexema2 );
	  		estado = Rec_Res[i].codigo;
	  		i = 30;
      	} else
	 		i++;
    }

   	return estado;
}

lexico Token(int tipo) {
    lexico RegLex;  // devuelve el componente lexico y el atributo //
    estado = E_inicial;
    strcpy( lexema, "" );
    strcpy( letra, "" );
    Fin = false;
    salir = false;
    while (!salir && token != EOF) {
		lee_car();
		//printf( "%c\n", token );
		letra[ 0 ] = token;
		letra[ 1 ] = '\0'; //sustituyendo: NULL -> \0
		strcat( lexema, letra );
		switch( estado ){
		       case E_inicial :        Inicial();	break;
		       case E_identificador :  Identificador();	break;
		       case E_num:             numero();	break;

		       default:
		       			salir = true;
						lexema[ strlen( lexema ) - 1 ] = '\0';
						ReturnFile2();
		       break;
		}//switch
  	}//while

	if( token == EOF )
		Fin = true;

	if( estado == E_identificador && tipo == 2 )
		estado = E_texto;

	if( estado == E_identificador && tipo == 1 )
	   estado = identifica_reservada();

	strcpy( RegLex.C_lex, lexema );
	RegLex.atrib = estado;

	return RegLex;
}

int eol() {
	int r = 0; int entro = 0;
 	while( token == ' ' ) {
 		if( token == ' ' ) {
	 		lee_car(); 
	 		entro = 1;
	 	}
	}

 	if( token == '\n'||token== EOF ) {
		r = 1;
	}

 	if( entro )
		ReturnFile2();

	//printf( "r: %d\n", r );

 	return r;
}

int parIntLargo( int conComa, int long unsigned *num ) {
	int r = false;
	struct lexico lex = Token(1);

	if( true == conComa ) {
		if( lex.atrib == E_coma ) {
			lex = Token(1);
		} else {
			encolar( &iniMsjErr, &finMsjErr, (void*)"Se esperaba una coma" );
			//mensaje_error( "Se esperaba una coma" );
			r = false;
			return r;
		} 
	}

	if( lex.atrib == E_num ) {
		*num = atoll( lex.C_lex );
		r = true;
	} 
	else 
		encolar( &iniMsjErr, &finMsjErr, (void*)"Se esperaba un número largo" );
		//mensaje_error( "Se esperaba un número largo" );
	
	return r;
}

int parStr( int conComa, char **str, const char *sep ) {
	char *nom = NULL;
	int r = false;
	struct lexico lex = Token(2);
	
	if( true == conComa ) {
		if( lex.atrib == E_coma ) {
			lex = Token(2);
		} else {
			//mensaje_error( "Se esperaba una coma" );
			encolar( &iniMsjErr, &finMsjErr, (void*)"Se esperaba una coma" );
			r = false;
			return r;
		} 
	}

	if( lex.atrib == E_com ) {
		lex = Token(2);

		do {
			nom = ( NULL == nom ) ? zstr( lex.C_lex ) : zstr( nom, sep, lex.C_lex );
			lex = Token(2);
		} while( !eol() && lex.atrib != E_com );
		//printf( "\nnom:%s", nom );
		//strcpy( *str, nom );
		//*str = zstr(*str);
		*str = zstr( nom );
		//char *tmp = strdup( nom );
		//printf( "\n\t\tstr:%s\n", *str );
		r = true;
	}
	else
		encolar( &iniMsjErr, &finMsjErr, (void*)"Se esperaban comillas dobles" );
		//mensaje_error( "Se esperaban comillas dobles" );

	return r;
}

int parId( int conComa, char **str ) {

	int r = false;
	struct lexico lex = Token(2);

	if( true == conComa ) {
		if( lex.atrib == E_coma ) {
			lex = Token(2);
		} else {
			encolar( &iniMsjErr, &finMsjErr, (void*)"Se esperaba una coma" );
			//mensaje_error( "Se esperaba una coma" );
			r = false;
			return r;
		} 
	}

	if( lex.atrib == E_texto ) {
		*str = zstr( lex.C_lex );
		r = true;
	}
	else {
		encolar( &iniMsjErr, &finMsjErr, (void*)"Se esperaba un identificador" );
		//mensaje_error( "Se esperaba un identificador" );	
	}

	return r;
}

void mensaje_error( const char *mensaje ) {
	printf( "\033[1;31m  |-> errmsg:\t%s\n", mensaje );
	printf( "\033[0m" );
}

void mensajeConLinea( const char *mensaje ) {
	//printf( "  Línea[ %d ]:\t%s\n", numlinea, mensaje );
	printf( "\033[0m" );
	printf( "  Línea[ %d ]:", numlinea );
	printf( "\033[1;36m\t%s\n", mensaje );
	printf( "\033[0m" );
}

void mensajeConLineaErr( const char *mensaje ) {
	printf( "\033[1;31m" );
	printf( "  Línea[ %d ]:", numlinea );
	printf( "\033[1;31m\t%s\n", mensaje );
	printf( "\033[0m" );
}