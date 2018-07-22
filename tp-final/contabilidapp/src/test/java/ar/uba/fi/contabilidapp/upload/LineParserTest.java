package ar.uba.fi.contabilidapp.upload;

import ar.uba.fi.contabilidapp.upload.model.Client;
import ar.uba.fi.contabilidapp.upload.model.Transaction;
import org.junit.Test;

import static ar.uba.fi.contabilidapp.upload.InputFileTestData.*;
import static org.assertj.core.api.Assertions.assertThat;
import static org.assertj.core.api.Assertions.assertThatExceptionOfType;

public class LineParserTest {

    @Test
    public void testParseLine_nullInput_throwsNullPointerException() {
        assertThatExceptionOfType(NullPointerException.class).isThrownBy(() -> {
            LineParser.parseLine(null);
        });
    }

    @Test
    public void testParseLine_invalidLine_throwsIllegalArgumentException() {
        assertThatExceptionOfType(IllegalArgumentException.class).isThrownBy(() -> {
            LineParser.parseLine("");
        });
    }

    @Test
    public void testParseLine_validLine_returnsTransaction() {
        Transaction transaction = LineParser.parseLine(INPUT_LINE);
        assertThat(transaction.getAmount()).isEqualTo(AMOUNT);
        Client client = transaction.getClient();
        assertThat(client.getClientCode()).isEqualTo(CLIENT_CODE);
        assertThat(client.getPrefixCode()).isEqualTo(PREFIX_CODE);
        assertThat(client.getMiddleCode()).isEqualTo(MIDDLE_CODE);
        assertThat(client.getSuffixCode()).isEqualTo(SUFFIX_CODE);

    }


}
