package ar.uba.fi.contabilidapp.dao;

import ar.uba.fi.contabilidapp.upload.model.Upload;

import javax.persistence.EntityManager;

public class UploadDao extends AbstractDao<Upload> {

    UploadDao(EntityManager entityManager) {
        super(entityManager);
    }

    @Override
    public Upload find(long id) {
        return find(id, Upload.class);
    }
}
