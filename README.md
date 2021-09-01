# Aplicación cliente/servidor usando sockets en C 🤖
#### *Enviando estructuras (struct's) de longitud variable*

Proyecto realizado en C y compilado con C++11 (en virtud del uso de la sobrecarga de funciones y/o procedimientos). Para automatizar la ejecución de tareas, *compilación*, se usa la utilidad Makefile.

## Descripción
Usar socket's para comunicar una aplicación cliente con una aplicación servidor (IPC) en un ambiente de red permite las características de no compartir memoria ni reloj del procesador.
Usualmente se envían tipos de datos primitivos y compuestos (struct's) con una longitud fija. Esto puede afectar a una cadena de caracteres sobre todo cuando desconocemos su tamaño y debemos definirlo en tiempo de compilación. Sin embargo se propone la idea de crear estructuras (struct) de longitud variable para almacenar tipos de datos primitivos como int, int long, unsigned int long, boolean, entre otros y además tipo de datos primitivos String (cadena) esto último con el objetivo de darle mayor potencia a lo que se envie y no tener que conocer en tiempo de compilación el tamaño del string (cadena) sino en tiempo de ejecución.

## Características
Uso de librerías personalizadas para la manipulación de
1. Strings (cadenas de caracteres)
2. Listas, Pilas y Colas genéricas
3. Escaner (Scanner) para el parseo de líneas de comandos

Además se plantea la solución (sin compresión de datos) para enviar estructuras con longitud variable que consiste básicamente en partir una estructura variable en pequeños segmentos con longitud fija, en el lado del cliente, para luego ser enviados al servidor y ser reconstruidos, concatenando cada fragmento, para su posterior almacenamiento en memoria primaria usando estructuras lineales genéricas.

### Instalación
Asegurarse que se pueda usar en la terminal los siguientes comandos
- `g++ 7.5.0` o superior
- make

#### Compilación
`$ make`

#### Uso
Primero, abrir una terminal y ejecutar el servidor
> `$ ./servidor`

Después abrir otra terminal y ejecutar el cliente. Existen varias formas de ejecutarlo. Se debe recordar que **localhost** es equivalente a *127.0.0.1*

| Comando | Ejemplo                    |
| ------------- | ------------------------------ |
| `$ ./cliente ip -ops nombre-archivo`      | `$ ./cliente localhost -f archivos/entrada0` |
| `$ ./cliente ip`   | `$ ./cliente 127.0.0.1` |

Sino se provee la opción -f seguido del *directorio/nombre-archivo-entrada*, la aplicación se queda *escuchando* para que se introduzca cualquiera de los siguientes comandos. Todos los parámetros alfanuméricos deben ser enviados entre comillas dobles (**"**)

| Comando | Params                    | Respuesta |
| ------------- | ------------------------------ | ------------- |
| `$ CREAR_CLIENTE(params)`      | Nombre, Apellidos, No. de DPI, Domicilio, Teléfono móvil, Teléfono residencial, correo electrónico | [RETORNO: ID_CLIENTE] |
| `$ ENVIAR_MENSAJE(params)`   | ID de cliente emisor, ID de cliente receptor, Mensaje | [RETORNO: Mensaje éxito/error] |
| `$ OBTENER_MENSAJES(params)`   | ID de cliente | [RETORNO: Mensaje] |
| `$ LISTA(params)`   | M = Mensaje; C = Clientes | [RETORNO: Lista de mensajes/clientes] |
| `$ LIMPIAR`   | *ninguno* | [RETORNO: ninguno] |
| `$ SALIR`   | *ninguno* | [RETORNO: ninguno]. **Se terminan de ejecutar el cliente como el servidor** |
| `$ BUSCAR_CLIENTE(params)`   | ID de cliente/CUI | [RETORNO: Información completa del cliente] |

#### Limpiar/borrar archivos ejecutables
> `$ make clear`

Después de ejecutar esto se debe hacer el llamado a make