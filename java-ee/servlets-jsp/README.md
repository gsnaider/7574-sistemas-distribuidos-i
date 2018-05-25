# Java EE: Servlets & JSP
En este documento se explica como implementar una aplicación web utilizando las herramientas que provee Java EE, en particular con Servlets y JSPs.

## Requerimientos
Se requiere tener instalado:
 - [Java 8 SE](http://www.oracle.com/technetwork/java/javase/downloads/index.html)
 - [Tomcat 9](https://tomcat.apache.org/download-90.cgi)
 - No es un requisito, pero en este documento se explcarán los pasos utilizando el IDE [IntelliJ IDEA](https://www.jetbrains.com/idea/)

## 1. Crear el proyecto

En IntelliJ, ir a **_File->New->Proyect_** y elegir la opción **_Java Enterprise_**.

<img src="./images/new-project-1.png" alt="New project 1">


Seleccionar el SDK 1.8, Java EE 8, y Tomcat 9. No es necesario ningún framework o librería adicional.

En caso de no tener el SDK configurado, hacer click en **_New_** y elegir el directorio donde está instalado Java.

<img src="./images/sdk-config.png" alt="SDK config">


En caso de no tener el Application Server configurado, hacer click en **_New->Tomcat Server_**,  y elegir tanto para **_Tomcat HOME_** como para **_Tomcat base directory_** el directorio donde está instalado Tomcat.

<img src="./images/tomcat-config.png" alt="Tomcat config">


Hacer click en **_Next_**, y en la siguiente pantalla tildar la opción **_Create project from template_**, y elegir **_Java Web Module_**.

<img src="./images/new-project-2.png" alt="New project 2">


Hacer click en next **_Next_**, elegir un nombre y una ubicación para el proyecto. Dejar los demas parámetros por default, y hacer click en **_Finish_**

<img src="./images/new-project-3.png" alt="New project 3">


Finalmente, deberiamos tener un proyecto creado con la siguiente estructura:

<img src="./images/project-created.png" alt="Project created">

## 2. Crear el modelo
El primer paso será crear nuestro modelo de datos. Para esto, crear un nuevo paquete en el directorio `src` con el nombre `com.example.model`, y dentro de este paquete crear una clase Java con nombre `Greeting`. Esta clase tendrá un único atributo `message` de tipo `String`. Además, agregamos un constructor que reciba dicho parámetro, y un getter.
```java
package com.example.model;

public class Greeting {

    private String message;
    
    public Greeting(String message) {
        this.message = message;
    }
    public String getMessage() {
        return message;
    }
}
```

## 3. Crear un Servlet
El siguiente paso será crear un servlet para recibir un request HTTP GET. Primero, crear un nuevo paquete `com.example.servlet` (también dentro del directorio `src`), y dentro de este paquete crear una clase Java llamada `HelloWorldServlet`.
Para facilitar el desarrollo, agregar los siguientes imports a la clase:
```java
import com.example.model.Greeting;
import javax.servlet.*;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.*;
import java.io.IOException;
```
Luego debemos extender esta clase de `HttpServlet`, y overridear el metodo `doGet`
```java
public class HelloWorldServlet extends HttpServlet {

    @Override
    public void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {

    }
}
````
Dentro del metodo `doGet`, agregar las siguientes lineas:
```java
Greeting greeting = new Greeting("Hello World!");
req.setAttribute("greeting", greeting);
RequestDispatcher view = req.getRequestDispatcher("view/hello-world.jsp");
view.forward(req, resp);
```
Esto crea un objeto `Greeting` con el mensaje **_Hello World!_**, y seteamos este objeto como un atributo dentro del request, con la clave **_greeting_**. Luego, obtenemos del request un `RequestDispatcher` que representa la vista a la cual enviaremos el request, y luego llamamos a `view.forward` para redirigir el request a la vista.

Por último, debemos configurar la URL sobre la que estará escuchando el Servlet. Para eso, agregamos la siguiente Annotation sobre la cabecera de la clase:
```java
@WebServlet(urlPatterns = "/hello")
public class HelloWorldServlet extends HttpServlet {
...
}
```
De esta forma, el Servlet quedará escuchando en la dirección `/hello` del servidor.

## 4. Crear la JSP
El último paso será crear la vista para poder mostrar nuestro mensaje. En el directorio `web`, crear un nuevo directorio `view`, y dentro de este directorio crear un archivo JSP con nombre `hello-world.jsp`. Este archivo simplemente debe contener lo siguiente:
```html
<html>
<head>
    <title>Hello</title>
</head>
<body>
    ${requestScope.greeting.message}
</body>
</html>
```
Lo que hacemos es obtener el atributo **_greeting_** que guardamos previamente en el request, y mostramos el mensaje del objeto `Greeting`.

La estructura del proyecto debería haber quedado de la siguiente forma:

<img src="./images/project-structure.png" alt="Project structure">

## 5. Ejecutar
Para ejecutar la aplicación, simplemente hacer click en **_Run->Run 'Tomcat 9.0.8'_** (ya debería estar creada la configuración de ejecución 'Tomcat 9.0.8', aunque la versión puede no ser exactamente la misma). Una vez que levantó la aplicación, en un explorador vamos a **_http://localhost:8080/hello_**, y deberíamos ver nuestro mensaje **Hello World!**

<img src="./images/hello-world.png" alt="Hello World">

