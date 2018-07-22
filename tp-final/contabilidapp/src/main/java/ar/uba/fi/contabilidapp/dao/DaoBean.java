package ar.uba.fi.contabilidapp.dao;


import ar.uba.fi.contabilidapp.upload.model.Client;
import ar.uba.fi.contabilidapp.upload.model.InputFile;
import ar.uba.fi.contabilidapp.upload.model.Transaction;
import ar.uba.fi.contabilidapp.upload.model.Upload;
import org.pmw.tinylog.Logger;

import javax.annotation.PreDestroy;
import javax.faces.bean.ApplicationScoped;
import javax.faces.bean.ManagedBean;
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

    private final ClientDao clientDao;
    private final TransactionDao transactionDao;
    private final InputFileDao inputFileDao;
    private final UploadDao uploadDao;

    public DaoBean() {
        entityManagerFactory = Persistence.createEntityManagerFactory("contabilidapp-unit");

        clientDao = new ClientDao(entityManagerFactory);
        transactionDao = new TransactionDao(entityManagerFactory);
        inputFileDao = new InputFileDao(entityManagerFactory);
        uploadDao = new UploadDao(entityManagerFactory);
    }

    public ClientDao getClientDao() {
        return clientDao;
    }

    public TransactionDao getTransactionDao() {
        return transactionDao;
    }

    public InputFileDao getInputFileDao() {
        return inputFileDao;
    }

    public UploadDao getUploadDao() {
        return uploadDao;
    }

    @PreDestroy
    public void tearUp() {
        Logger.info("Closing entity manager factory.");
        entityManagerFactory.close();
    }

}
