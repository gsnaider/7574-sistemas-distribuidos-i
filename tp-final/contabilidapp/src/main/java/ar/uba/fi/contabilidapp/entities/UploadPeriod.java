package ar.uba.fi.contabilidapp.entities;

import javax.persistence.*;
import java.util.Arrays;
import java.util.Objects;

@Entity
@Table(name = "UPLOAD_PERIODS")
@NamedQueries(value = {
        @NamedQuery(name = "UploadPeriods.findPeriodsIdsByState", query = "SELECT p.id FROM UploadPeriod p WHERE p.open = :open")
})
public class UploadPeriod {

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE)
    @Column(name = "ID")
    private long id;

    @Column(name = "OPEN")
    private boolean open;

    @Lob
    @Column(name = "CONGLOMERATE_FILE_DATA")
    private byte[] conglomerateFileData;

    public long getId() {
        return id;
    }

    public boolean isOpen() {
        return open;
    }

    public void setOpen(boolean open) {
        this.open = open;
    }

    public byte[] getConglomerateFileData() {
        return conglomerateFileData;
    }

    public void setConglomerateFileData(byte[] conglomerateFileData) {
        this.conglomerateFileData = conglomerateFileData;
    }

    @Override
    public String toString() {
        return "UploadPeriod{" +
                "id=" + id +
                "open=" + open +
                ", conglomerateFileData=" + Arrays.toString(conglomerateFileData) +
                '}';
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        UploadPeriod uploadPeriod = (UploadPeriod) o;
        return id == uploadPeriod.id && open == uploadPeriod.open &&
                Arrays.equals(conglomerateFileData, uploadPeriod.conglomerateFileData);
    }

    @Override
    public int hashCode() {

        int result = Objects.hash(id);
        result = 31 * result + Arrays.hashCode(conglomerateFileData);
        return result;
    }
}
