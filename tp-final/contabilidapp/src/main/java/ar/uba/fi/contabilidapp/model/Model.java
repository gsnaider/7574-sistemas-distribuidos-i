package ar.uba.fi.contabilidapp.model;

import java.io.InputStream;

public interface Model {

    public void handleFileUpload(byte[] fileData);

    public String getAggregatedDataFile(long uploadId);

}
