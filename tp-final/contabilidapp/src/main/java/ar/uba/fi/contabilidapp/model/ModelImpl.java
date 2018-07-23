package ar.uba.fi.contabilidapp.model;

import ar.uba.fi.contabilidapp.dao.DaoManager;
import ar.uba.fi.contabilidapp.entities.Client;
import ar.uba.fi.contabilidapp.entities.InputFile;
import ar.uba.fi.contabilidapp.entities.Transaction;
import ar.uba.fi.contabilidapp.entities.UploadPeriod;

import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.math.BigDecimal;
import java.math.RoundingMode;
import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.Formatter;
import java.util.List;
import java.util.Scanner;
import java.util.regex.Pattern;

public class ModelImpl implements Model {

    private static final int CLIENT_CODE_POSITION = 6;
    private static final int CLIENT_CODE_LENGTH = 9;
    private static final int CLIENT_CODE_END_POSITION = CLIENT_CODE_POSITION + CLIENT_CODE_LENGTH;

    private static final int AMOUNT_POSITION = 23;
    static final int AMOUNT_LENGTH = 11;
    private static final int AMOUNT_END_POSITION = AMOUNT_POSITION + AMOUNT_LENGTH;

    private static final String LINE_REGEX = String.format("^.{23}\\d{%d}.{47}$", AMOUNT_LENGTH);
    private static final Pattern PATTERN = Pattern.compile(LINE_REGEX);

    private final DaoManager daoManager;

    public ModelImpl(DaoManager daoManager) {
        this.daoManager = daoManager;
    }

    @Override
    public long startUploadPeriod() {
        UploadPeriod uploadPeriod = new UploadPeriod();
        uploadPeriod.setOpen(true);
        uploadPeriod = daoManager.getUploadDao().add(uploadPeriod);
        return uploadPeriod.getId();
    }

    @Override
    public void handleFileUpload(byte[] fileData, long uploadId) throws ContabilidappException {
        List<Transaction> transactions = new ArrayList<>();
        InputStream is = new ByteArrayInputStream(fileData);
        Scanner scanner = new Scanner(is);
        while (scanner.hasNextLine()) {
            Transaction transaction = parseLine(scanner.nextLine());
            transactions.add(transaction);
        }

        InputFile inputFile = persistInputFile(fileData, transactions, uploadId);
    }

    @Override
    public String getAggregatedDataFile(long uploadId) {

        List<Object[]> aggregatedDataList = daoManager.getAggregatedInputData(uploadId);
        StringBuilder aggregatedTable = new StringBuilder();
        Formatter lineFormatter = new Formatter(aggregatedTable);
        for (Object[] tuple : aggregatedDataList) {
            Client client = (Client) tuple[0];
            BigDecimal amount = (BigDecimal) tuple[1];
            amount = amount.multiply(new BigDecimal("100"));
            DecimalFormat df = new DecimalFormat("00000000000");
            String formattedAmount = df.format(amount);

            lineFormatter.format(
                    "%s%s%s%s%s\n",
                    client.getPrefixCode(),
                    client.getClientCode(),
                    client.getMiddleCode(),
                    formattedAmount,
                    client.getSuffixCode());
        }

        return aggregatedTable.toString();
    }

    @Override
    public List<Long> getOpenUploadPeriodsIds() {
        return daoManager.getUploadDao().findOpenUploadPeriodsIds();
    }

    private InputFile persistInputFile(byte[] fileData, List<Transaction> transactions, long uploadId) throws ContabilidappException {
        InputFile inputFile = new InputFile();
        inputFile.setFileData(fileData);
        for (Transaction transaction : transactions) {
            Client client = daoManager.getClientDao().addIfNotPresent(transaction.getClient());
            transaction.setClient(client);
            transaction.setInputFile(inputFile);
        }
        inputFile.setTransactions(transactions);

        UploadPeriod period = daoManager.getUploadDao().find(uploadId);
        if (!period.isOpen()) {
            throw new ContabilidappException("Trying to add file to closed period.");
        }
        inputFile.setUploadPeriod(period);

        UploadPeriod uploadPeriod = new UploadPeriod();
        inputFile.setUploadPeriod(uploadPeriod);

        return daoManager.getInputFileDao().add(inputFile);
    }


    /**
     * Parses a line from an input file to a {@link Transaction}.
     *
     * @param inputLine The line to be parsed. Should have a valid transaction format. A valid format consists of a line
     *                  that matches the following regex: "^.{23}\d{11}.{47}$".
     * @return A new transaction with the amount and client information from the inputLine.
     * @throws IllegalArgumentException if inputLine does not have a valid transaction format.
     */
    private Transaction parseLine(String inputLine) throws IllegalArgumentException {
        if (!isValid(inputLine)) {
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

    private static boolean isValid(String line) {
        return PATTERN.matcher(line).matches();
    }
}
