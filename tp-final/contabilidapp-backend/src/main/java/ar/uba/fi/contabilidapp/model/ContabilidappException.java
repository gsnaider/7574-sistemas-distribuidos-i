package ar.uba.fi.contabilidapp.model;

public class ContabilidappException extends Exception {

    public ContabilidappException(String s) {
        super(s);
    }

    public ContabilidappException(String s, Exception e) {
        super(s, e);
    }
}