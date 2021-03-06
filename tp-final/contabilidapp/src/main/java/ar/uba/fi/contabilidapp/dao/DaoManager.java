package ar.uba.fi.contabilidapp.dao;


import ar.uba.fi.contabilidapp.entities.Transaction;
import org.pmw.tinylog.Logger;

import javax.annotation.PreDestroy;
import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;
import javax.persistence.Query;
import java.util.List;

// TODO add index by CLIENT CODE
// TODO add error checking in all DB operations.

/**
 * Class for initializing and providing access to all the DAO entities from the app.
 * Also contains general database operations involving multiple entities.
 */
public class DaoManager {

    private final EntityManagerFactory entityManagerFactory;

    private final ClientDao clientDao;
    private final TransactionDao transactionDao;
    private final InputFileDao inputFileDao;
    private final UploadDao uploadDao;

    public DaoManager() {
        entityManagerFactory = Persistence.createEntityManagerFactory("contabilidapp-unit");
        clientDao = new ClientDao(entityManagerFactory);
        transactionDao = new TransactionDao(entityManagerFactory);
        inputFileDao = new InputFileDao(entityManagerFactory);
        uploadDao = new UploadDao(entityManagerFactory);
    }

    public List<Object[]> getAggregatedInputData(long uploadId) {
        EntityManager entityManager = entityManagerFactory.createEntityManager();
        String queryStrng =
                "SELECT c, SUM(t.amount) " +
                        "FROM Transaction t " +
                        "INNER JOIN t.client c " +
                        "WHERE t.inputFile.uploadPeriod.id = :uploadId " +
                        "GROUP BY c.id";
        Query query = entityManager.createQuery(
                queryStrng)
                .setParameter("uploadId", uploadId);
        List<Object[]> result = null;
        try {
            result = query.getResultList();
        } catch (Exception e) {
            Logger.error("Error getting aggregated data", e);
        } finally {
            entityManager.close();
        }

        return result;
    }

    public List<Transaction> getTransactionsFromPeriod(long uploadId) {
        EntityManager entityManager = entityManagerFactory.createEntityManager();
        String queryStrng =
                "SELECT t " +
                        "FROM Transaction t " +
                        "INNER JOIN t.inputFile f " +
                        "WHERE f.uploadPeriod.id = :uploadId " +
                        "ORDER BY f.id, t.id";
        Query query = entityManager.createQuery(
                queryStrng)
                .setParameter("uploadId", uploadId);
        List<Transaction> result = null;
        try {
            result = query.getResultList();
        } catch (Exception e) {
            Logger.error("Error getting transactions from period.", e);
        } finally {
            entityManager.close();
        }

        return result;

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
