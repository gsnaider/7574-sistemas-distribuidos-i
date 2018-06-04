package com.example.form;

import com.example.dao.UserDao;
import com.example.model.Address;
import com.example.model.Phone;
import com.example.model.User;

import javax.annotation.PostConstruct;
import javax.faces.bean.ManagedBean;
import javax.faces.bean.ManagedProperty;
import java.util.ArrayList;
import java.util.List;

@ManagedBean
public class RegisterView {

    private User user;

    // Usamos @ManagedProperty para obtener una referencia a la instancia de DaoBean
    @ManagedProperty(value="#{daoBean}")
    private DaoBean daoBean;

    private UserDao userDao;

    // @PostConstruct se ejecuta luego del constructor de la clase y luego de que todas las dependecias hayan sido
    // inyectadas (en este caso se inyecta la referencia daoBean declarada arriba). Esto nos permite obtener el UserDao
    // del daoBean.
    // Ademas aprovechamos para crear el objeto User que vamos a llenar desde la vista (register.xhtml).
    @PostConstruct
    public void startUp() {
        user = new User();
        user.setAddress(new Address());

        List<Phone> phones = new ArrayList<>();
        phones.add(new Phone());
        phones.add(new Phone());
        user.setPhones(phones);

        userDao = daoBean.getUserDao();
    }

    /**
     * Adds user to DB, and redirects to index.xhtml
     */
    public String register() {
        userDao.add(user);
        System.out.println(String.format("User added: %s", user));
        return "/index?faces-redirect=true";
    }

    public User getUser() {
        return user;
    }

    public void setUser(User user) {
        this.user = user;
    }

    public void setDaoBean(DaoBean daoBean) {
        this.daoBean = daoBean;
    }
}
