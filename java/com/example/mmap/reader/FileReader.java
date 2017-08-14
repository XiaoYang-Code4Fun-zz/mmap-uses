package com.example.mmap.reader;

import java.io.IOException;

public interface FileReader {
    /**
     * Opens file and returns file size in bytes.
     */
    long open(String filename) throws IOException;

    /**
     * Reads file content in the given byte array, and returns read buffer size.
     */
    int read(byte[] buffer, int size) throws IOException;

    /**
     * Closes file.
     */
    void close() throws IOException;
}
