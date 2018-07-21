package ar.uba.fi.contabilidapp.upload.model;

import com.google.gson.Gson;

import javax.persistence.*;
import java.util.Arrays;
import java.util.Objects;

@Entity
@Table(name = "UPLOADS")
public class Upload {

    @Id
    @Column(name = "ID")
    private long id;

    // TODO Check if BLOB column ok.
    @Lob
    @Column(name = "CONGLOMERATE_FILE_DATA")
    private byte[] conglomerateFileData;


    public long getId() {
        return id;
    }

    public byte[] getConglomerateFileData() {
        return conglomerateFileData;
    }

    public void setConglomerateFileData(byte[] conglomerateFileData) {
        this.conglomerateFileData = conglomerateFileData;
    }

    @Override
    public String toString() {
        return new Gson().toJson(this);
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Upload upload = (Upload) o;
        return id == upload.id &&
                Arrays.equals(conglomerateFileData, upload.conglomerateFileData);
    }

    @Override
    public int hashCode() {

        int result = Objects.hash(id);
        result = 31 * result + Arrays.hashCode(conglomerateFileData);
        return result;
    }
}
