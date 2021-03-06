# Aplicaci贸n cliente/servidor usando sockets en C 馃
#### *Enviando estructuras (struct's) de longitud variable*

Proyecto realizado en C y compilado con C++11 (en virtud del uso de la sobrecarga de funciones y/o procedimientos). Para automatizar la ejecuci贸n de tareas, *compilaci贸n*, se usa la utilidad Makefile.

## Descripci贸n
Usar socket's para comunicar una aplicaci贸n cliente con una aplicaci贸n servidor (IPC) en un ambiente de red permite las caracter铆sticas de no compartir memoria ni reloj del procesador.
Usualmente se env铆an tipos de datos primitivos y compuestos (struct's) con una longitud fija. Esto puede afectar a una cadena de caracteres sobre todo cuando desconocemos su tama帽o y debemos definirlo en tiempo de compilaci贸n. Sin embargo se propone la idea de crear estructuras (struct) de longitud variable para almacenar tipos de datos primitivos como int, int long, unsigned int long, boolean, entre otros y adem谩s tipo de datos primitivos String (cadena) esto 煤ltimo con el objetivo de darle mayor potencia a lo que se envie y no tener que conocer en tiempo de compilaci贸n el tama帽o del string (cadena) sino en tiempo de ejecuci贸n.

## Caracter铆sticas
Uso de librer铆as personalizadas para la manipulaci贸n de
1. Strings (cadenas de caracteres)
2. Listas, Pilas y Colas gen茅ricas
3. Escaner (Scanner) para el parseo de l铆neas de comandos

Adem谩s se plantea la soluci贸n (sin compresi贸n de datos) para enviar estructuras con longitud variable que consiste b谩sicamente en partir una estructura variable en peque帽os segmentos con longitud fija, en el lado del cliente, para luego ser enviados al servidor y ser reconstruidos, concatenando cada fragmento, para su posterior almacenamiento en memoria primaria usando estructuras lineales gen茅ricas.

### Instalaci贸n
Asegurarse que se pueda usar en la terminal los siguientes comandos
- `g++ 7.5.0` o superior
- make

#### Compilaci贸n
`$ make`

#### Uso
Primero, abrir una terminal y ejecutar el servidor
> `$ ./servidor`

Despu茅s abrir otra terminal y ejecutar el cliente. Existen varias formas de ejecutarlo. Se debe recordar que **localhost** es equivalente a *127.0.0.1*

| Comando | Ejemplo 聽 聽 聽 聽 聽 聽 聽 聽 聽 聽|
| ------------- | ------------------------------ |
| `$ ./cliente ip -ops nombre-archivo` 聽 聽 聽| `$ ./cliente localhost -f archivos/entrada0` |
| `$ ./cliente ip` 聽 | `$ ./cliente 127.0.0.1` |

Sino se provee la opci贸n -f seguido del *directorio/nombre-archivo-entrada*, la aplicaci贸n se queda *escuchando* para que se introduzca cualquiera de los siguientes comandos. Todos los par谩metros alfanum茅ricos deben ser enviados entre comillas dobles (**"**)

| Comando | Params 聽 聽 聽 聽 聽 聽 聽 聽 聽 聽| Respuesta |
| ------------- | ------------------------------ | ------------- |
| `$ CREAR_CLIENTE(params)` 聽 聽 聽| Nombre, Apellidos, No. de DPI, Domicilio, Tel茅fono m贸vil, Tel茅fono residencial, correo electr贸nico | [RETORNO: ID_CLIENTE] |
| `$ ENVIAR_MENSAJE(params)` 聽 | ID de cliente emisor, ID de cliente receptor, Mensaje | [RETORNO: Mensaje 茅xito/error] |
| `$ OBTENER_MENSAJES(params)` 聽 | ID de cliente | [RETORNO: Mensaje] |
| `$ LISTA(params)` 聽 | M = Mensaje; C = Clientes | [RETORNO: Lista de mensajes/clientes] |
| `$ LIMPIAR` 聽 | *ninguno* | [RETORNO: ninguno] |
| `$ SALIR` 聽 | *ninguno* | [RETORNO: ninguno]. **Se terminan de ejecutar el cliente como el servidor** |
| `$ BUSCAR_CLIENTE(params)` 聽 | ID de cliente/CUI | [RETORNO: Informaci贸n completa del cliente] |

#### Limpiar/borrar archivos ejecutables
> `$ make clear`

Despu茅s de ejecutar esto se debe hacer el llamado a make