package ar.uba.fi.contabilidapp.model;

import ar.uba.fi.contabilidapp.dao.*;
import org.pmw.tinylog.Logger;

import javax.annotation.PostConstruct;
import javax.annotation.PreDestroy;
import javax.faces.bean.ApplicationScoped;
import javax.faces.bean.ManagedBean;
import javax.persistence.Persistence;

@ManagedBean(name = "modelProvider")
@ApplicationScoped
public class ModelProvider {

    private final Model model;
    private final DaoManager daoManager;

    public ModelProvider() {
        daoManager = new DaoManager();
        model = new ModelImpl(daoManager);
    }

    public Model getModel() {
        return model;
    }

    @PreDestroy
    public void tearUp() {
        Logger.info("Closing DaoManager.");
        daoManager.tearUp();
    }

}
