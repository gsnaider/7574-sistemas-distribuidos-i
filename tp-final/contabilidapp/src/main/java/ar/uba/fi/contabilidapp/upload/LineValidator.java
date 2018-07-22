package ar.uba.fi.contabilidapp.upload;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import static ar.uba.fi.contabilidapp.upload.LineParser.AMOUNT_LENGTH;

final class LineValidator {

    private static final String LINE_REGEX = String.format("^.{23}\\d{%d}.{47}$", AMOUNT_LENGTH);
    private static final Pattern PATTERN = Pattern.compile(LINE_REGEX);

    static boolean isValid(String line) {
        return PATTERN.matcher(line).matches();
    }

}
