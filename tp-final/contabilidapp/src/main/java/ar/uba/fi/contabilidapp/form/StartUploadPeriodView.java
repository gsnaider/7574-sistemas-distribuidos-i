package ar.uba.fi.contabilidapp.form;

import ar.uba.fi.contabilidapp.model.ContabilidappException;
import ar.uba.fi.contabilidapp.model.Model;
import ar.uba.fi.contabilidapp.model.ModelProvider;
import org.pmw.tinylog.Logger;

import javax.annotation.PostConstruct;
import javax.faces.application.FacesMessage;
import javax.faces.bean.ManagedBean;
import javax.faces.bean.RequestScoped;
import javax.faces.context.FacesContext;
import javax.servlet.ServletContext;

@ManagedBean
@RequestScoped
public class StartUploadPeriodView {

    private long uploadPeriodId;

    private Model model;

    @PostConstruct
    public void init() {
        ServletContext servletContext = (ServletContext) FacesContext
                .getCurrentInstance().getExternalContext().getContext();
        ModelProvider modelProvider = (ModelProvider) servletContext.getAttribute(ModelProvider.CTX_ATTRIBUTE);
        model = modelProvider.getModel();
    }

    public String startUploadPeriod() {
        try {
            uploadPeriodId = model.startUploadPeriod();
            return "upload-period-started";
        } catch (ContabilidappException e) {
            Logger.warn("Error starting period.", e);
            FacesMessage msg = new FacesMessage(FacesMessage.SEVERITY_ERROR, "Error", "Error iniciando periodo");
            FacesContext.getCurrentInstance().addMessage(null, msg);
        }
        return "";

    }

    public long getUploadPeriodId() {
        return uploadPeriodId;
    }

}
