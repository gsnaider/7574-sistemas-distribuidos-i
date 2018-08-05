package ar.uba.fi.contabilidapp.model;

import org.pmw.tinylog.Logger;

import javax.servlet.ServletContext;
import javax.servlet.ServletContextEvent;
import javax.servlet.ServletContextListener;
import javax.servlet.annotation.WebListener;

/**
 * Servlet listener for reading configuration flags on startup and initializing the {@link ModelProvider}.
 */
@WebListener
public class InitListener implements ServletContextListener {

    @Override
    public void contextInitialized(ServletContextEvent sce) {
        Logger.info("Initializing context.");
        ServletContext ctx = sce.getServletContext();

        String backendLocation = ctx.getInitParameter("backend");
        ModelProvider modelProvider = new ModelProvider(backendLocation);


        ctx.setAttribute(ModelProvider.CTX_ATTRIBUTE, modelProvider);
    }

    @Override
    public void contextDestroyed(ServletContextEvent sce) {
        Logger.info("Destroying context.");
        ServletContext ctx = sce.getServletContext();
        ModelProvider modelProvider = (ModelProvider) ctx.getAttribute(ModelProvider.CTX_ATTRIBUTE);
        modelProvider.tearUp();
    }
}
