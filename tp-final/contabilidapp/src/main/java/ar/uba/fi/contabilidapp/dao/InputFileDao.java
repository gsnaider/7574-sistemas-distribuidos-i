package ar.uba.fi.contabilidapp.dao;

import ar.uba.fi.contabilidapp.upload.model.InputFile;

import javax.persistence.EntityManager;

public class InputFileDao extends AbstractDao<InputFile> {

    InputFileDao(EntityManager entityManager) {
        super(entityManager);
    }

    @Override
    public InputFile find(long id) {
        return find(id, InputFile.class);
    }
}
