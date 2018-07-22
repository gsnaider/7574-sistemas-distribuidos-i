package ar.uba.fi.contabilidapp.dao;

import ar.uba.fi.contabilidapp.upload.model.Client;
import org.pmw.tinylog.Logger;

import javax.persistence.*;


public final class ClientDao extends AbstractDao<Client> {

    ClientDao(EntityManagerFactory entityManagerFactory) {
        super(entityManagerFactory);
    }

    @Override
    public Client find(long id) {
        return find(id, Client.class);
    }

    public Client findByClientCode(String clientCode) {
        Logger.info("Searching client with clientCode {}", clientCode);
        EntityManager entityManager = entityManagerFactory.createEntityManager();
        Query query = entityManager.createNamedQuery("Clients.findByClientCode");
        query.setParameter("clientCode", clientCode);
        try {
            Client client = (Client) query.getSingleResult();
            Logger.info("Client found for clientCode {} : {}", clientCode, client);
            return client;
        } catch (NoResultException e) {
            Logger.info("No client found for clientCode {}", clientCode);
            return null;
        } catch (NonUniqueResultException e) {
            Logger.warn("Multiple clients found for clientCode {}", clientCode);
            return null;
        } finally {
            entityManager.close();
        }

    }


}
