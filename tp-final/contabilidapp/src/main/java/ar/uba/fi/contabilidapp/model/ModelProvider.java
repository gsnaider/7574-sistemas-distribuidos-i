package ar.uba.fi.contabilidapp.model;

import ar.uba.fi.contabilidapp.dao.DaoManager;
import org.pmw.tinylog.Logger;

public class ModelProvider {

    public static final String CTX_ATTRIBUTE = "ModelProvider";

    private final Model model;
    private final DaoManager daoManager;

    public ModelProvider() {
        daoManager = new DaoManager();
        model = new ModelProxy(); // TODO use property for config.
    }

    public Model getModel() {
        return model;
    }

    public void tearUp() {
        Logger.info("Closing DaoManager.");
        daoManager.tearUp();
    }

}
