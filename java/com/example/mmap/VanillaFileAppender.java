package com.example.mmap;

import java.io.FileOutputStream;
import java.io.IOException;

public class VanillaFileAppender implements FileAppender {
    private FileOutputStream output = null;

    @Override
    public void open(String filename) throws IOException {
        output = new FileOutputStream(filename, true);
    }

    @Override
    public void write(byte[] buffer) throws IOException {
        output.write(buffer);
    }

    @Override
    public void close() throws IOException {
        if (output != null) {
            output.flush();
            output.close();
        }
    }
}
