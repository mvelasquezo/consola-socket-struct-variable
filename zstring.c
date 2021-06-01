#include "zstring.h"

#define MAX_MULT (120)
const int BASE_NUMERACION = 127;

char *zscanf(void) {
	int chr = 0, mult = 0, len = 0; 
	char *_c = NULL;

  	while( 1 ) {
    	chr = fgetc( stdin );

    	if( chr == EOF || chr == '\n' ) /* Detente solo si terminamos o si es final de linea */
    		break;

    	if (mult >= len) { /* pide mas memoria si es necesario. */
      		char *tmp = (char*)realloc( _c, mult + MAX_MULT );
      		
      		if( !tmp ) /* si falla al pedir memoria, entonces detente ahora */
        		break;

      		_c = tmp, mult += MAX_MULT;
        }

        _c[ len++ ] = chr; /* finalmente asigna. */
    }
    
    if( len )
        _c[ len ] = 0; /* pon el caracter nulo al final si no es NULL nuestra cadena */
    else {
        _c = (char*)malloc( 1 );
        _c[ 0 ] = '\0';
    }

    return _c; /* finalmente, devuelvela. */
}
#undef MAX_MULT

void zstruprV0( char *s ) {
	char *p = s;

	while( *p ) {
		*p = toupper(*p);
		++p;
	}
}

char *zstrupr( char *str ) {

    if( NULL == str )    
        return NULL;

    char *p = zstr(str);
    int j = 0;

	while( (p[ j ] = (char)toupper((int)p[ j ])) )
        j++;

	return p;
    
    /*char *p = str;
    while( *str ) {
        *str = toupper(*str);   
        str++;   
    }
    return p;*/
}

char *zstr_replace( char *orig, char *rep, char *with ) {
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep (the string to remove)
    int len_with; // length of with (the string to replace rep with)
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    // sanity checks and initialization
    if (!orig || !rep)
        return NULL;
    len_rep = strlen(rep);
    if (len_rep == 0)
        return NULL; // empty rep causes infinite loop during count

    char *tmpStr = NULL;

    if (!with) {
        //with = {""};
        len_with = 0;
    } else {
        size_t tam = strlen( with ) + 1;
	    tmpStr = (char*) malloc ( tam );
	    strcpy( tmpStr, with );
        len_with = strlen(with);
    }
    //if (!with)
    //    with = {""};
    //len_with = strlen(with);

    // count the number of replacements needed
    ins = orig;
    for (count = 0; (tmp = strstr(ins, rep)); ++count) {
        ins = tmp + len_rep;
    }

    tmp = result = (char*)malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        //tmp = strcpy(tmp, with) + len_with;
        tmp = strcpy(tmp, tmpStr) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    
    return result;
}

char *zstring_strtok( char *str, const char *delim ) {
    static char *static_str=0;      /* var to store last address */
    int index=0, strlength=0;       /* integers for indexes */
    int found = 0;                  /* check if delim is found */

    /* delimiter cannot be NULL
    * if no more char left, return NULL as well
    */
    if (delim==0 || (str == 0 && static_str == 0))
        return 0;

    if (str == 0)
        str = static_str;

    /* get length of string */
    while(str[strlength])
        strlength++;

    /* find the first occurance of delim */
    for (index=0;index<strlength;index++)
        if (str[index]==delim[0] && str[index-1]!='\\' ) {
            found=1;
            break;
        }

    /* if delim is not contained in str, return str */
    if (!found) {
        static_str = 0;
        return str;
    }

    /* check for consecutive delimiters
    *if first char is delim, return delim
    */
    if (str[0]==delim[0]) {
        static_str = (str + 1);
        return (char *)delim;
    }

    /* terminate the string
    * this assignmetn requires char[], so str has to
    * be char[] rather than *char
    */
    str[index] = '\0';

    /* save the rest of the string */
    if ((str + index + 1)!=0) {
        static_str = (str + index + 1);
    } else {
        static_str = 0;
    }
    //printf( "static: %s\n", static_str );
    return str;
}

long zstrAULInt( const char *clave ) {
	long d = 0; int j, len = strlen( clave );
	for( j = 0; j < len; j++ ) {
		d = d * BASE_NUMERACION + clave[ j ];
    }
	return ( ( d >= 0 ) ? d : -d );
}

char *zitoa( unsigned long long  value, char str[], int radix ) {
    char        buf [66];
    char*       dest = buf + sizeof(buf);
    int         sign = 0;

    if (value == 0) {
        memcpy (str, "0", 2);
        return str;
    }

    if (radix < 0) {
        radix = -radix;
        if ( (long long) value < 0) {
            value = -value;
            sign = 1;
        }
    }

    *--dest = '\0';

    switch (radix)
    {
    case 16:
        while (value) {
            * --dest = '0' + (value & 0xF);
            if (*dest > '9') *dest += 'A' - '9' - 1;
            value >>= 4;
        }
        break;
    case 10:
        while (value) {
            *--dest = '0' + (value % 10);
            value /= 10;
        }
        break;

    case 8:
        while (value) {
            *--dest = '0' + (value & 7);
            value >>= 3;
        }
        break;

    case 2:
        while (value) {
            *--dest = '0' + (value & 1);
            value >>= 1;
        }
        break;

    default:            // The slow version, but universal
        while (value) {
            *--dest = '0' + (value % radix);
            if (*dest > '9') *dest += 'A' - '9' - 1;
            value /= radix;
        }
        break;
    }

    if (sign) *--dest = '-';

    memcpy (str, dest, buf +sizeof(buf) - dest);
    return str;
}

char *zULIntAStr( const unsigned long int clave ) {
	size_t tam = 1;
	char *str = (char*) malloc ( tam );

    sprintf( str, "%lu", clave ); //%lu l = long; u = unsigned int; d = int
	return str;
}

/*char *zsubstring( char *string, int position, int length ) {
   char *p;
   int c;
 
   p = (char*)malloc(length+1);
   
   if (p == NULL) {
      printf("Unable to allocate memory.\n");
      exit(1);
   }
 
   for (c = 0; c < length; c++) {
      *(p+c) = *(string+position-1);
      string++;  
   }
 
   *(p+c) = '\0';
 
   return p;
}*/

char *zsubstr( char *s, int a, int b ){
    char *r = (char*)malloc(b);
    strcpy( r, "" );
    int m = 0, n = 0;
    //printf( "[%c]", s[a-1] );
    while( s[ n ] != '\0' ) {
        if( n >= a && m < b ){
            r[ m ] = s[ n ];
            m++;
            //printf( "'%c' ", r[m] );
        }   
        n++;
    }
    r[ m ] = '\0';
    return r;
}

char *zstr( char *str1 ) {
	size_t tam = strlen( str1 ) + 1;
	char *str = (char*) malloc( tam );
	strcpy( str, str1 );
	return str;
}

char *zstr( char *str1, char *str2 ) {
    size_t len1 = strlen( str1 );
    size_t len2 = strlen( str2 );
    char *tmp = (char*) malloc ( ( len1 + len2 + 1 ) );
    strcpy( tmp, str1 );
	strcat( tmp, str2 );

	return tmp;
}

char *zstr( char *str1, const char *sep, char *str2 ) {
	char *tmp = (char*) malloc ( strlen( str1 ) );
	strcpy( tmp, str1 );

	size_t tam = strlen( str1 ) + strlen( str2 ) + 2;
	char *str = (char*) malloc ( tam );
	strcpy( str, str1 );
	strcat( str, sep );
	strcat( str, str2 );

	return str;
}