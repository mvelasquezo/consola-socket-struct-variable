#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include "zstring.h"
#include "lista.h"
#include "cola.h"
#include "scanner.h"
#include "estructura.h"

Nodo *ini = NULL, *fin = NULL, *iniMsj = NULL, *finMsj = NULL;
unsigned int corrCli = 0;

void mensajeConLinea( int numlinea, const char *mensaje ){
	printf( " Línea[ %d ]:\t%s\n", numlinea, mensaje );
}

int 
main( int argc, char *args[] ) {
    //$ g++ -o servidor -Iheaders servidor.c zstring.c lista.c scanner.c
    //$ make -f mks
    int sersock, newsock;
    struct sockaddr_in seraddr;
    struct sockaddr_in cliinfo;
    socklen_t csize         = sizeof(cliinfo);
    seraddr.sin_family      = AF_INET;
    seraddr.sin_port        = htons(SER_PORT);
    seraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    int primeraVez = true;

    if( (sersock = socket(AF_INET,SOCK_STREAM,0)) < 0 ) {
        perror( "\nSocket" );
        exit(EXIT_FAILURE);
    }

    if( bind( sersock,(struct sockaddr *)&seraddr,sizeof(seraddr)) < 0 ) {
        perror( "\nBIND" );
        exit(EXIT_FAILURE);
    }

    if( listen(sersock,1) < 0 ) {
        exit(EXIT_FAILURE);
    }

    if( (newsock = accept(sersock,(struct sockaddr *)&cliinfo,&csize)) < 0 ) {
        perror( "\nACCEPT" );
        exit(EXIT_FAILURE);
    }
    else
        printf( "\n Ahora esta conectado %s\n", inet_ntoa( cliinfo.sin_addr ) );

    InfoCliente *infCli = NULL;
    size_t len = -1;
    int pack;
    char buffer[ BUFFER_LEN ] = "";

    do {
        if( true == primeraVez ) {
            primeraVez = false;
        } else {
            listen( newsock, 1 );
        }

        strcpy( buffer, "" );
        read( newsock, buffer, sizeof( buffer ) );

        if( strcasecmp( buffer, "" ) == 0 ) continue;
        else if( strcasecmp( buffer, "SALIR" ) == 0 ) break;
        else if( strncasecmp( buffer, "LISTA", 5 ) == 0 ) {

            if( strcasecmp( buffer, "LISTA|C" ) == 0 ) {
                printf( "\n > Lista de clientes registrados:" );
                imprimirLista( ini, imprimirInfoCliente );
            }
            else if( strcasecmp( buffer, "LISTA|M" ) == 0 ) {
                printf( "\n > Cola de mensajes:" );
                imprimirLista( iniMsj, imprimirInfoMensaje );
            }
        }
        else if( strcasecmp( buffer, "OBTENER_MENSAJES" ) == 0 ) {
            
            Nodo *iniMsjB = NULL, *finMsjB = NULL;
            unsigned int long idOrigen;
            unsigned int r = false;

            listen( newsock, 1 );
            read( newsock, &idOrigen, sizeof( idOrigen ) );
            
            buscarDentroDeLista( iniMsj, compararInfoMensaje, &idOrigen, &iniMsjB, &finMsjB );

            if( estaVacia(iniMsjB) ) {
                r = false;
                write( newsock, &r, sizeof( r ) );
            }
            else {
                r = contarElementosLista( iniMsjB );
                write( newsock, &r, sizeof( r ) );
                
                if( !estaVacia( iniMsjB ) ) {
                    int errno = -1;
                    do {
                        InfoMensaje *infoMsj = (InfoMensaje*)descolar( &iniMsjB, &finMsjB, &errno );
                        
                        if( errno == 0 ) {

                            char *str   = serializarInfoMensaje( infoMsj );
                            size_t len  = strlen( str );
                            int pack    = calcPack( len );

                            write( newsock, &len, sizeof( len ) );

                            for( int i = 0; i < pack; i++ ) {
                                int ini = (i==0) ? 0 : i * BUFFER_LEN - i;
                                strcpy( buffer, "" );
                                strcpy( buffer, zsubstr( str, ini, BUFFER_LEN - 1 ) );
                                write( newsock, buffer, sizeof( buffer ) );
                            }

                        }
                    } while( errno == 0 );
                    printf( "\n" );
                }
            }
        }
        else if( strcasecmp( buffer, "LIMPIAR" ) == 0 ) {
            system( "clear" );
            printf( "\n" );
        }
        else if( strcasecmp( buffer, "BUSCAR_CLIENTE" ) == 0 ) {

            infCli = (InfoCliente*) malloc( sizeof( InfoCliente ) );
            InfoBuscarCliente *infBCli = (InfoBuscarCliente*) malloc( sizeof( InfoCliente ) );

            listen( newsock, 1 );
            read( newsock, infBCli, sizeof( InfoBuscarCliente ) );

            int resultadoBusqueda = buscarDentroDeLista( ini, compararInfoCliente, &infBCli -> cui, (void**)&infCli );

            if( false ==  resultadoBusqueda || -1 == resultadoBusqueda ) {

                resultadoBusqueda = -1;
                char str[ 100 ] = { '\0' } ; 
                sprintf( str, "Cliente con No. de DPI/id '%lu' no fue encontrado", infBCli -> cui );
                mensajeConLinea( infBCli -> numlinea, str );

            } else {
                resultadoBusqueda = 1;
                char str[ 100 ] = { '\0' } ; 
                sprintf( str, "Cliente con No. de DPI/id '%lu' encontrado", infBCli -> cui );
                mensajeConLinea( infBCli -> numlinea, str );
                imprimirInfoCliente( infCli );
            }

            write( newsock, &resultadoBusqueda, sizeof(resultadoBusqueda)  );

            if( resultadoBusqueda == 1 ) {
                char *str = serializarInfoCliente( infCli );
                size_t len = strlen(str);
                int pack = calcPack( len );

                write( newsock, &len, sizeof( len ) );

                for( int i = 0; i < pack; i++ ) {
                    int ini = (i==0) ? 0 : i * BUFFER_LEN - i;
                    strcpy( buffer, "" );
                    strcpy( buffer, zsubstr( str, ini, BUFFER_LEN - 1 ) );
                    write( newsock, buffer, sizeof( buffer ) );
                }
            }
        }
        else if( strcasecmp( buffer, "ENVIAR_MENSAJE" ) == 0 ) {

            InfoCliente *infO = (InfoCliente*) malloc( sizeof( InfoCliente ) );
            InfoCliente *infD = (InfoCliente*) malloc( sizeof( InfoCliente ) );
            InfoMensaje *infM = (InfoMensaje*)malloc( sizeof( InfoMensaje ) );

            listen( newsock, 1 );
            read( newsock, &len, sizeof( size_t ) );
            pack = calcPack( len );

            char *nwStr = (char*)malloc(1);
            for( int i = 0; i < pack; i++ ) {
                listen( newsock, 1 );
                read( newsock, buffer, BUFFER_LEN );
                nwStr = zstr( nwStr, (char*)buffer );
            }

            char *strNum = zstring_strtok( nwStr, "#" );
            char *strIdO = zstring_strtok( NULL, "#" );
            char *strIdD = zstring_strtok( NULL, "#" );
            char *strMsj = zstring_strtok( NULL, "#" );
            char *strEst = zstring_strtok( NULL, "#" );

            infCli = (InfoCliente*) malloc( sizeof( InfoCliente ) );

            char with[] = "#";
            char rep[] = "\\#";

            strMsj = zstr_replace( strMsj, rep, with );

            infM -> numlinea = atoi( strNum );
            infM -> idOrigen = atoll( strIdO );
            infM -> idDestino = atoll( strIdD );
            infM -> mensaje = strMsj;
            infM -> estado = atoi( strEst );

            //imprimirInfoMensaje(infM);

            int o1 = buscarDentroDeLista( ini, compararInfoClienteConId, &(infM -> idOrigen ), (void**)&infO );
            int d1 = buscarDentroDeLista( ini, compararInfoClienteConId, &(infM -> idDestino ), (void**)&infD );

            unsigned int errno = 0;
            if( o1 <= 0 ) {
                errno = -1;
                write( newsock, &errno, sizeof( errno ) );
                
                char str[ 100 ] = { '\0' } ;
                sprintf( str, "Origen '%lu' no existe", infM ->idOrigen );
                mensajeConLinea( infM->numlinea, str );
            }
            else if( d1 <= 0 ) {
                errno = -2;
                write( newsock, &errno, sizeof( errno ) );

                char str[ 100 ] = { '\0' } ;
                sprintf( str, "Destinatario '%lu' no existe", infM ->idDestino );
                mensajeConLinea( infM->numlinea, str );
            }
            else if( true == o1 && true == d1 ) {
                infM -> estado = RECIBIDO;
                encolar( &iniMsj, &finMsj, infM );
                write( newsock, &infM -> estado, sizeof( infM -> estado ) );
            }
        }
        else if( strcasecmp( buffer, "CREAR_CLIENTE" ) == 0 ) {

            InfoCliente *infCliB = (InfoCliente*) malloc( sizeof( InfoCliente ) );

            listen( newsock, 1 );
            read( newsock, &len, sizeof( size_t ) );
            pack = calcPack( len );

            char *nwStr = (char*)malloc(1);
            for( int i = 0; i < pack; i++ ) {
                listen( newsock, 1 );
                read( newsock, buffer, BUFFER_LEN );
                //printf( "buffer: '%s'\n", buffer );
                nwStr = zstr( nwStr, (char*)buffer );
            }

            //printf( " > rec: '%s'\n", nwStr );

            char *strNum = zstring_strtok( nwStr, "#" );
                           zstring_strtok( NULL, "#" );
            char *strCui = zstring_strtok( NULL, "#" );
            char *strNom = zstring_strtok( NULL, "#" );
            char *strApe = zstring_strtok( NULL, "#" );
            char *strDom = zstring_strtok( NULL, "#" );
            char *strMov = zstring_strtok( NULL, "#" );
            char *strTel = zstring_strtok( NULL, "#" );
            char *strCor = zstring_strtok( NULL, "#" );

            infCli = (InfoCliente*) malloc( sizeof( InfoCliente ) );

            char with[] = "#";
            char rep[] = "\\#";

            strDom = zstr_replace( strDom, rep, with );

            infCli -> numlinea = atoi( strNum );
            infCli -> cui = atoll( strCui );
            infCli -> nombre = strNom;
            infCli -> apellidos = strApe;
            infCli -> domicilio = strDom;
            infCli -> movil = atoll( strMov );
            infCli -> telefono = atoll( strTel );
            infCli -> correoe = strCor;

            //imprimirLista( ini, imprimirInfoCliente );
            int resultadoBusqueda = buscarDentroDeLista( ini, compararInfoCliente, &(infCli -> cui ), (void**)&infCliB );
                
            if( false ==  resultadoBusqueda || -1 == resultadoBusqueda ) {
                infCli -> id = resultadoBusqueda = ++corrCli;
                insertarAlFinal( &ini, &fin, infCli );
                mensajeConLinea( infCli -> numlinea, "comando correcto" );
            } else {
                resultadoBusqueda = -1;
                char str[ 100 ] = { '\0' } ; 
                sprintf( str, "No. de DPI '%lu' repetido, no será tomada en cuenta la instrucción", infCli -> cui );
                mensajeConLinea( infCli->numlinea, str );
            }

            write( newsock, &resultadoBusqueda, sizeof(resultadoBusqueda)  );
        }
    } while( 1 );

    close( newsock );
    close( sersock );

    return EXIT_SUCCESS;
}
//#include<stdio.h>
//#include<stdlib.h>
//system( "clear" );