package ar.uba.fi.contabilidapp.form;

import javax.faces.bean.ManagedBean;
import javax.faces.bean.RequestScoped;

@ManagedBean
@RequestScoped
public class FileUploadForm {

    private String file;

    public String upload() {
        System.out.println(String.format("Received: %s", file));
        return "/index?faces-redirect=true";
    }

    public String getFile() {
        return file;
    }

    public void setFile(String file) {
        this.file = file;
    }
}
