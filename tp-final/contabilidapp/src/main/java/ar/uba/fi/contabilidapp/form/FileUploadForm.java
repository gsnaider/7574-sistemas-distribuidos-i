package ar.uba.fi.contabilidapp.form;

import ar.uba.fi.contabilidapp.model.Model;
import ar.uba.fi.contabilidapp.model.ModelProvider;
import org.pmw.tinylog.Logger;
import org.primefaces.event.FileUploadEvent;
import org.primefaces.model.UploadedFile;

import javax.annotation.PostConstruct;
import javax.faces.bean.ManagedBean;
import javax.faces.bean.ManagedProperty;
import javax.faces.bean.RequestScoped;

@ManagedBean
@RequestScoped
public class FileUploadForm {

    @ManagedProperty(value = "#{modelProvider}")
    private ModelProvider modelProvider;

    private Model model;

    private String file;

    @PostConstruct
    public void init() {
        this.model = modelProvider.getModel();
    }

    public void handleFileUpload(FileUploadEvent event) {
        UploadedFile file = event.getFile();
        Logger.info("File received: {}", file.getFileName());
        byte[] fileData = file.getContents();
        // TODO check and show errors.
        model.handleFileUpload(fileData);
    }

    public String endUpload() {
        Logger.info("Ending model of files.");
        return "/index?faces-redirect=true";
    }

    public String getFile() {
        return file;
    }

    public void setFile(String file) {
        this.file = file;
    }

    public void setModelProvider(ModelProvider modelProvider) {
        this.modelProvider = modelProvider;
    }
}
