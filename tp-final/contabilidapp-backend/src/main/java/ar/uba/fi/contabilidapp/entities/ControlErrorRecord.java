package ar.uba.fi.contabilidapp.entities;

import java.math.BigDecimal;
import java.util.Objects;

/**
 * Class used to store the data for each line on the errors file from a control.
 */
public class ControlErrorRecord {

    private String clientCode;

    private BigDecimal transactionAmount;

    private BigDecimal controlAmount;

    public ControlErrorRecord(String clientCode, BigDecimal transactionAmount, BigDecimal controlAmount) {
        this.clientCode = clientCode;
        this.transactionAmount = transactionAmount;
        this.controlAmount = controlAmount;
    }

    public String getClientCode() {
        return clientCode;
    }

    public void setClientCode(String clientCode) {
        this.clientCode = clientCode;
    }

    public BigDecimal getTransactionAmount() {
        return transactionAmount;
    }

    public void setTransactionAmount(BigDecimal transactionAmount) {
        this.transactionAmount = transactionAmount;
    }

    public BigDecimal getControlAmount() {
        return controlAmount;
    }

    public void setControlAmount(BigDecimal controlAmount) {
        this.controlAmount = controlAmount;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        ControlErrorRecord that = (ControlErrorRecord) o;
        return Objects.equals(clientCode, that.clientCode) &&
                Objects.equals(transactionAmount, that.transactionAmount) &&
                Objects.equals(controlAmount, that.controlAmount);
    }

    @Override
    public int hashCode() {

        return Objects.hash(clientCode, transactionAmount, controlAmount);
    }
}
