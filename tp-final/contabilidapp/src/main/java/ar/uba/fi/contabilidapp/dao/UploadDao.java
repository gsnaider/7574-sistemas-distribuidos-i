package ar.uba.fi.contabilidapp.dao;

import ar.uba.fi.contabilidapp.entities.UploadPeriod;
import ar.uba.fi.contabilidapp.model.ContabilidappException;
import org.pmw.tinylog.Logger;

import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Query;
import java.util.ArrayList;
import java.util.List;

/**
 * Class containing specific DB operations for the {@link UploadPeriod} entity.
 */
public final class UploadDao extends AbstractDao<UploadPeriod> {

    UploadDao(EntityManagerFactory entityManagerFactory) {
        super(entityManagerFactory);
    }

    @Override
    public UploadPeriod find(long id) {
        return find(id, UploadPeriod.class);
    }

    public List<Long> getOpenUploadPeriodsIds() {
        Logger.info("Searching open periods");
        EntityManager entityManager = entityManagerFactory.createEntityManager();
        Query query = entityManager.createNamedQuery("UploadPeriods.findPeriodsIdsByState").setParameter("open", true);
        try {
            return query.getResultList();
        } catch (Exception e) {
            Logger.error("Error searching open periods", e);
        } finally {
            entityManager.close();
        }
        return new ArrayList<>();
    }

    public List<Long> getClosedUploadPeriodsIds() {
        Logger.info("Searching closed periods");
        EntityManager entityManager = entityManagerFactory.createEntityManager();
        Query query = entityManager.createNamedQuery("UploadPeriods.findPeriodsIdsByState").setParameter("open", false);
        try {
            return query.getResultList();
        } catch (Exception e) {
            Logger.error("Error searching closed periods", e);
        } finally {
            entityManager.close();
        }
        return new ArrayList<>();
    }

    public void closePeriod(long uploadId) throws ContabilidappException {
        Logger.info("Closing period {}", uploadId);
        EntityManager entityManager = entityManagerFactory.createEntityManager();
        entityManager.getTransaction().begin();
        UploadPeriod period = entityManager.find(UploadPeriod.class, uploadId);
        if (period == null) {
            throw new ContabilidappException(String.format("Upload period not found: %d.", uploadId));
        }
        period.setOpen(false);
        entityManager.merge(period);
        try {
            entityManager.getTransaction().commit();
        } catch (Exception e) {
            throw new ContabilidappException(String.format("Error updating period: %d.", uploadId), e);
        } finally {
            entityManager.close();
        }

    }
}
