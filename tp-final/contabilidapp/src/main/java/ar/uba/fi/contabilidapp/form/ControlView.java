package ar.uba.fi.contabilidapp.form;

import ar.uba.fi.contabilidapp.entities.ControlResults;
import ar.uba.fi.contabilidapp.model.ContabilidappException;
import ar.uba.fi.contabilidapp.model.Model;
import ar.uba.fi.contabilidapp.model.ModelProvider;
import org.pmw.tinylog.Logger;
import org.primefaces.model.DefaultStreamedContent;
import org.primefaces.model.StreamedContent;
import org.primefaces.model.UploadedFile;

import javax.annotation.PostConstruct;
import javax.faces.application.FacesMessage;
import javax.faces.bean.ManagedBean;
import javax.faces.bean.SessionScoped;
import javax.faces.context.FacesContext;
import javax.servlet.ServletContext;
import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

@ManagedBean
@SessionScoped
public class ControlView {

    private Model model;

    private long uploadId;

    private UploadedFile file;

    private ControlResults controlResults;

    @PostConstruct
    public void init() {
        ServletContext servletContext = (ServletContext) FacesContext
                .getCurrentInstance().getExternalContext().getContext();
        ModelProvider modelProvider = (ModelProvider) servletContext.getAttribute(ModelProvider.CTX_ATTRIBUTE);
        model = modelProvider.getModel();
    }

    public void control() {
        if (file != null) {
            Logger.info("File received: {}", file.getFileName());

            byte[] fileData = file.getContents();

            try {
                controlResults = model.controlPeriod(fileData, uploadId);
                if (controlResults.getErrorsFile().isEmpty()) {
                    FacesMessage message = new FacesMessage("Control finalizado sin errores.");
                    FacesContext.getCurrentInstance().addMessage(null, message);
                } else {
                    FacesMessage message = new FacesMessage(FacesMessage.SEVERITY_WARN, "Control finalizado con errores.", "");
                    FacesContext.getCurrentInstance().addMessage(null, message);
                }

            } catch (ContabilidappException e) {
                Logger.warn("Error contorlling period.", e);
                FacesMessage msg = new FacesMessage(FacesMessage.SEVERITY_ERROR, "Error", e.getMessage());
                FacesContext.getCurrentInstance().addMessage(null, msg);
            }
        }
    }

    public Map<String, Long> getClosedUploadIds() {
        Logger.info("Searching for closed period lists.");
        Map<String, Long> idStrings = new HashMap<>();
        List<Long> periodIds = model.getClosedUploadPeriodsIds();
        for (Long id : periodIds) {
            idStrings.put(id.toString(), id);
        }
        return idStrings;
    }

    public StreamedContent getSplitFiles() {
        InputStream stream = new ByteArrayInputStream(controlResults.getZipFiles());
        return  new DefaultStreamedContent(stream, "application/zip", "split-files.zip");
    }

    public StreamedContent getErrorsFile() {
        InputStream stream = new ByteArrayInputStream(controlResults.getErrorsFile().getBytes());
        return  new DefaultStreamedContent(stream, "text/plain", "errors.txt");
    }

    public long getUploadId() {
        return uploadId;
    }

    public void setUploadId(long uploadId) {
        this.uploadId = uploadId;
    }

    public boolean hasSplitFiles() {
        return controlResults != null && controlResults.hasSplitFiles();
    }

    public boolean hasErrorsFile() {
        return controlResults != null && controlResults.hasErrorFiles();
    }

    public UploadedFile getFile() {
        return file;
    }

    public void setFile(UploadedFile file) {
        this.file = file;
    }

}
