package com.example.form;

import com.example.dao.UserDao;
import com.example.model.User;

import javax.annotation.PostConstruct;
import javax.faces.bean.ManagedBean;
import javax.faces.bean.ManagedProperty;
import java.util.List;

@ManagedBean
public class ListUsersView {

    // Usamos @ManagedProperty para obtener una referencia a la instancia de DaoBean
    @ManagedProperty(value = "#{daoBean}")
    private DaoBean daoBean;

    private UserDao userDao;

    // @PostConstruct se ejecuta luego del constructor de la clase y luego de que todas las dependecias hayan sido
    // inyectadas (en este caso se inyecta la referencia daoBean declarada arriba). Esto nos permite obtener el UserDao
    // del daoBean.
    @PostConstruct
    public void startUp() {
        userDao = daoBean.getUserDao();
    }

    public List<User> getUsers() {
        return userDao.findAll();
    }

    public void setDaoBean(DaoBean daoBean) {
        this.daoBean = daoBean;
    }
}
