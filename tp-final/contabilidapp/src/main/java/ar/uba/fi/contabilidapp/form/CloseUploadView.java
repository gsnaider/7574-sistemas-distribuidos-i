package ar.uba.fi.contabilidapp.form;

import ar.uba.fi.contabilidapp.model.ContabilidappException;
import ar.uba.fi.contabilidapp.model.Model;
import ar.uba.fi.contabilidapp.model.ModelProvider;
import org.pmw.tinylog.Logger;
import org.primefaces.model.DefaultStreamedContent;
import org.primefaces.model.StreamedContent;

import javax.annotation.PostConstruct;
import javax.faces.application.FacesMessage;
import javax.faces.bean.ManagedBean;
import javax.faces.bean.ManagedProperty;
import javax.faces.bean.RequestScoped;
import javax.faces.context.FacesContext;
import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Map;

@ManagedBean
@RequestScoped
public class CloseUploadView {

    @ManagedProperty(value = "#{modelProvider}")
    private ModelProvider modelProvider;

    private Model model;

    private long uploadId;

    @PostConstruct
    public void init() {
        this.model = modelProvider.getModel();
    }


    public Map<String, Long> getOpenUploadIds() {
        Logger.info("Searching for open period lists.");
        Map<String, Long> idStrings = new HashMap<>();
        for (Long id : model.getOpenUploadPeriodsIds()) {
            idStrings.put(id.toString(), id);
        }
        return idStrings;
    }

    public void closePeriod() {
        try {
            model.closePeriod(uploadId);
            FacesMessage message = new FacesMessage("Periodo " + uploadId + " cerrado con éxito.");
            FacesContext.getCurrentInstance().addMessage(null, message);
        } catch (ContabilidappException e) {
            Logger.warn("Error closing period", e);
            FacesMessage msg = new FacesMessage(FacesMessage.SEVERITY_ERROR, "Error", e.getMessage());
            FacesContext.getCurrentInstance().addMessage(null, msg);
        }

    }

    public long getUploadId() {
        return uploadId;
    }

    public void setUploadId(long uploadId) {
        this.uploadId = uploadId;
    }

    public void setModelProvider(ModelProvider modelProvider) {
        this.modelProvider = modelProvider;
    }
}
