package ar.uba.fi.contabilidapp.model;

import ar.uba.fi.contabilidapp.dao.DaoManager;
import org.pmw.tinylog.Logger;

public class ModelProvider {

    public static final String CTX_ATTRIBUTE = "ModelProvider";

    private final Model model;
    private final DaoManager daoManager;

    public ModelProvider(String backendLocation) {
        if (backendLocation.equals("local")) {
            daoManager = new DaoManager();
            model = new ModelImpl(daoManager);
        } else {
            daoManager = null;
            model = new ModelProxy(backendLocation);
        }
    }

    public Model getModel() {
        return model;
    }

    public void tearUp() {
        if (daoManager != null) {
            Logger.info("Closing DaoManager.");
            daoManager.tearUp();
        }
    }
}
