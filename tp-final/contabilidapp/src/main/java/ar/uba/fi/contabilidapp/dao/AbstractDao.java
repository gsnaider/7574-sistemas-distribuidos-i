package ar.uba.fi.contabilidapp.dao;

import org.pmw.tinylog.Logger;

import javax.persistence.EntityManager;

abstract class AbstractDao<T> implements Dao<T> {

    private final EntityManager entityManager;

    AbstractDao(EntityManager entityManager) {
        this.entityManager = entityManager;
    }

    // TODO check if we need to use transactions.

    @Override
    public T add(T t) {
        Logger.info("Adding: '{}'", t.toString());
        entityManager.getTransaction().begin();
        entityManager.persist(t);
        entityManager.getTransaction().commit();
        return t;
    }

    T find(long id, Class<T> entityClass) {
        Logger.info("Finding object of type '{}' with id '{}'", entityClass.getCanonicalName(), id);
        return entityManager.find(entityClass, id);
    }

    @Override
    public T update(T t) {
        Logger.info("Updating: '{}'", t.toString());
        entityManager.getTransaction().begin();
        t = entityManager.merge(t);
        entityManager.getTransaction().commit();
        return t;
    }

    @Override
    public void delete(T t) {
        Logger.info("Deleting: '{}'", t.toString());
        entityManager.getTransaction().begin();
        entityManager.remove(t);
        entityManager.getTransaction().commit();
    }
}
