package ar.uba.fi.contabilidapp.dao;

import ar.uba.fi.contabilidapp.upload.model.Transaction;

import javax.persistence.EntityManager;

class TransactionDao extends AbstractDao<Transaction> {

    TransactionDao(EntityManager entityManager) {
        super(entityManager);
    }

    @Override
    public Transaction find(long id) {
        return find(id, Transaction.class);
    }
}
