package ar.uba.fi.contabilidapp.dao;

import ar.uba.fi.contabilidapp.upload.model.Client;

import javax.persistence.EntityManagerFactory;


final class ClientDao extends AbstractDao<Client> {

    ClientDao(EntityManagerFactory entityManagerFactory) {
        super(entityManagerFactory);
    }

    @Override
    public Client find(long id) {
        return find(id, Client.class);
    }

}
