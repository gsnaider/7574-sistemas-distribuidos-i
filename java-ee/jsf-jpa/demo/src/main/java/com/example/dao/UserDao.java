package com.example.dao;

import com.example.model.User;

import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;
import javax.persistence.Query;
import java.util.List;

/**
 * Class for doing User queries on the database.
 */
public class UserDao {

    private final EntityManagerFactory entityManagerFactory;
    private final EntityManager em;

    public UserDao() {
        // Referenciamos a la unit declarada en persistence.xml
        entityManagerFactory = Persistence.createEntityManagerFactory("demo-unit");
        em = entityManagerFactory.createEntityManager();
    }

    public void add(User user) {
        em.getTransaction().begin();
        em.persist(user);
        em.getTransaction().commit();
    }

    public User findById(long id) {
        return em.find(User.class, id);
    }

    public User findByUsername(String username) {
        Query query = em.createNamedQuery("User.findByUsername");
        query.setParameter("username", username);
        return (User) query.getSingleResult();
    }


    public List<User> findAll() {
        Query query = em.createNamedQuery("User.findAll");
        return query.getResultList();
    }


    public void close() {
        System.out.println("Destroying dao...");
        em.close();
        entityManagerFactory.close();
    }

}
