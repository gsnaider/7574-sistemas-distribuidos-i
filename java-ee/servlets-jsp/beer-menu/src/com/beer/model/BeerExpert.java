package com.beer.model;

public class BeerExpert {

    public Beer getBeer(String color, String intensity) {
        if (color.equals("rubia")) {
            if (intensity.equals("suave")) {
                return new Beer("Quilmes Lager", 80);
            } else {
                return new Beer("Stella Artois", 120);
            }
        } else if (color.equals("roja")){
           return new Beer("Quilmes Red Lager", 85);

        } else {
            if (intensity.equals("fuerte")) {
                return new Beer("Guinness", 150);
            } else {
                return new Beer("Quilmes Stout", 90);
            }
        }
    }

}
