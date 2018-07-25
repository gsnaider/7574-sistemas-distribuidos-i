package ar.uba.fi.contabilidapp.model;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

public class ZipBuilder {

    private ZipOutputStream zos;

    private ByteArrayOutputStream bos;

    public ZipBuilder() {
        bos = new ByteArrayOutputStream();
        zos = new ZipOutputStream(bos);
    }

    public void addToZip(String fileName, String content) throws IOException {
        zos.putNextEntry(new ZipEntry(fileName));
        zos.write(content.getBytes(), 0, content.getBytes().length);
        zos.closeEntry();
    }

    public byte[] getZip() throws IOException {
        zos.close();
        return bos.toByteArray();
    }


}
