package ar.uba.fi.contabilidapp.form;

import ar.uba.fi.contabilidapp.upload.LineParser;
import ar.uba.fi.contabilidapp.upload.model.Transaction;
import org.primefaces.event.FileUploadEvent;
import org.primefaces.model.UploadedFile;

import javax.faces.bean.ManagedBean;
import javax.faces.bean.RequestScoped;
import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.util.Scanner;
import org.pmw.tinylog.Logger;

@ManagedBean
@RequestScoped
public class FileUploadForm {

    private String file;

    public void handleFileUpload(FileUploadEvent event) {
        UploadedFile file = event.getFile();
        Logger.info("File received: {}", file.getFileName());
        byte[] contents = file.getContents();

        InputStream is = new ByteArrayInputStream(contents);
        Scanner scanner = new Scanner(is);
        while (scanner.hasNextLine()) {
            Transaction transaction = LineParser.parseLine(scanner.nextLine());
            // TODO Store User and transaction in DB.
            // Check if user already exists in DB, if not create it.
        }
        // TODO store file in DB.
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
}
