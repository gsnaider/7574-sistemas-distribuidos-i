package ar.uba.fi.contabilidapp.dao;

import ar.uba.fi.contabilidapp.entities.InputFile;
import ar.uba.fi.contabilidapp.entities.UploadPeriod;
import ar.uba.fi.contabilidapp.model.ContabilidappException;
import org.pmw.tinylog.Logger;

import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;

public final class InputFileDao extends AbstractDao<InputFile> {

    InputFileDao(EntityManagerFactory entityManagerFactory) {
        super(entityManagerFactory);
    }

    @Override
    public InputFile find(long id) {
        return find(id, InputFile.class);
    }

    public InputFile addInputFile(InputFile inputFile, long uploadId) throws ContabilidappException {
        Logger.info("Adding input file: '{}'", inputFile.toString());
        EntityManager entityManager = entityManagerFactory.createEntityManager();
        entityManager.getTransaction().begin();

        UploadPeriod period = entityManager.find(UploadPeriod.class, uploadId);
        if (!period.isOpen()) {
            throw new ContabilidappException("Trying to add file to closed period.");
        }
        inputFile.setUploadPeriod(period);

        entityManager.persist(inputFile);
        entityManager.getTransaction().commit();
        entityManager.close();
        return inputFile;
    }
}
