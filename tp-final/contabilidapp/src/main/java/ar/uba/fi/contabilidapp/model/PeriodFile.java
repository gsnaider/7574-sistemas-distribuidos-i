package ar.uba.fi.contabilidapp.model;

import ar.uba.fi.contabilidapp.entities.InputFile;

/**
 * Class for storing data from an {@link InputFile} upload request.
 */
public class PeriodFile {

    private long uploadPeriodId;

    private byte[] fileData;

    public PeriodFile() {
    }

    public PeriodFile(long uploadPeriodId, byte[] fileData) {
        this.uploadPeriodId = uploadPeriodId;
        this.fileData = fileData;
    }

    public long getUploadPeriodId() {
        return uploadPeriodId;
    }

    public void setUploadPeriodId(long uploadPeriodId) {
        this.uploadPeriodId = uploadPeriodId;
    }

    public byte[] getFileData() {
        return fileData;
    }

    public void setFileData(byte[] fileData) {
        this.fileData = fileData;
    }

    @Override
    public String toString() {
        return "PeriodFile{" +
                "uploadPeriodId=" + uploadPeriodId +
                ", fileData=" + fileData +
                '}';
    }
}
