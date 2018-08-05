package ar.uba.fi.contabilidapp.entities;

/**
 * Class used to store the result files from a control.
 */
public class ControlResults {

    private String errorsFile;

    private byte[] zipFiles;

    public ControlResults() {}

    public ControlResults(String errorsFile, byte[] zipFiles) {
        this.errorsFile = errorsFile;
        this.zipFiles = zipFiles;
    }

    public boolean hasErrorFiles() {
        return errorsFile != null && !errorsFile.isEmpty();
    }
    public boolean hasSplitFiles() {
        return zipFiles != null && zipFiles.length > 0;
    }

    public String getErrorsFile() {
        return errorsFile;
    }

    public void setErrorsFile(String errorsFile) {
        this.errorsFile = errorsFile;
    }

    public byte[] getZipFiles() {
        return zipFiles;
    }

    public void setZipFiles(byte[] zipFiles) {
        this.zipFiles = zipFiles;
    }


}
