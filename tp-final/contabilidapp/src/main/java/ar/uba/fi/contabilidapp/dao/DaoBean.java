package ar.uba.fi.contabilidapp.dao;


import ar.uba.fi.contabilidapp.upload.model.Client;
import ar.uba.fi.contabilidapp.upload.model.InputFile;
import ar.uba.fi.contabilidapp.upload.model.Transaction;
import ar.uba.fi.contabilidapp.upload.model.Upload;

import javax.annotation.PreDestroy;
import javax.faces.bean.ApplicationScoped;
import javax.faces.bean.ManagedBean;
import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;

/**
 * Application Bean for holding the DAOs.
 * <p>It creates the EntityManager and DAOs when the application starts,
 * and closes the EntityManager when the application stops.
 */
@ManagedBean(name = "daoBean")
@ApplicationScoped
public class DaoBean {

    private final EntityManagerFactory entityManagerFactory;
    private final EntityManager entityManager;

    private final Dao<Client> clientDao;
    private final Dao<Transaction> transactionDao;
    private final Dao<InputFile> inputFileDao;
    private final Dao<Upload> uploadDao;

    public DaoBean() {
        entityManagerFactory = Persistence.createEntityManagerFactory("contabilidapp-unit");
        entityManager = entityManagerFactory.createEntityManager();

        clientDao = new ClientDao(entityManager);
        transactionDao = new TransactionDao(entityManager);
        inputFileDao = new InputFileDao(entityManager);
        uploadDao = new UploadDao(entityManager);
    }

    public Dao<Client> getClientDao() {
        return clientDao;
    }

    public Dao<Transaction> getTransactionDao() {
        return transactionDao;
    }

    public Dao<InputFile> getInputFileDao() {
        return inputFileDao;
    }

    public Dao<Upload> getUploadDao() {
        return uploadDao;
    }

    @PreDestroy
    public void tearUp() {
        entityManager.close();
        entityManagerFactory.close();
    }

}
