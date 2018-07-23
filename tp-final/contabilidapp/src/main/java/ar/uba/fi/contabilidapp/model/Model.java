package ar.uba.fi.contabilidapp.model;

import java.io.InputStream;
import java.util.List;

public interface Model {

    public long startUploadPeriod();

    public void handleFileUpload(byte[] fileData, long uploadId) throws ContabilidappException;

    public String getAggregatedDataFile(long uploadId);

    List<Long> getOpenUploadPeriodsIds();
}
