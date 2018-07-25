package ar.uba.fi.contabilidapp.model;

import ar.uba.fi.contabilidapp.entities.ControlResults;
import ar.uba.fi.contabilidapp.entities.UploadPeriod;
import org.pmw.tinylog.Logger;

import javax.ws.rs.client.Client;
import javax.ws.rs.client.ClientBuilder;
import javax.ws.rs.client.Entity;
import javax.ws.rs.core.Form;
import javax.ws.rs.core.GenericType;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import java.util.Arrays;
import java.util.List;

public class ModelProxy implements Model {

    private static final String REST_URI
            = "http://localhost:8080/contabilidapp-backend/service";

    private Client client = ClientBuilder.newClient();

    @Override
    public long startUploadPeriod() {
        Response response = client.target(REST_URI)
                .path("upload-period")
                .request(MediaType.APPLICATION_JSON)
                .post(Entity.entity(new UploadPeriod(), MediaType.APPLICATION_JSON));
        Long uploadId = response.readEntity(Long.class);
        return uploadId;
    }

    @Override
    public void handleFileUpload(byte[] fileData, long uploadId) throws ContabilidappException {
        Response response = client.target(REST_URI)
                .path("input-file")
                .request(MediaType.APPLICATION_JSON)
                .post(Entity.entity(new PeriodFile(uploadId, Arrays.toString(fileData)), MediaType.APPLICATION_JSON));
        Logger.info("Status {}", response.getStatus());
        Logger.info("Response {}", response);

        if (response.getStatus() != Response.Status.OK.getStatusCode()) {
            throw new ContabilidappException("Remote error uploading file.");
        }
    }

    @Override
    public String getAggregatedDataFile(long uploadId) {
        return client
                .target(REST_URI)
                .path("aggregated").queryParam("uploadId", uploadId)
                .request(MediaType.APPLICATION_JSON)
                .get(String.class);
    }

    @Override
    public List<Long> getOpenUploadPeriodsIds() {
        return client
                .target(REST_URI)
                .path("upload-period").queryParam("open", true)
                .request(MediaType.APPLICATION_JSON)
                .get(new GenericType<List<Long>>() {
                });
    }

    @Override
    public List<Long> getClosedUploadPeriodsIds() {
        return client
                .target(REST_URI)
                .path("upload-period").queryParam("open", false)
                .request(MediaType.APPLICATION_JSON)
                .get(new GenericType<List<Long>>() {
                });
    }

    @Override
    public void closePeriod(long uploadId) throws ContabilidappException {
        Response response = client.target(REST_URI)
                .path("upload-period").path(String.valueOf(uploadId))
                .request(MediaType.APPLICATION_JSON)
                .post(Entity.entity(new UploadPeriod(), MediaType.APPLICATION_JSON));
        if (response.getStatus() != Response.Status.OK.getStatusCode()) {
            throw new ContabilidappException("Remote error closing period");
        }
    }

    @Override
    public ControlResults controlPeriod(byte[] fileData, long uploadId) throws ContabilidappException {
        Response response = client.target(REST_URI)
                .path("control")
                .request(MediaType.APPLICATION_JSON)
                .post(Entity.entity(new PeriodFile(uploadId, Arrays.toString(fileData)), MediaType.APPLICATION_JSON));
        if (response.getStatus() != Response.Status.OK.getStatusCode()) {
            throw new ContabilidappException("Remote error uploading file.");
        }
        ControlResults controlResults = response.readEntity(ControlResults.class);
        return controlResults;
    }
}
