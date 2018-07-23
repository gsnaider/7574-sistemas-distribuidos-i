package ar.uba.fi.contabilidapp.dao;

import ar.uba.fi.contabilidapp.entities.Upload;

import javax.persistence.EntityManagerFactory;

public final class UploadDao extends AbstractDao<Upload> {

    UploadDao(EntityManagerFactory entityManagerFactory) {
        super(entityManagerFactory);
    }

    @Override
    public Upload find(long id) {
        return find(id, Upload.class);
    }
}
