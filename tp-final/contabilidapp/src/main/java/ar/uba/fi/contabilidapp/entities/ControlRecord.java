package ar.uba.fi.contabilidapp.entities;

import java.math.BigDecimal;
import java.util.Objects;

public class ControlRecord {

    private String clientCode;

    private String clientName;

    private String middleCode;

    private BigDecimal amount;

    private String suffixCode;

    public ControlRecord(String clientCode, String clientName, String middleCode, BigDecimal amount, String suffixCode) {
        this.clientCode = clientCode;
        this.clientName = clientName;
        this.middleCode = middleCode;
        this.amount = amount;
        this.suffixCode = suffixCode;
    }

    public String getClientCode() {
        return clientCode;
    }

    public void setClientCode(String clientCode) {
        this.clientCode = clientCode;
    }

    public String getClientName() {
        return clientName;
    }

    public void setClientName(String clientName) {
        this.clientName = clientName;
    }

    public String getMiddleCode() {
        return middleCode;
    }

    public void setMiddleCode(String middleCode) {
        this.middleCode = middleCode;
    }

    public BigDecimal getAmount() {
        return amount;
    }

    public void setAmount(BigDecimal amount) {
        this.amount = amount;
    }

    public String getSuffixCode() {
        return suffixCode;
    }

    public void setSuffixCode(String suffixCode) {
        this.suffixCode = suffixCode;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        ControlRecord that = (ControlRecord) o;
        return Objects.equals(clientCode, that.clientCode) &&
                Objects.equals(clientName, that.clientName) &&
                Objects.equals(middleCode, that.middleCode) &&
                Objects.equals(amount, that.amount) &&
                Objects.equals(suffixCode, that.suffixCode);
    }

    @Override
    public int hashCode() {

        return Objects.hash(clientCode, clientName, middleCode, amount, suffixCode);
    }
}
