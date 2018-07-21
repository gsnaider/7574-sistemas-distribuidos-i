package ar.uba.fi.contabilidapp.upload.model;

import com.google.gson.Gson;

import javax.persistence.*;
import java.util.Arrays;
import java.util.List;
import java.util.Objects;

@Entity
@Table(name = "INPUT_FILES")
public class InputFile {

    private static final int MAX_BYTES = 100000;

    @Id
    @Column(name = "ID")
    private long id;

    @Column(name = "UPLOAD_ID")
    private long uploadId;

    @OneToMany(cascade = CascadeType.ALL)
    private List<Transaction> transactions;

    // TODO Check if BLOB column ok.
    @Lob
    @Column(name = "FILE_DATA", length=MAX_BYTES)
    private byte[] fileData;

    public long getId() {
        return id;
    }

    public long getUploadId() {
        return uploadId;
    }

    public void setUploadId(long uploadId) {
        this.uploadId = uploadId;
    }

    public List<Transaction> getTransactions() {
        return transactions;
    }

    public void setTransactions(List<Transaction> transactions) {
        this.transactions = transactions;
    }

    public byte[] getFileData() {
        return fileData;
    }

    public void setFileData(byte[] fileData) {
        this.fileData = fileData;
    }

    @Override
    public String toString() {
        return new Gson().toJson(this);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        InputFile inputFile = (InputFile) o;
        return id == inputFile.id &&
                uploadId == inputFile.uploadId &&
                Objects.equals(transactions, inputFile.transactions) &&
                Arrays.equals(fileData, inputFile.fileData);
    }

    @Override
    public int hashCode() {

        int result = Objects.hash(id, uploadId, transactions);
        result = 31 * result + Arrays.hashCode(fileData);
        return result;
    }

}
