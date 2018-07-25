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
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "ID")
    private long id;

    @Column(name = "OPEN")
    private boolean open;

    public long getId() {
        return id;
    }

    public boolean isOpen() {
        return open;
    }

    public void setOpen(boolean open) {
        this.open = open;
    }

    @Override
    public String toString() {
        return "UploadPeriod{" +
                "id=" + id +
                "open=" + open +
                '}';
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        UploadPeriod uploadPeriod = (UploadPeriod) o;
        return id == uploadPeriod.id && open == uploadPeriod.open;
    }

    @Override
    public int hashCode() {

        return Objects.hash(id, open);
    }
}
