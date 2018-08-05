package ar.uba.fi.contabilidapp.model;

/**
 * Generic exception from the ContabilidApp system.
 */
public class ContabilidappException extends Exception {

    public ContabilidappException(String s) {
        super(s);
    }

    public ContabilidappException(String s, Exception e) {
        super(s, e);
    }
}
