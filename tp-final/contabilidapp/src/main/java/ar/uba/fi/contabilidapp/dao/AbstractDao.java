package ar.uba.fi.contabilidapp.dao;

import org.pmw.tinylog.Logger;

import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.PersistenceContext;

abstract class AbstractDao<T> implements Dao<T> {

    private final EntityManagerFactory entityManagerFactory;

    AbstractDao(EntityManagerFactory entityManagerFactory) {
        this.entityManagerFactory = entityManagerFactory;
    }

    @Override
    public T add(T t) {
        Logger.info("Adding: '{}'", t.toString());
        EntityManager entityManager = entityManagerFactory.createEntityManager();
        entityManager.getTransaction().begin();
        entityManager.persist(t);
        entityManager.getTransaction().commit();
        entityManager.close();
        return t;
    }

    T find(long id, Class<T> entityClass) {
        Logger.info("Finding object of type '{}' with id '{}'", entityClass.getCanonicalName(), id);
        EntityManager entityManager = entityManagerFactory.createEntityManager();
        T found = entityManager.find(entityClass, id);
        entityManager.close();
        return found;
    }

    @Override
    public T update(T t) {
        Logger.info("Updating: '{}'", t.toString());
        EntityManager entityManager = entityManagerFactory.createEntityManager();
        entityManager.getTransaction().begin();
        t = entityManager.merge(t);
        entityManager.getTransaction().commit();
        entityManager.close();
        return t;
    }

    @Override
    public void delete(T t) {
        Logger.info("Deleting: '{}'", t.toString());
        EntityManager entityManager = entityManagerFactory.createEntityManager();
        entityManager.getTransaction().begin();
        entityManager.remove(t);
        entityManager.getTransaction().commit();
        entityManager.close();
    }
}
