# ContabilidApp
ContabilidApp es una aplicación web que permite la carga de archivos de transacciones de usuarios, la descarga de esta información, y realizar controles sobre la misma.

# Arquitectura
La aplicación está desarrollada en Java EE, y utiliza Apache Tomcat como servidor web. La aplicación está distribuida en dos servidores: 
1. Un servidor frontend que maneja la vista de la aplicación. Este servidor utiliza el Java Server Faces (JSF) y PrimeFaces como frameworks de frontend para Java EE.
2. Un servidor backend que contiene la lógica de negocio y el modelo de datos. Este servidor utiliza Java Persistence API (JPA) y su implementación de Hibernate como frameworks para ORM (Object Relational Mapping).

A su vez, el servidor backend tiene una API REST para las distintas operaciones de negocio, y el servidor frontend consume dicha API para conectarse con el servidor backend.

Por último, como base de datos se utiliza MySQL Cluster, una base de datos distribuida que utiliza el motor de base de datos MySQL. Al ser distribuida, esta base de datos permite tener varios nodos de administración y almacenamiento de datos. Esto nos trae varios beneficios, como por ejemplo la posibilidad de escalar horizontalmente, contar con mayor disponibilidad, y evitar tener un single point of failure.

A continuación se muestra un diagrama de la arquitectura completa del sistema.

<img src="./doc/contabilidapp-architecture.png" alt="Arquitectura">

