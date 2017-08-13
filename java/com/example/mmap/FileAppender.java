package com.example.mmap;

import java.io.IOException;

public interface FileAppender {
    /**
     * Opens the file to operate on.
     */
    void open(String filename) throws IOException;

    /**
     * Writes buffer to the file.
     */
    void write(byte buffer[]) throws IOException;

    /**
     * Closes the file.
     */
    void close() throws IOException;
}