package ar.uba.fi.contabilidapp.model;

import ar.uba.fi.contabilidapp.dao.DaoManager;
import ar.uba.fi.contabilidapp.entities.*;
import org.pmw.tinylog.Logger;

import java.io.*;
import java.math.BigDecimal;
import java.math.RoundingMode;
import java.text.DecimalFormat;
import java.util.*;
import java.util.regex.Pattern;

public class ModelImpl implements Model {

    private static final int CLIENT_CODE_POSITION = 6;
    private static final int CLIENT_CODE_LENGTH = 9;
    private static final int CLIENT_CODE_END_POSITION = CLIENT_CODE_POSITION + CLIENT_CODE_LENGTH;

    private static final int AMOUNT_POSITION = 25;
    static final int AMOUNT_LENGTH = 9;
    private static final int AMOUNT_END_POSITION = AMOUNT_POSITION + AMOUNT_LENGTH;


    private static final int CONTROL_CLIENT_NAME_POSITION = 9;
    private static final int CONTROL_CLIENT_NAME_LENGTH = 33;
    private static final int CONTROL_CLIENT_NAME_END_POSITION = CONTROL_CLIENT_NAME_POSITION + CONTROL_CLIENT_NAME_LENGTH;

    private static final int CONTROL_AMOUNT_POSITION = 46;
    private static final int CONTROL_AMOUNT_LENGTH = 10;
    private static final int CONTROL_AMOUNT_END_POSITION = CONTROL_AMOUNT_POSITION + CONTROL_AMOUNT_LENGTH;


    private static final String INPUT_LINE_REGEX = String.format("^.{25}\\d{%d}.{47}$", AMOUNT_LENGTH);
    private static final String CONTROL_LINE_REGEX = "^.{46}\\d{7}\\.\\d{2}.{11}$";

    private static final Pattern INPUT_PATTERN = Pattern.compile(INPUT_LINE_REGEX);
    private static final Pattern CONTROL_PATTERN = Pattern.compile(CONTROL_LINE_REGEX);
    private static final String NO_AMOUNT = "---";

