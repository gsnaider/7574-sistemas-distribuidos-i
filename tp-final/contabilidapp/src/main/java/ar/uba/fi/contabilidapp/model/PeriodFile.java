package ar.uba.fi.contabilidapp.model;

public class PeriodFile {

    private long uploadPeriodId;

    private String fileData;

    public PeriodFile(long uploadPeriodId, String fileData) {
        this.uploadPeriodId = uploadPeriodId;
        this.fileData = fileData;
    }

    public long getUploadPeriodId() {
        return uploadPeriodId;
    }

    public void setUploadPeriodId(long uploadPeriodId) {
        this.uploadPeriodId = uploadPeriodId;
    }

    public String getFileData() {
        return fileData;
    }

    public void setFileData(String fileData) {
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
