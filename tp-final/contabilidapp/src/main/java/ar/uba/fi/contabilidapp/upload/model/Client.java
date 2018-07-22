package ar.uba.fi.contabilidapp.upload.model;

import com.google.gson.Gson;

import javax.persistence.*;
import java.util.List;
import java.util.Objects;

@Entity
@Table(name = "CLIENTS")
public class Client {

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    @Column(name = "ID")
    private long id;

    @Column(name = "CLIENT_CODE")
    private String clientCode;

    @OneToMany(cascade = CascadeType.ALL)
    private List<Transaction> transactions;

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

    public List<Transaction> getTransactions() {
        return transactions;
    }

    public void setTransactions(List<Transaction> transactions) {
        this.transactions = transactions;
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
                ", transactions=" + transactions +
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
                Objects.equals(transactions, client.transactions) &&
                Objects.equals(prefixCode, client.prefixCode) &&
                Objects.equals(middleCode, client.middleCode) &&
                Objects.equals(suffixCode, client.suffixCode);
    }

    @Override
    public int hashCode() {
        return Objects.hash(id, clientCode, transactions, prefixCode, middleCode, suffixCode);
    }
}
