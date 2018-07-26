package ar.uba.fi.contabilidapp.service;

import ar.uba.fi.contabilidapp.entities.ControlResults;
import ar.uba.fi.contabilidapp.entities.UploadPeriod;
import ar.uba.fi.contabilidapp.model.ContabilidappException;
import ar.uba.fi.contabilidapp.model.Model;
import ar.uba.fi.contabilidapp.model.ModelProvider;
import ar.uba.fi.contabilidapp.model.PeriodFile;
import org.pmw.tinylog.Logger;

import javax.annotation.PostConstruct;
import javax.servlet.ServletContext;
import javax.ws.rs.*;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.GenericEntity;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;
import java.util.List;

@Path("/service")
public class WebService {

    private Model model;

    @Context
    private ServletContext servletContext;

    @PostConstruct
    public void init() {
        ModelProvider modelProvider = (ModelProvider) servletContext.getAttribute(ModelProvider.CTX_ATTRIBUTE);
        model = modelProvider.getModel();
    }

    @POST
    @Path("/upload-period")
    @Produces(MediaType.APPLICATION_JSON)
    @Consumes(MediaType.APPLICATION_JSON)
    public Response startUploadPeriod(UploadPeriod uploadPeriod) {
        try {
            long uploadId = model.startUploadPeriod();
            return Response.status(Status.OK).entity(uploadId).build();
        } catch (ContabilidappException e) {
            Logger.error("Error starting period. ", e);
            throw new InternalServerErrorException(e);
        }
    }

    @POST
    @Path("/upload-period/{uploadId}")
    @Produces(MediaType.APPLICATION_JSON)
    public Response closeUploadPeriod(@PathParam("uploadId") long uploadId) {
        try {
            model.closePeriod(uploadId);
            return Response.status(Status.OK).build();
        } catch (ContabilidappException e) {
            Logger.error("Error closing period. ", e);
            throw new InternalServerErrorException(e);
        }
    }

    @POST
    @Path("/input-file")
    @Produces(MediaType.APPLICATION_JSON)
    @Consumes(MediaType.APPLICATION_JSON)
        public Response handleFileUpload(PeriodFile periodFile) {
        long uploadId = periodFile.getUploadPeriodId();
        byte[] fileData = periodFile.getFileData();
        Logger.info("Uploading file: {}", periodFile);
        try {
            model.handleFileUpload(fileData, uploadId);
            return Response.status(Status.OK).build();
        } catch (ContabilidappException e) {
            Logger.error("Error saving file. ", e);
            return Response.status(Status.INTERNAL_SERVER_ERROR).entity(e.getMessage()).build();
        }
    }

    @GET
    @Path("/aggregated")
    @Produces(MediaType.APPLICATION_JSON)
    public Response getAggregatedDataFile(@QueryParam("uploadId") long uploadId) {
        try {
            String fileData = model.getAggregatedDataFile(uploadId);
            return Response.status(Status.OK).entity(fileData).build();
        } catch (ContabilidappException e) {
            Logger.error("Error getting aggregated file. ", e);
            return Response.status(Status.INTERNAL_SERVER_ERROR).entity(e.getMessage()).build();
        }
    }

    @GET
    @Path("/upload-period")
    @Produces(MediaType.APPLICATION_JSON)
    public Response getUploadPeriodsIds(@QueryParam("open") boolean open) {
        try {
            List<Long> uploadIds = open ? model.getOpenUploadPeriodsIds() : model.getClosedUploadPeriodsIds();
            GenericEntity<List<Long>> entity = new GenericEntity<List<Long>>(uploadIds) {};
            return Response.status(Status.OK).entity(entity).build();
        } catch (ContabilidappException e) {
            Logger.error("Error searching periods. ", e);
            return Response.status(Status.INTERNAL_SERVER_ERROR).entity(e.getMessage()).build();
        }
    }

    @POST
    @Path("/control")
    @Produces(MediaType.APPLICATION_JSON)
    @Consumes(MediaType.APPLICATION_JSON)
    public Response controlPeriod(PeriodFile periodFile) {
        long uploadId = periodFile.getUploadPeriodId();
        byte[] fileData = periodFile.getFileData();
        try {
            ControlResults results = model.controlPeriod(fileData, uploadId);
            return Response.status(Status.OK).entity(results).build();
        } catch (ContabilidappException e) {
            Logger.error("Error controlling. ", e);
            return Response.status(Status.INTERNAL_SERVER_ERROR).entity(e.getMessage()).build();
        }
    }


}
