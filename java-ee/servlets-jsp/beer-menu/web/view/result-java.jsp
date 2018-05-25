<%@ page import="com.beer.model.Beer" %>

<html>
<body>
<h2>Cerveza recomendada</h2>
<%
        Beer beer = (Beer) request.getAttribute("beer");
        out.println("<br>Marca: " + beer.getBrand() + "<br>Precio: $" + beer.getPrice());
    %>
<p>
    <a href="/index.html">Volver</a>
</body>
</html>