    private static final String SPLIT_FILE_NAME = "file%d.txt";


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
            String line = scanner.nextLine();
            if (!line.isEmpty()) {
                Transaction transaction = parseTransactionLine(line);
                transactions.add(transaction);
            }
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
            DecimalFormat df = new DecimalFormat("000000000");
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
        return daoManager.getUploadDao().getOpenUploadPeriodsIds();
    }

    @Override
    public List<Long> getClosedUploadPeriodsIds() {
        return daoManager.getUploadDao().getClosedUploadPeriodsIds();
    }

    @Override
    public void closePeriod(long uploadId) throws ContabilidappException {
        daoManager.getUploadDao().closePeriod(uploadId);
    }

    Map<String, ControlRecord> parseControlFile(byte[] fileData) throws ContabilidappException {
        Map<String, ControlRecord> records = new HashMap<>();

        try (BufferedReader reader = new BufferedReader(new InputStreamReader(new ByteArrayInputStream(fileData)))) {
            String line = reader.readLine();
            while (line != null) {
                if (!line.isEmpty()) {
                    ControlRecord controlRecord = parseControlLine(line);
                    records.put(controlRecord.getClientCode(), controlRecord);
                }
                line = reader.readLine();
            }
        } catch (IOException e) {
            throw new ContabilidappException("Error reading control file", e);
        }
        return records;
    }

    private String generateErrorFile(List<ControlErrorRecord> errors) {
        if (errors.isEmpty()) {
            return "";
        }
        StringBuilder errorsTable = new StringBuilder();
        Formatter lineFormatter = new Formatter(errorsTable);

        // Add header.
        lineFormatter.format(
                "%s,%s,%s\n",
                "Cliente",
                "Monto de periodo",
                "Monto de control");

        DecimalFormat df = new DecimalFormat("0000000.00");
        for (ControlErrorRecord errorRecord : errors) {

            BigDecimal transactionAmount = errorRecord.getTransactionAmount();
            BigDecimal controlAmount = errorRecord.getControlAmount();

            String formattedTransactionAmount = transactionAmount == null ? NO_AMOUNT : df.format(transactionAmount);
            String formattedControlAmount = controlAmount == null ? NO_AMOUNT : df.format(controlAmount);

            lineFormatter.format(
                    "%s,%s,%s\n",
                    errorRecord.getClientCode(),
                    formattedTransactionAmount,
                    formattedControlAmount);
        }

        return errorsTable.toString();
    }

    @Override
    public ControlResults controlPeriod(byte[] fileData, long uploadId) throws ContabilidappException {
        Map<String, ControlRecord> controlRecords = parseControlFile(fileData);

        Map<String, BigDecimal> transactionAmounts = new HashMap<>();
        Set<String> transactionClients = new HashSet<>();
        List<Object[]> aggregatedDataList = daoManager.getAggregatedInputData(uploadId);
        for (Object[] tuple : aggregatedDataList) {
            Client client = (Client) tuple[0];
            BigDecimal amount = (BigDecimal) tuple[1];
            transactionAmounts.put(client.getClientCode(), amount);
            transactionClients.add(client.getClientCode());
        }

        List<ControlErrorRecord> errors = new ArrayList<>();

        for (ControlRecord controlRecord : controlRecords.values()) {
            String clientCode = controlRecord.getClientCode();
            BigDecimal transactionAmount = transactionAmounts.get(clientCode);
            BigDecimal controlAmount = controlRecord.getControlAmount();

            controlRecord.setTransactionsAmount(transactionAmount);
            if (!controlRecord.amountsMatch()) {
                errors.add(new ControlErrorRecord(clientCode, transactionAmount, controlAmount));
            }
            if (transactionAmount != null) {
                transactionClients.remove(clientCode);
            }
        }

        for (String clientCode : transactionClients) {
            BigDecimal transactionAmount = transactionAmounts.get(clientCode);
            errors.add(new ControlErrorRecord(clientCode, transactionAmount, null));
        }

        byte[] splitTransactionZipFiles = getSplitFiles(uploadId, controlRecords);

        return new ControlResults(generateErrorFile(errors), splitTransactionZipFiles);
    }

    private byte[] getSplitFiles(long uploadId, Map<String, ControlRecord> controlRecords) throws ContabilidappException {
        try {
            List<Transaction> transactions = daoManager.getTransactionsFromPeriod(uploadId);
            if (transactions.isEmpty()) {
                Logger.warn("No transactions found for period {}", uploadId);
                return new byte[0];
            }

            ZipBuilder zipBuilder = new ZipBuilder();
            int currentSplitFileId = 1;
            long currentInputFileId = transactions.get(0).getInputFile().getId();
            DecimalFormat df = new DecimalFormat("0000000.00");
            StringBuilder stringBuilder = new StringBuilder();
            Formatter lineFormatter = new Formatter(stringBuilder);
            for (Transaction transaction : transactions) {
                ControlRecord controlRecord = controlRecords.get(transaction.getClient().getClientCode());
                if (controlRecord != null && controlRecord.amountsMatch()) {
                    long inputFileId = transaction.getInputFile().getId();
                    if (inputFileId != currentInputFileId) {
                        zipBuilder.addToZip(String.format(SPLIT_FILE_NAME, currentSplitFileId), stringBuilder.toString());
                        stringBuilder = new StringBuilder();
                        lineFormatter = new Formatter(stringBuilder);
                        currentInputFileId = inputFileId;
                        currentSplitFileId++;
                    }
                    lineFormatter.format(
                            "%s%s%s%s%s\n",
                            controlRecord.getClientCode(),
                            controlRecord.getClientName(),
                            controlRecord.getMiddleCode(),
                            df.format(transaction.getAmount()),
                            controlRecord.getSuffixCode());
                }


            }
            zipBuilder.addToZip(String.format(SPLIT_FILE_NAME, currentSplitFileId), stringBuilder.toString());
            return zipBuilder.getZip();
        } catch (IOException e) {
            throw new ContabilidappException("Error generating split zip file.", e);
        }

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

        return daoManager.getInputFileDao().addInputFile(inputFile, uploadId);
    }


    private ControlRecord parseControlLine(String controlLine) throws ContabilidappException {
        if (!isValidLine(controlLine, CONTROL_PATTERN)) {
            throw new ContabilidappException(String.format("Invalid line format: '%s'", controlLine));
        }

        String clientCode = controlLine.substring(0, CONTROL_CLIENT_NAME_POSITION);
        String name = controlLine.substring(CONTROL_CLIENT_NAME_POSITION, CONTROL_CLIENT_NAME_END_POSITION);
        String middleCode = controlLine.substring(CONTROL_CLIENT_NAME_END_POSITION, CONTROL_AMOUNT_POSITION);

        String amountString = controlLine.substring(CONTROL_AMOUNT_POSITION, CONTROL_AMOUNT_END_POSITION);
        BigDecimal amount = new BigDecimal(amountString);

        String suffixCode = controlLine.substring(CONTROL_AMOUNT_END_POSITION);

        return new ControlRecord(clientCode, name, middleCode, amount, suffixCode);

    }


    /**
     * Parses a line from an input file to a {@link Transaction}.
     *
     * @param inputLine The line to be parsed. Should have a valid transaction format. A valid format consists of a line
     *                  that matches the following regex: "^.{23}\d{11}.{47}$".
     * @return A new transaction with the amount and client information from the inputLine.
     * @throws IllegalArgumentException if inputLine does not have a valid transaction format.
     */
    private Transaction parseTransactionLine(String inputLine) throws ContabilidappException {
        if (!isValidLine(inputLine, INPUT_PATTERN)) {
            throw new ContabilidappException(String.format("Invalid line format: '%s'", inputLine));
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

    private static boolean isValidLine(String line, Pattern pattern) {
        Logger.info("Validating line '{}'", line);
        return pattern.matcher(line).matches();
    }

}
