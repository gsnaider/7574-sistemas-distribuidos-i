package ar.uba.fi.contabilidapp.form;

import org.primefaces.event.FileUploadEvent;
import org.primefaces.model.UploadedFile;

import javax.faces.bean.ManagedBean;
import javax.faces.bean.RequestScoped;

@ManagedBean
@RequestScoped
public class FileUploadForm {

    private String file;

    public void handleFileUpload(FileUploadEvent event) {
        System.out.println("File received");
        UploadedFile file = event.getFile();
        byte[] contents = file.getContents();
        String content = new String(contents);

        System.out.println(String.format("Contents: %s", content));
    }

    public String getFile() {
        return file;
    }

    public void setFile(String file) {
        this.file = file;
    }
}
