package ar.uba.fi.contabilidapp.model;

import ar.uba.fi.contabilidapp.dao.DaoManager;
import org.pmw.tinylog.Logger;

/**
 * Class for initializing and providing access to the {@link Model} and {@link DaoManager} implementations for the app.
 */
public class ModelProvider {

    public static final String CTX_ATTRIBUTE = "ModelProvider";

    private final Model model;
    private final DaoManager daoManager;

    public ModelProvider() {
        daoManager = new DaoManager();
        model = new ModelImpl(daoManager);
    }

    public Model getModel() {
        return model;
    }

    public void tearUp() {
        Logger.info("Closing DaoManager.");
        daoManager.tearUp();
    }

}
