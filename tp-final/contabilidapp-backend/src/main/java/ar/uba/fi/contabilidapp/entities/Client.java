package ar.uba.fi.contabilidapp.entities;

import javax.persistence.*;
import java.util.Objects;

/**
 * Entity that represents a client from a {@link Transaction}.
 */
@Entity
@Table(name = "CLIENTS")
@NamedQueries(value = {
        @NamedQuery(name = "Clients.findByClientCode", query = "SELECT c FROM Client c WHERE c.clientCode = :clientCode")
})
public class Client {

    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    @Column(name = "ID")
    private long id;

    @Column(name = "CLIENT_CODE")
    private String clientCode;

    @Column(name = "PREFIX_CODE")
    private String prefixCode;

    @Column(name = "MIDDLE_CODE")
    private String middleCode;

    @Column(name = "SUFFIX_CODE")
    private String suffixCode;

    public long getId() {
        return id;
    }

    public String getClientCode() {
        return clientCode;
    }

    public void setClientCode(String clientCode) {
        this.clientCode = clientCode;
    }

    public String getPrefixCode() {
        return prefixCode;
    }

    public void setPrefixCode(String prefixCode) {
        this.prefixCode = prefixCode;
    }

    public String getMiddleCode() {
        return middleCode;
    }

    public void setMiddleCode(String middleCode) {
        this.middleCode = middleCode;
    }

    public String getSuffixCode() {
        return suffixCode;
    }

    public void setSuffixCode(String suffixCode) {
        this.suffixCode = suffixCode;
    }

    @Override
    public String toString() {
        return "Client{" +
                "id=" + id +
                ", clientCode='" + clientCode + '\'' +
                ", prefixCode='" + prefixCode + '\'' +
                ", middleCode='" + middleCode + '\'' +
                ", suffixCode='" + suffixCode + '\'' +
                '}';
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Client client = (Client) o;
        return id == client.id &&
                Objects.equals(clientCode, client.clientCode) &&
                Objects.equals(prefixCode, client.prefixCode) &&
                Objects.equals(middleCode, client.middleCode) &&
                Objects.equals(suffixCode, client.suffixCode);
    }

    @Override
    public int hashCode() {
        return Objects.hash(id, clientCode, prefixCode, middleCode, suffixCode);
    }
}
