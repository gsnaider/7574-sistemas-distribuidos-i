<html>
<body>
<h2>Atributos elegidos</h2>
    Color: ${sessionScope.color}
    <br>Intensidad: ${sessionScope.intensity}
<p>
<h2>Cerveza recomendada</h2>
    Marca: ${requestScope.beer.brand}
    <br>Precio: $${requestScope.beer.price}
    <br><br>
    <a href="/index.html">Volver</a>
</body>
</html>