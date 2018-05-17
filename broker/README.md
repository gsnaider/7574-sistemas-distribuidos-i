# MOM
Este proyecto es el MOM (Message-oriented middleware) desarrollado para la asignatura 75.74 Sistemas Distribuidos I de FIUBA.

## Requerimientos
El proyecto está desarrollado en C11, y para compilarlo se requiere tener instalado:
 - [GNU Make](https://www.gnu.org/software/make/) 4.1 o superior
 - [CMake](https://cmake.org/) 3.11.1 o superior

## Compilar
Para compilar el proyecto, desde el directorio `broker` ejecutar:
```sh
$ mkdir build
$ cd build
$ cmake ../
$ make
```
## Ejecutar
El proyecto consiste de varios archivos de ejecución que se deben corren desde el directorio `build`.
#### Server
Para levantar el server del MOM, ejecutar:
```sh
$ ./server
```
#### Broker local
Para levantar el broker local (del lado del cliente):
```sh
$ ./broker_req_handler
```
#### Cliente
Finalmente, para levantar el cliente, ejecutar:
```sh
$ ./client
```
El cliente consiste de una interfaz de consola que permite subscribirse a topics, enviar mensajes y recibir nuevos mensajes.
Pueden ejecutarse más de un cliente a la vez en la misma máquina, siempre y cuando se esté ejecutando el Broker local en dicha máquina.

Los demás archivos de ejecución dentro del directorio `build` no deben ser ejecutados por el usuario, estos serán ejecutados por distintos procesos del sistema.

### Configuración
Actualmente el proyecto está configurado para que el servidor corra sobre el puerto 8080, y el cliente se intentará conectar a ese puerto en la dirección 127.0.0.1. Para modificar estos parámetros, se deberá cambiar del lado del cliente el archivo `broker/client/broker_handler.h`, especificando la IP y el puerto del server, y del lado del server se puede cambiar el puerto modificandolo en el archivo `broker/server/server.h`.