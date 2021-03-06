package ar.uba.fi.contabilidapp.entities;

import javax.persistence.*;
import java.util.Arrays;
import java.util.List;
import java.util.Objects;

/**
 * Entity representing an input file of {@link Transaction} records.
 *
 * <p>Each InputFile contains one or more {@link Transaction} records, and it belongs to a specific
 * {@link UploadPeriod}.
 *
 * <p>The same {@link Client} shouldn't appear more than once in the {@link Transaction} records form an InputFile.
 */
@Entity
@Table(name = "INPUT_FILES")
public class InputFile {

    private static final int MAX_BYTES = 100000;

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "ID")
    private long id;

    @ManyToOne(cascade = CascadeType.ALL)
    @JoinColumn(name = "UPLOAD_PERIOD_ID")
    private UploadPeriod uploadPeriod;

    @OneToMany(cascade = CascadeType.ALL)
    private List<Transaction> transactions;

    @Lob
    @Column(name = "FILE_DATA", length = MAX_BYTES)
    private byte[] fileData;

    public long getId() {
        return id;
    }

    public UploadPeriod getUploadPeriod() {
        return uploadPeriod;
    }

    public void setUploadPeriod(UploadPeriod uploadPeriod) {
        this.uploadPeriod = uploadPeriod;
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
        return "InputFile{" +
                "id=" + id +
                ", model=" + uploadPeriod +
                ", transactions=" + transactions +
                ", fileData=" + Arrays.toString(fileData) +
                '}';
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        InputFile inputFile = (InputFile) o;
        return id == inputFile.id &&
                Objects.equals(uploadPeriod, inputFile.uploadPeriod) &&
                Objects.equals(transactions, inputFile.transactions) &&
                Arrays.equals(fileData, inputFile.fileData);
    }

    @Override
    public int hashCode() {

        int result = Objects.hash(id, uploadPeriod, transactions);
        result = 31 * result + Arrays.hashCode(fileData);
        return result;
    }
}
