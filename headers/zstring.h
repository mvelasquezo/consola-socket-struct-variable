//_string.h
#ifndef __STRING_H
   #define __STRING_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char *zscanf(void);
void zstruprV0(char*);
char *zstrupr(char*);
char *zstr_replace( char*, char*, char* );
char *zstring_strtok( char*, const char* );
char *zULIntAStr( const unsigned long int );
char *zitoa( unsigned long long, char[], int );
long zstrAULInt( const char* );
char *zsubstr( char*, int, int );

char *zstr(char*);
char *zstr(char*,char*);
char *zstr(char*,const char*,char*);

   #endif