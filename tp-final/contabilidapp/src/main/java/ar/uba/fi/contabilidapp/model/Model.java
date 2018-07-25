package ar.uba.fi.contabilidapp.model;

import ar.uba.fi.contabilidapp.entities.ControlResults;

import java.util.List;

// TODO separar vista de model/DB en otro tomcat, y llamar por web service.
// hacer otra implementacion del modelo en la vista (ModelProxy), que redirija el mensaje al otro server.

public interface Model {

    long startUploadPeriod() throws ContabilidappException;

    void handleFileUpload(byte[] fileData, long uploadId) throws ContabilidappException;

    String getAggregatedDataFile(long uploadId);

    List<Long> getOpenUploadPeriodsIds();

    List<Long> getClosedUploadPeriodsIds();

    void closePeriod(long uploadId) throws ContabilidappException;

    ControlResults controlPeriod(byte[] fileData, long uploadId) throws ContabilidappException;


}
