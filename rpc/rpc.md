Sistemas Distribuidos I - 2018 1C
Authors: Gaston Snaider, Martin Cura


Remote Procedure Calls
======================

Con el propósito de facilitar el desarrollo de sistemas distribuidos, se ideó un sistema de comunicación para poder ejecutar prodcedimientos en una máquina remota, como si estos se estuvieran ejecutando localmente. Este sistema se conoce como Remote Procedure Call (RPC), el cual fue publicado en febrero de 1984 por Birrell y Nelson en su paper [Implementing Remote Procedure Calls](https://www.cc.gatech.edu/classes/AY2009/cs4210_fall/papers/rpc-birrell84.pdf).


Objetivo de RPC
---------------

RPC permite al programador correr código remotamente tal como si se tratara de un procedimiento local y sin tener que preocuparse por los detalles de comunicación. Ya que el llamado a procedimientos y su transferencia de control son bien conocidos, se buscó mantener una interfaz que provea esta abstracción, y así no complicar al programador. Más aún, Birrell y Nelson buscaron que su implementación fuera eficiente y razonablemente rápida para que el programador no evite utilizar RPC. 

Funcionamiento general
----------------------

Cuando se invoca un procedimiento remoto, el cliente utiliza un _stub_ que normaliza los datos a un formato de red, y los vuelve un stream para enviarlo por la red utilizando _sockets_. Este proceso de normalización se llama _marshaling_. Al hacer esto, el proceso invocador es suspendido en espera de la respuesta. Cuando el llamado llega a la máquina remota, ocurre el proceso inverso en su propio _stub_, el cual ejecuta el procedimiento localmente. Al finalizar este, el servidor le devuelve el resultado a su _stub_, quien lo _marshaliza_ y lo envía a través de la red a la máquina cliente, cuyo _stub_ recibe, transformando al formato local si es necesario, y lo entrega al procedimiento del cliente que pidió el RPC, quien ahora resume su ejecución.

El proceso se puede ver resumido en el siguiente esquema.

![rpc-flow](https://www.cs.rutgers.edu/~pxk/417/notes/images/rpc-flow.png)

El pasaje de parámetros a un RPC puede realizarse tanto por valor como por referencia. Al pasarse por valor, los datos de los parámetros son copiados a los mensajes del RPC y se envian al servidor. En cambio, el pasaje por referencia es mucho más complejo, ya que en el caso de punteros, estos solo tienen sentido dentro del espacio de direcciones local en el que se están utilizando. Por lo tanto, para pasar un parámetro por referencia, estos deben enviarse al servidor como datos concretos, y si estos son modificados en el servidor, habrá que detectar este cambio, y luego replicarlo en el cliente.


Paso a paso
-----------
TODO


Comparacion RPC y LPC
---------------------
TODO


1) Describir RPC
2) Ejemplo p a p
3) a) ejecuc y tiempo LPC(), b) ejecuc y tiempo RPC