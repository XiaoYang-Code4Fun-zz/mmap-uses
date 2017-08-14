package com.example.mmap.reader;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

public class VanillaFileReader implements FileReader {
    private File file = null;
    private FileInputStream fileInputStream = null;
    private long index = 0;

    @Override
    public long open(String filename) throws IOException {
        file = new File(filename);
        fileInputStream = new FileInputStream(file);
        return file.length();
    }

    @Override
    public int read(byte[] buffer, int size) throws IOException {
        int sizeToRead = (int)Math.min((long)size, file.length() - index);
        if (sizeToRead > 0) {
            fileInputStream.read(buffer, 0, sizeToRead);
            index += sizeToRead;
        }
        return sizeToRead;
    }

    @Override
    public void close() throws IOException {
        if (fileInputStream != null) {
            fileInputStream.close();
        }
    }
}
