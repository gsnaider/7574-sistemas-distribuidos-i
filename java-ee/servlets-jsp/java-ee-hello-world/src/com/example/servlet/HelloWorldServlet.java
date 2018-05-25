package com.example.servlet;


import com.example.model.Greeting;
import javax.servlet.*;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.*;
import java.io.IOException;

@WebServlet(urlPatterns = "/hello")
public class HelloWorldServlet extends HttpServlet {

    @Override
    public void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        Greeting greeting = new Greeting("Hello World!");

        req.setAttribute("greeting", greeting);

        RequestDispatcher view = req.getRequestDispatcher("view/hello-world.jsp");
        view.forward(req, resp);
    }
}
