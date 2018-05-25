package com.beer.web;

import com.beer.model.Beer;
import com.beer.model.BeerExpert;

import javax.servlet.RequestDispatcher;
import javax.servlet.ServletException;
import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import java.io.IOException;

@WebServlet(urlPatterns = "/IntensitySelect.do")
public class IntensitySelect extends HttpServlet {

    public void doPost(HttpServletRequest request, HttpServletResponse response) throws IOException, ServletException {

        HttpSession session = request.getSession();
        String color = (String) session.getAttribute("color");

        String intensity = request.getParameter("intensity");
        session.setAttribute("intensity", intensity);

        BeerExpert beerExpert = new BeerExpert();
        Beer beer = beerExpert.getBeer(color, intensity);

        request.setAttribute("beer", beer);

        RequestDispatcher view = request.getRequestDispatcher("view/result-EL.jsp");
        view.forward(request, response);
    }

}
