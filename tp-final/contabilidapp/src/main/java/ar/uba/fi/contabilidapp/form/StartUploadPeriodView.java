package ar.uba.fi.contabilidapp.form;

import ar.uba.fi.contabilidapp.model.Model;
import ar.uba.fi.contabilidapp.model.ModelProvider;

import javax.annotation.PostConstruct;
import javax.faces.bean.ManagedBean;
import javax.faces.bean.ManagedProperty;
import javax.faces.bean.RequestScoped;

@ManagedBean
@RequestScoped
public class StartUploadPeriodView {

    private long uploadPeriodId;

    @ManagedProperty(value = "#{modelProvider}")
    private ModelProvider modelProvider;

    private Model model;

    @PostConstruct
    public void init() {
        this.model = modelProvider.getModel();
    }

    public String startUploadPeriod() {
        uploadPeriodId = model.startUploadPeriod();
        return "upload-period-started";
    }

    public long getUploadPeriodId() {
        return uploadPeriodId;
    }

    public void setModelProvider(ModelProvider modelProvider) {
        this.modelProvider = modelProvider;
    }



}
