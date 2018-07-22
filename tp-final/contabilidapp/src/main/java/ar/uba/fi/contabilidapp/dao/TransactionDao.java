package ar.uba.fi.contabilidapp.dao;

import ar.uba.fi.contabilidapp.upload.model.Transaction;

import javax.persistence.EntityManagerFactory;

class TransactionDao extends AbstractDao<Transaction> {

    TransactionDao(EntityManagerFactory entityManagerFactory) {
        super(entityManagerFactory);
    }

    @Override
    public Transaction find(long id) {
        return find(id, Transaction.class);
    }
}
