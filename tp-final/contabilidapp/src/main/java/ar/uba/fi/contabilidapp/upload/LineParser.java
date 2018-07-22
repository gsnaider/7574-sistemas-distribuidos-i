package ar.uba.fi.contabilidapp.upload;

import ar.uba.fi.contabilidapp.upload.model.Client;
import ar.uba.fi.contabilidapp.upload.model.Transaction;

import java.math.BigDecimal;
import java.math.RoundingMode;

public class LineParser {

    private static final int CLIENT_CODE_POSITION = 6;
    private static final int CLIENT_CODE_LENGTH = 9;
    private static final int CLIENT_CODE_END_POSITION = CLIENT_CODE_POSITION + CLIENT_CODE_LENGTH;

    private static final int AMOUNT_POSITION = 23;
    static final int AMOUNT_LENGTH = 11;
    private static final int AMOUNT_END_POSITION = AMOUNT_POSITION + AMOUNT_LENGTH;

    /**
     * Parses a line from an input file to a {@link Transaction}.
     * @param inputLine The line to be parsed. Should have a valid transaction format. A valid format consists of a line
     *                  that matches the following regex: "^.{23}\d{11}.{47}$".
     * @return A new transaction with the amount and client information from the inputLine.
     * @throws IllegalArgumentException if inputLine does not have a valid transaction format.
     */
    public static Transaction parseLine(String inputLine) throws IllegalArgumentException {
        if (!LineValidator.isValid(inputLine)) {
            throw new IllegalArgumentException(String.format("Invalid line format: '%s'", inputLine));
        }
        String prefixCode = inputLine.substring(0, CLIENT_CODE_POSITION);
        String clientCode = inputLine.substring(CLIENT_CODE_POSITION, CLIENT_CODE_END_POSITION);
        String middleCode = inputLine.substring(CLIENT_CODE_END_POSITION, AMOUNT_POSITION);

        String amountString = inputLine.substring(AMOUNT_POSITION, AMOUNT_END_POSITION);
        BigDecimal amount = new BigDecimal(amountString);
        amount = amount.divide(new BigDecimal("100")).setScale(2, RoundingMode.CEILING);

        String suffixCode = inputLine.substring(AMOUNT_END_POSITION);

        Transaction transaction = new Transaction();
        Client client = new Client();
        client.setClientCode(clientCode);
        client.setPrefixCode(prefixCode);
        client.setMiddleCode(middleCode);
        client.setSuffixCode(suffixCode);
        transaction.setClient(client);
        transaction.setAmount(amount);
        return transaction;
    }

}
