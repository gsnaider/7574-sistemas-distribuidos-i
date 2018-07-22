package ar.uba.fi.contabilidapp.dao;

import ar.uba.fi.contabilidapp.upload.model.Client;

import javax.persistence.EntityManager;


final class ClientDao extends AbstractDao<Client> {

    ClientDao(EntityManager entityManager) {
        super(entityManager);
    }

    @Override
    public Client find(long id) {
        return find(id, Client.class);
    }

}
