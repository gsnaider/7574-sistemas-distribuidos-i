package ar.uba.fi.contabilidapp.form;

import ar.uba.fi.contabilidapp.model.Model;
import ar.uba.fi.contabilidapp.model.ModelProvider;
import org.pmw.tinylog.Logger;
import org.primefaces.model.DefaultStreamedContent;
import org.primefaces.model.StreamedContent;

import javax.annotation.PostConstruct;
import javax.faces.bean.ManagedBean;
import javax.faces.bean.RequestScoped;
import javax.faces.context.FacesContext;
import javax.servlet.ServletContext;
import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Map;

/**
 * JSF Bean for controlling the download-aggregated.xhtml view.
 */
@ManagedBean
@RequestScoped
public class DownloadAggregatedView {

    private Model model;

    private long uploadId;

    @PostConstruct
    public void init() {
        ServletContext servletContext = (ServletContext) FacesContext
                .getCurrentInstance().getExternalContext().getContext();
        ModelProvider modelProvider = (ModelProvider) servletContext.getAttribute(ModelProvider.CTX_ATTRIBUTE);
        model = modelProvider.getModel();
    }

    public Map<String, Long> getClosedUploadIds() {
        Logger.info("Searching for closed period lists.");
        Map<String, Long> idStrings = new HashMap<>();
        for (Long id : model.getClosedUploadPeriodsIds()) {
            idStrings.put(id.toString(), id);
        }
        return idStrings;
    }

    public StreamedContent getFile() {
        String fileString = model.getAggregatedDataFile(uploadId);
        InputStream stream = new ByteArrayInputStream(fileString.getBytes());
        return  new DefaultStreamedContent(stream, "text/plain", "aggregated.txt");
    }

    public long getUploadId() {
        return uploadId;
    }

    public void setUploadId(long uploadId) {
        this.uploadId = uploadId;
    }

}
