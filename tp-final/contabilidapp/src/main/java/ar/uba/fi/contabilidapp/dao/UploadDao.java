package ar.uba.fi.contabilidapp.dao;

import ar.uba.fi.contabilidapp.entities.Client;
import ar.uba.fi.contabilidapp.entities.UploadPeriod;
import org.pmw.tinylog.Logger;

import javax.persistence.*;
import java.util.ArrayList;
import java.util.List;

public final class UploadDao extends AbstractDao<UploadPeriod> {

    UploadDao(EntityManagerFactory entityManagerFactory) {
        super(entityManagerFactory);
    }

    @Override
    public UploadPeriod find(long id) {
        return find(id, UploadPeriod.class);
    }

    public List<Long> findOpenUploadPeriodsIds() {
        Logger.info("Searching open periods");
        EntityManager entityManager = entityManagerFactory.createEntityManager();
        Query query = entityManager.createNamedQuery("UploadPeriods.findOpenPeriodsIds");
        try {
            return query.getResultList();
        } catch (Exception e) {
            Logger.error("Error searching open periods", e);
        } finally {
            entityManager.close();
        }
        return new ArrayList<>();
    }
}
