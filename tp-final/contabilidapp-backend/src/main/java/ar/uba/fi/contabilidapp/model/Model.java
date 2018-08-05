package ar.uba.fi.contabilidapp.model;

import ar.uba.fi.contabilidapp.entities.*;

import java.util.List;

/**
 * Interface with each of the business logic methods from the app.
 */
public interface Model {

    /**
     * Creates a new {@link UploadPeriod} and returns its id. The new period will be opened.
     */
    long startUploadPeriod() throws ContabilidappException;

    /**
     * Uploads a new {@link InputFile} for the specified {@link UploadPeriod}.
     */
    void handleFileUpload(byte[] fileData, long uploadId) throws ContabilidappException;

    /**
     * Returns the aggregated transactions file from a specific {@link UploadPeriod}.
     * <p>This will search all the {@link InputFile}s from the {@link UploadPeriod}, get the {@link Transaction}s for
     * each input file, and sum the amounts for each {@link Client}. The generated file will contain one record for each
     * client, with the sum of it's transactions.
     */
    String getAggregatedDataFile(long uploadId) throws ContabilidappException;

    /**
     * Returns a List of the ids from all open {@link UploadPeriod}s
     */
    List<Long> getOpenUploadPeriodsIds() throws ContabilidappException;

    /**
     * Returns a List of the ids from all closed {@link UploadPeriod}s
     */
    List<Long> getClosedUploadPeriodsIds() throws ContabilidappException;

    /**
     * Changes the status of an {@link UploadPeriod} to closed.
     */
    void closePeriod(long uploadId) throws ContabilidappException;

    /**
     * Uses the provided file to control a specific {@link UploadPeriod}, and returns a {@link ControlResults} instance
     * containing both the splitted transaction files and an errors file.
     * <p>This will check that the amounts from each {@link Client} from the control file match the amount of that
     * same client in that period. If there's a mismatch, or if there's a client missing either in the control file or
     * in the upload period, that will be populated as a record in the errors file. In any other case, the amount from
     * that client will be split into the original amounts from each {@link InputFile} from that period where that client
     * had a {@link Transaction}, and stored as split files.
     */
    ControlResults controlPeriod(byte[] fileData, long uploadId) throws ContabilidappException;


}
