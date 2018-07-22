package ar.uba.fi.contabilidapp.dao;

public interface Dao<T> {

    T add(T t);

    T find(long id);

    T update(T t);

    void delete(T t);

}
