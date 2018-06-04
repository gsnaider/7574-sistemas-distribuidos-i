package com.example.form;

import com.example.dao.UserDao;

import javax.annotation.PreDestroy;
import javax.faces.bean.ApplicationScoped;
import javax.faces.bean.ManagedBean;

/**
 * Application Bean for holding the UserDao.
 * <p>It creates the UserDao when the application starts, and destroys it when the application ends.
 */
@ManagedBean(name = "daoBean")
@ApplicationScoped
public class DaoBean {

    private UserDao userDao = new UserDao();

    public UserDao getUserDao() {
        return this.userDao;
    }

    @PreDestroy
    public void tearUp() {
        userDao.close();
    }

}
