package ar.uba.fi.contabilidapp.dao;

import ar.uba.fi.contabilidapp.upload.model.Client;

import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;

// TODO
public class ClientDao {

    private final EntityManagerFactory entityManagerFactory;
    private final EntityManager em;

    public ClientDao() {
        entityManagerFactory = Persistence.createEntityManagerFactory("demo-unit");
        em = entityManagerFactory.createEntityManager();
    }

    public void add(Client client) {
        em.getTransaction().begin();
        em.persist(client);
        em.getTransaction().commit();
    }

}
