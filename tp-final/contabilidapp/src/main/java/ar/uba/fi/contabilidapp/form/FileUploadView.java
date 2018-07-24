package ar.uba.fi.contabilidapp.form;

import ar.uba.fi.contabilidapp.model.ContabilidappException;
import ar.uba.fi.contabilidapp.model.Model;
import ar.uba.fi.contabilidapp.model.ModelProvider;
import org.pmw.tinylog.Logger;
import org.primefaces.event.FileUploadEvent;
import org.primefaces.model.UploadedFile;

import javax.annotation.PostConstruct;
import javax.faces.application.FacesMessage;
import javax.faces.bean.ManagedBean;
import javax.faces.bean.ManagedProperty;
import javax.faces.bean.RequestScoped;
import javax.faces.context.FacesContext;
import java.util.HashMap;
import java.util.Map;

@ManagedBean
@RequestScoped
public class FileUploadView {

    @ManagedProperty(value = "#{modelProvider}")
    private ModelProvider modelProvider;

    private Model model;

    private long uploadId;

    private UploadedFile file;


    @PostConstruct
    public void init() {
        this.model = modelProvider.getModel();
    }

    public void upload() {
        if(file != null) {
            Logger.info("File received: {}", file.getFileName());

            byte[] fileData = file.getContents();

            try {
                model.handleFileUpload(fileData, uploadId);
                FacesMessage message = new FacesMessage("Archivo " + file.getFileName() + " cargado con éxito.");
                FacesContext.getCurrentInstance().addMessage(null, message);
            } catch (ContabilidappException e) {
                Logger.warn("Error uploading file", e);
                FacesMessage msg = new FacesMessage(FacesMessage.SEVERITY_ERROR, "Error", e.getMessage());
                FacesContext.getCurrentInstance().addMessage(null, msg);
            }

        }
    }

    public Map<String, Long> getOpenUploadIds() {
        Logger.info("Searching for open period lists.");
        Map<String, Long> idStrings = new HashMap<>();
        for (Long id : model.getOpenUploadPeriodsIds()) {
            idStrings.put(id.toString(), id);
        }
        return idStrings;
    }

    public long getUploadId() {
        return uploadId;
    }

    public void setUploadId(long uploadId) {
        this.uploadId = uploadId;
    }

    public UploadedFile getFile() {
        return file;
    }

    public void setFile(UploadedFile file) {
        this.file = file;
    }

    public void setModelProvider(ModelProvider modelProvider) {
        this.modelProvider = modelProvider;
    }
}
