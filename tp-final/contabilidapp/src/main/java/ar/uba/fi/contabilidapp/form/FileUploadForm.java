package ar.uba.fi.contabilidapp.form;

import ar.uba.fi.contabilidapp.dao.DaoBean;
import ar.uba.fi.contabilidapp.upload.LineParser;
import ar.uba.fi.contabilidapp.upload.model.InputFile;
import ar.uba.fi.contabilidapp.upload.model.Transaction;
import org.primefaces.event.FileUploadEvent;
import org.primefaces.model.UploadedFile;

import javax.faces.bean.ManagedBean;
import javax.faces.bean.ManagedProperty;
import javax.faces.bean.RequestScoped;
import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.util.Scanner;
import org.pmw.tinylog.Logger;

@ManagedBean
@RequestScoped
public class FileUploadForm {

    @ManagedProperty(value = "#{daoBean}")
    private DaoBean daoBean;

    private String file;

    public void handleFileUpload(FileUploadEvent event) {
        UploadedFile file = event.getFile();
        Logger.info("File received: {}", file.getFileName());

        byte[] fileData = file.getContents();
        persistInputFile(fileData);

        InputStream is = new ByteArrayInputStream(fileData);
        Scanner scanner = new Scanner(is);
        while (scanner.hasNextLine()) {
            Transaction transaction = LineParser.parseLine(scanner.nextLine());

            // TODO Store User and transaction in DB.
            // Check if user already exists in DB, if not create it.
        }

    }

    private void persistInputFile(byte[] fileData) {
        InputFile inputFile = new InputFile();
        inputFile.setFileData(fileData);
        // TODO set current uploadId
        daoBean.getInputFileDao().add(inputFile);
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
