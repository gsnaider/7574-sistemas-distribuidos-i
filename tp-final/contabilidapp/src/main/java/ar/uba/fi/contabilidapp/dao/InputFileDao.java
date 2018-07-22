package ar.uba.fi.contabilidapp.dao;

import ar.uba.fi.contabilidapp.upload.model.InputFile;

import javax.persistence.EntityManagerFactory;

public final class InputFileDao extends AbstractDao<InputFile> {

    InputFileDao(EntityManagerFactory entityManagerFactory) {
        super(entityManagerFactory);
    }

    @Override
    public InputFile find(long id) {
        return find(id, InputFile.class);
    }
}
