package com.beer.model;

import java.util.Objects;

public class Beer {

    private String brand;
    private int price;

    public Beer(String brand, int price) {
        this.brand = brand;
        this.price = price;
    }

    public String getBrand() {
        return brand;
    }

    public void setBrand(String brand) {
        this.brand = brand;
    }

    public int getPrice() {
        return price;
    }

    public void setPrice(int price) {
        this.price = price;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Beer beer = (Beer) o;
        return Objects.equals(brand, beer.brand) &&
                Objects.equals(price, beer.price);
    }

    @Override
    public int hashCode() {
        return Objects.hash(brand, price);
    }

    @Override
    public String toString() {
        return "Beer{" +
                "brand='" + brand + '\'' +
                ", price=" + price +
                '}';
    }
}