# Instalación
Las siguientes instrucciones describen como realizar la instalación del sistema sobre Linux.
## Pre-requisitos
### Java 8
Para correr la aplicación, se debe tener instalado Java 8. Para instalarlo, simplemente ejecutar:
```sh
$ sudo apt-get install openjdk-8-jdk
```
Para verificar que Java 8 se instaló correctamente, ejecutar:
```sh
$ java -version
```
y se debería obtener un resultado similar al siguiente:
```sh
openjdk version "1.8.0_171"
OpenJDK Runtime Environment (build 1.8.0_171-8u171-b11-0ubuntu0.16.04.1-b11)
OpenJDK 64-Bit Server VM (build 25.171-b11, mixed mode)
```
### Apache Tomcat
Tanto el servidor frontend como el backend corren sobre Apache Tomcat. En este caso, utilizamos la version 9.0.10 de Tomcat, la cual puede ser descargada directamente del siguiente link: http://www.gtlib.gatech.edu/pub/apache/tomcat/tomcat-9/v9.0.10/bin/apache-tomcat-9.0.10.tar.gz, o desde: https://tomcat.apache.org/download-90.cgi.
Una vez realizada la descarga, extraer el contenido, crear un directorio en `/opt/tomcat` y mover el directorio `apache-tomcat-9.0.10` a este nuevo directorio:
```sh
$ tar -xvzf apache-tomcat-9.0.10.tar.gz
$ sudo mkdir /opt/tomcat
$ sudo mv apache-tomcat-9.0.10 /opt/tomcat/
```
### MySQL Cluster
#### Pre-requisitos de MySQL Cluster
Para poder instalar MySQL Cluster, previamente se deben instalar algunas dependencias. Para empezar, ejecutar:
```sh
$ sudo apt-get install libaio1
```
En caso de no tener Python instalado, ejecutar:
```sh
$ sudo apt-get install python2.7
```
Verificar la instalación de Python con:
```sh
$ python --version
```
Luego se deben instalar algunos paquetes de Python. En caso de no tener pip instalado, ejecutar:
```sh
$ sudo apt-get install python-pip
```
Y luego descargar los siguientes paquetes de Python con pip:
```sh
$ pip install paramiko
$ pip install pycrypto
```
#### Instalación de MySQL Cluster
(Las instrucciones oficiales pueden encontrarse en: https://downloads.mysql.com/tutorials/cluster/mysql_wp_cluster_quickstart.pdf y https://dev.mysql.com/doc/refman/5.7/en/mysql-cluster-install-auto-using.html)

MySQL Cluster puede descargarse desde: https://dev.mysql.com/downloads/cluster/. Seleccionar la opción **Linux - Generic**, elegir la arquitectura del sistema, y hacer click en **Download**.

<img src="./doc/mysql-cluster-download.png" alt="MySQL Cluster Download">

Una vez descargado, extraer el directorio y moverlo a `/usr/local/mysql`
```sh
$ tar -xvzf mysql-cluster-gpl-7.6.6-linux-glibc2.12-x86_64.tar.gz
$ mv mysql-cluster-gpl-7.6.6-linux-glibc2.12-x86_64 mysql
$ sudo mv mysql /usr/local
```
Una vez hecho esto, ir al directorio `/usr/local/mysql` y ejecutar `bin/ndb_setup.py`:
```sh
$ cd /usr/local/mysql
$ bin/ndb_setup.py
```
Esto abrirá en un browser la URL https://localhost:8081/welcome.html. En caso de no abrirse automáticamente, copiar la URL y abrirla manualmente desde un browser. Esto iniciará el Wizard para la instalación de MySQL Cluster.

<img src="./doc/mysql-cluster-1.png" alt="MySQL Cluster 1">

Aquí podemos crear una nueva configuración para nuestro cluster y deployarlo. Comenzamos seleccionando **NEW CONFIGURATION**, y seteando una Passphrase para nuestro cluster (por ejemplo: 'mysql'). Luego hacemos click en **Continue**.

<img src="./doc/mysql-cluster-2.png" alt="MySQL Cluster 2">

Ahora elegimos un nombre para nuestro cluster (por ejemplo: 'contabilidapp-cluster'), y hacemos click en **OK**.

<img src="./doc/mysql-cluster-3.png" alt="MySQL Cluster 3">

En esta pantalla podemos especificar los hosts que utilizaremos para nuestro cluster. Como en este ejemplo lo estamos corriendo de forma local, solo especificamos un host: 127.0.0.1, pero podrían agregarse más hosts separados por coma. Podemos especificar el tipo de aplicación y la carga de escritura (en este caso dejamos las opciones por default). En la sección de **SSH property** especificamos el usuario y password que se utilizará para la conexión al host (en este caso usamos el usuario y password de la máquina local). Finalmente, podemos elegir abrir los puertos de Firewall para la  aplicación. Luego de todo esto, hacemos click en **Save&Next**.

<img src="./doc/mysql-cluster-4.png" alt="MySQL Cluster 4">

En la siguiente pantalla podemos visualizar los hosts que se utilizarán para el cluster. Si quisieramos, podemos agregar o quitar hosts, o editar la configuración de cada uno. Hacemos click en **Save&Next**.

<img src="./doc/mysql-cluster-5.png" alt="MySQL Cluster 5">

En esta pantalla podemos visualizar los nodos que se crearán en cada host. Si queremos podemos agregar o quitar nodos en cada host. Nuevamente, dejamos la configuración por defecto, que en particular utiliza un management node, y dos multithreaded data nodes. Para más información sobre los distintos nodos de MySQL Cluster, ver: https://dev.mysql.com/doc/refman/5.7/en/mysql-cluster-basics.html. Hacemos click en **Save&Next**.

<img src="./doc/mysql-cluster-6.png" alt="MySQL Cluster 6">

En esta pantalla vemos cómo quedarían las distintas capas correspondientes a los distintos tipos de nodos en cada host. Si hacemos click en **Show advanced configuration options**, y nos paramos sobre algún Layer o nodo, podemos editar algunos parámetros de configuración, como por ejemplo la cantidad de memoria a asignar a cada nodo. Dejamos las configuraciones por default, y hacemos click en **Save&Next**.

<img src="./doc/mysql-cluster-7.png" alt="MySQL Cluster 7">

En esta última pantalla, podemos deployar e iniciar nuestro cluster. Hacemos click en **Deploy cluster**, y deberíamos obtener el siguiente mensaje:

<img src="./doc/mysql-cluster-8.png" alt="MySQL Cluster 8">

Luego, hacemos click en **Start cluster**, y deberíamos obtener el siguiente mensaje:

<img src="./doc/mysql-cluster-9.png" alt="MySQL Cluster 9">

Una vez hecho esto, ya tenemos corriendo un MySQL Cluster en nuestra máquina. Para monitorear el cluster, podemos ejecutar desde `/usr/local/mysql/`:
```sh
$ bin/ndb_mgm -e show
```
Esto nos mostrará el estado de los distintos nodos corriendo en nuestro host. Deberíamos ver algo como esto:
```sh
Connected to Management Server at: localhost:1186
Cluster Configuration
---------------------
[ndbd(NDB)]	2 node(s)
id=1	@127.0.0.1  (mysql-5.7.22 ndb-7.6.6, Nodegroup: 0, *)
id=2	@127.0.0.1  (mysql-5.7.22 ndb-7.6.6, Nodegroup: 0)

[ndb_mgmd(MGM)]	1 node(s)
id=49	@127.0.0.1  (mysql-5.7.22 ndb-7.6.6)

[mysqld(API)]	5 node(s)
id=53	@127.0.0.1  (mysql-5.7.22 ndb-7.6.6)
id=54	@127.0.0.1  (mysql-5.7.22 ndb-7.6.6)
id=231 (not connected, accepting connect from 127.0.0.1)
id=232 (not connected, accepting connect from 127.0.0.1)
id=233 (not connected, accepting connect from 127.0.0.1)
```

## Configuración de base de datos
### Credenciales
Una vez levantado el MySQL Cluster, debemos configurar el usuario y password que usaremos para conectarnos desde nuestra Web App. Para entrar a la base de datos, ejecutar desde `/usr/local/mysql/`:
```sh
$ bin/mysql -h 127.0.0.1 -P 3306 -u root
```
Una vez conectado a la base de datos, ejecutar:
```sh
mysql> use mysql;
mysql> update user set authentication_string=password('mysql') where user='root';
mysql> flush privileges;
mysql> quit;
```
Esto setea la password 'mysql' para el usuario 'root', que son las credenciales que utilizaremos para conectarnos desde nuestra app. A partir de ahora, si queremos conectarnos a la base de datos, debemos ejecutar:
```sh
$ bin/mysql -h 127.0.0.1 -P 3306 -u root -p
```
y tipear la password 'mysql'.

### Cargar schema
Ahora cargaremos el schema de la base de datos que usaremos en nuestra aplicación. Para eso, 