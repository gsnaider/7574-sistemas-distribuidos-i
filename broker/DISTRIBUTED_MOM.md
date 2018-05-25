# MOM distribuido
En este documento se detallan las decisiones de diseño tomadas para el desarrollo del sistema MOM de forma distribuida.
## DB centralizada
Se decidió mantener la base de datos de forma centralizada por varias razones:
- Todas las instancias de los brokers van a ver siempre la misma información. 
- La información que ve cualquier broker estará siempre actualizada.
- Al tener la DB centralizada, no hay que preocuparnos por replicar cambios de un nodo de la DB en otro nodo.
- Los **global_ids** del sistema se crean en la DB. Al tener la DB centralizada, no pueden haber duplicaciones de ids generados por distintos nodos.
## Arquitectura de los nodos broker
### Tabla de global_ids
Cada nodo broker mantendrá su tabla local de los **global_ids** creados a través de ese nodo. Esos usuarios solo podrán utilizar el MOM a través de dicho nodo. Esta restricción es necesaria, ya que cuando un usuario se registra, se crea un socket entre ese usuario y el nodo del broker a través del cual se registró. Sería muy complicado trasladar esta conexión a otro nodo.
### Conexión con la DB
Cada broker al inicializarse creará una conexión con la Base de datos para poder enviar las operaciones correspondientes (crear usuarios, subscribir, etc.).
### Conexión entre brokers
Casi todos los pedidos del cliente a un broker pueden resolverse solamente entre el cliente, su broker correspondiente, y la DB, sin tener que comunicarse con otros nodos broker: crear, subscribir, recibir, eliminar. Sin embargo, para publicar un mensaje, este debe ser enviado a todos los clientes subscriptos al topic, y estos clientes podrían pertenecer a otro nodo broker.
Para realizar el publish, por cada cliente subscripto el broker debe preguntarse: _¿Puedo enviar el mensaje a este cliente?_ es decir, _¿Tengo una conexión activa con este cliente?_. O más especificamente: _¿El global_id de este cliente está en mi tabla de global_ids?_ Si la respuesta es afirmativa, entonces el nodo simplemente reenvia el mensaje al cliente sin problemas. Pero si este broker no tiene una conexión con el cliente, entonces debe reenviar el mensaje a otro broker. Ese broker se hará las mismas preguntas que el anterior, y así sucesivamente hasta que algún broker pueda enviar el mensaje al cliente.
En caso de que ningún broker conozca al cliente, entones se deja de intentar enviarlo.
Para llevar a cabo este algorítmo, los brokers estarán conectados en una estructura de anillo, y cada broker tendrá un único broker al cual reenviar los mensajes en caso de no pertenecer a un cliente propio.


