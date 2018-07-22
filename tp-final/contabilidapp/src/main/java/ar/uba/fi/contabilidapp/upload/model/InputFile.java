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
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "ID")
    private long id;

    @ManyToOne(cascade = CascadeType.ALL)
    private Upload upload;

    @OneToMany(cascade = CascadeType.ALL)
    private List<Transaction> transactions;

    // TODO Check if BLOB column ok.
    @Lob
    @Column(name = "FILE_DATA", length=MAX_BYTES)
    private byte[] fileData;

    public long getId() {
        return id;
    }

    public Upload getUpload() {
        return upload;
    }

    public void setUpload(Upload upload) {
        this.upload = upload;
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
                Objects.equals(upload, inputFile.upload) &&
                Objects.equals(transactions, inputFile.transactions) &&
                Arrays.equals(fileData, inputFile.fileData);
    }

    @Override
    public int hashCode() {

        int result = Objects.hash(id, upload, transactions);
        result = 31 * result + Arrays.hashCode(fileData);
        return result;
    }
}
