# MOM
Este proyecto es el MOM (Message-oriented middleware) desarrollado para la asignatura 75.74 Sistemas Distribuidos I de FIUBA.

## Requerimientos
El proyecto está desarrollado en C11, y para compilarlo se requiere tener instalado:
 - [GNU Make](https://www.gnu.org/software/make/) 4.1 o superior
 - [CMake](https://cmake.org/) 3.11.1 o superior

## Configuración
Los parámetros como IPs y números de puertos se configuran en los siguientes archivos:
 - `broker/client/broker_handler.h`: contiene la IP y el puerto del server al cual se conectara el cliente.
 - `broker/server/server.h`: Contiene el puerto en que levanta el server, y el ID del server (BROKER_ID), el cual debe ser único entre los demás servers.
 - `broker/server/db-connection.h`: Contiene la IP y puerto de la base de datos a la cual se conectará el server.
 - `broker/db-server/db_server.h`: Contiene el puerto en el que se levanta la DB.

## Compilación
Para compilar el proyecto, desde el directorio `broker` ejecutar:
```sh
$ mkdir build
$ cd build
$ cmake ../
$ make
```
## Ejecución
El proyecto consiste de varios archivos de ejecución que se deben corren desde el directorio `build`.
### Database
Para comenzar, se debe levantar la base de datos en la máquina destinada a ser la DB, con el siguiente commando:
```sh
$ ./db_server
```
#### Server
Para levantar cada uno de los servers del MOM, ejecutar en cada server:
```sh
$ ./server
```
#### Conexión ring
Una vez que todos los servers hallan sido levantados, en cada server se debe ejecutar el siguiente script para iniciar la conexión anillo entre los servers:
```sh
$ ./ring_connection
```

#### Broker local
Para levantar el broker local (del lado del cliente):
```sh
$ ./broker_req_handler
```
Debe haber solo un broker local por cada máquina cliente.
#### Cliente
Finalmente, para levantar el cliente, ejecutar:
```sh
$ ./client
```
El cliente consiste de una interfaz de consola que permite subscribirse a topics, enviar mensajes y recibir nuevos mensajes.
Pueden ejecutarse más de un cliente a la vez en cada máquina, siempre y cuando se esté ejecutando el Broker local en dicha máquina.

Los demás archivos de ejecución dentro del directorio `build` no deben ser ejecutados por el usuario, estos serán ejecutados por distintos procesos del sistema.