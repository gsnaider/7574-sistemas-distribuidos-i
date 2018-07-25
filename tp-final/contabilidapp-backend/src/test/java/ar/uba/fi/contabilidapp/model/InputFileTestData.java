package ar.uba.fi.contabilidapp.model;

import com.google.common.collect.ImmutableList;

import java.math.BigDecimal;

final class InputFileTestData {

    static final String INPUT_LINE = "CR0618230405410592T01000000010086000000000000000000000000000000000000000000000000";
    static final String CLIENT_CODE = "230405410";
    static final BigDecimal AMOUNT = new BigDecimal("000001008.60");
    static final String PREFIX_CODE = "CR0618";
    static final String MIDDLE_CODE = "592T0100";
    static final String SUFFIX_CODE = "00000000000000000000000000000000000000000000000";


    static final ImmutableList<String> VALID_LINES = ImmutableList.of(
            INPUT_LINE,
            "ABCDEF123456789GHIJKLMN12345678901AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
            "xxxxxxXXXXXXXXXxxxxxxxx00000100860xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
    );

    static final ImmutableList<String> INVALID_LINES = ImmutableList.of(
            "",
            "1234567890",
            "CR0618230405410592T0100000001008600000000000000000000000000000000000000000000000",
            "ABCDEF123456789GHIJKLMN12345X78901AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
    );


}
