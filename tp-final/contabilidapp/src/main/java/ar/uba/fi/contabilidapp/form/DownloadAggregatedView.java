package ar.uba.fi.contabilidapp.form;

import ar.uba.fi.contabilidapp.model.Model;
import ar.uba.fi.contabilidapp.model.ModelProvider;
import org.pmw.tinylog.Logger;
import org.primefaces.application.resource.barcode.UPCAGenerator;
import org.primefaces.model.DefaultStreamedContent;
import org.primefaces.model.StreamedContent;

import javax.annotation.PostConstruct;
import javax.faces.bean.ManagedBean;
import javax.faces.bean.ManagedProperty;
import javax.faces.bean.RequestScoped;
import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Map;

@ManagedBean
@RequestScoped
public class DownloadAggregatedView {


    @ManagedProperty(value = "#{modelProvider}")
    private ModelProvider modelProvider;

    private Model model;

    private long uploadId;

    @PostConstruct
    public void init() {
        this.model = modelProvider.getModel();
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

    public void setModelProvider(ModelProvider modelProvider) {
        this.modelProvider = modelProvider;
    }


}
