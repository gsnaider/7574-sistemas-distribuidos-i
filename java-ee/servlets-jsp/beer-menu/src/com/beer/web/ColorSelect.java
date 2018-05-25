package com.beer.web;

import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import java.io.IOException;

@WebServlet(urlPatterns = "/ColorSelect.do")
public class ColorSelect extends HttpServlet {

    public void doPost(HttpServletRequest request, HttpServletResponse response) throws IOException, ServletException {

        String color = request.getParameter("color");

        HttpSession session = request.getSession();

        session.setAttribute("color", color);

        response.sendRedirect("view/intensity-form.html");

    }

}
