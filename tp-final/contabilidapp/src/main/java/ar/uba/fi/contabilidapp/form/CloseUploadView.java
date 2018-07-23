package ar.uba.fi.contabilidapp.form;

import ar.uba.fi.contabilidapp.model.Model;
import ar.uba.fi.contabilidapp.model.ModelProvider;
import org.primefaces.model.DefaultStreamedContent;
import org.primefaces.model.StreamedContent;

import javax.annotation.PostConstruct;
import javax.faces.bean.ManagedBean;
import javax.faces.bean.ManagedProperty;
import javax.faces.bean.RequestScoped;
import java.io.ByteArrayInputStream;
import java.io.InputStream;

@ManagedBean
@RequestScoped
public class CloseUploadView {

    @ManagedProperty(value = "#{modelProvider}")
    private ModelProvider modelProvider;

    private Model model;

    @PostConstruct
    public void init() {
        this.model = modelProvider.getModel();
    }


    public StreamedContent getFile() {
        String fileString = model.getAggregatedDataFile(0);
        InputStream stream = new ByteArrayInputStream(fileString.getBytes());
        return  new DefaultStreamedContent(stream, "text/plain", "aggregated.txt");
    }

    public void setModelProvider(ModelProvider modelProvider) {
        this.modelProvider = modelProvider;
    }


}
