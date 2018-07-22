package ar.uba.fi.contabilidapp.form;

import ar.uba.fi.contabilidapp.dao.DaoBean;
import ar.uba.fi.contabilidapp.upload.LineParser;
import ar.uba.fi.contabilidapp.upload.model.Client;
import ar.uba.fi.contabilidapp.upload.model.InputFile;
import ar.uba.fi.contabilidapp.upload.model.Transaction;
import org.pmw.tinylog.Logger;
import org.primefaces.event.FileUploadEvent;
import org.primefaces.model.UploadedFile;

import javax.faces.bean.ManagedBean;
import javax.faces.bean.ManagedProperty;
import javax.faces.bean.RequestScoped;
import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

@ManagedBean
@RequestScoped
public class FileUploadForm {

    @ManagedProperty(value = "#{daoBean}")
    private DaoBean daoBean;

    private String file;

    public void handleFileUpload(FileUploadEvent event) {
        UploadedFile file = event.getFile();
        Logger.info("File received: {}", file.getFileName());

        List<Transaction> transactions = new ArrayList<>();

        byte[] fileData = file.getContents();
        InputStream is = new ByteArrayInputStream(fileData);
        Scanner scanner = new Scanner(is);
        while (scanner.hasNextLine()) {
            Transaction transaction = LineParser.parseLine(scanner.nextLine());
            transactions.add(transaction);
        }

        InputFile inputFile = persistInputFile(fileData, transactions);
    }

    private InputFile persistInputFile(byte[] fileData, List<Transaction> transactions) {
        for (Transaction transaction : transactions) {
            Client client = daoBean.getClientDao().addIfNotPresent(transaction.getClient());
            transaction.setClient(client);
        }
        InputFile inputFile = new InputFile();
        inputFile.setFileData(fileData);
        inputFile.setTransactions(transactions);
        // TODO set current uploadId
        // TODO check if clients already exist.
        return daoBean.getInputFileDao().add(inputFile);
    }


    public String endUpload() {
        Logger.info("Ending upload of files.");
        return "/index?faces-redirect=true";
    }

    public String getFile() {
        return file;
    }

    public void setFile(String file) {
        this.file = file;
    }

    public void setDaoBean(DaoBean daoBean) {
        this.daoBean = daoBean;
    }
}
