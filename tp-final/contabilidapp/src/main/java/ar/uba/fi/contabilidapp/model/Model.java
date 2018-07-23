package ar.uba.fi.contabilidapp.model;

import java.util.List;

// TODO separar vista de model/DB en otro tomcat, y llamar por web service.
// hacer otra implementacion del modelo en la vista (ModelFacade), que redirija el mensaje al otro server.

public interface Model {

    long startUploadPeriod();

    void handleFileUpload(byte[] fileData, long uploadId) throws ContabilidappException;

    String getAggregatedDataFile(long uploadId);

    List<Long> getOpenUploadPeriodsIds();

    List<Long> getClosedUploadPeriodsIds();

    void closePeriod(long uploadId) throws ContabilidappException;

    String controlPeriod(byte[] fileData, long uploadId) throws ContabilidappException;
}
