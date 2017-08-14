package com.example.mmap.writer;

import sun.misc.Unsafe;

import java.io.IOException;
import java.io.RandomAccessFile;
import java.lang.reflect.Field;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;

public class MMapFileAppender implements FileAppender {
    private static final int DEFAULT_PAGE_SIZE = 4096;
    private static final int PAGE_MULTIPLIER = 30;
    private final int pageSize;
    private long index = 0;
    private long totalFileSize = 0;
    private RandomAccessFile file = null;
    private MappedByteBuffer mappedByteBuffer = null;


    private static int getSystemPageSize() throws Exception {
        Field f = Unsafe.class.getDeclaredField("theUnsafe");
        f.setAccessible(true);
        Unsafe unsafe = (Unsafe)f.get(null);
        return unsafe.pageSize();
    }

    public MMapFileAppender() {
        int tmpPageSize;
        try {
            tmpPageSize = getSystemPageSize();
        } catch (Exception e) {
            tmpPageSize = DEFAULT_PAGE_SIZE;
        }
        pageSize = tmpPageSize * PAGE_MULTIPLIER;
    }

    @Override
    public void open(String filename) throws IOException {
        file = new RandomAccessFile(filename, "rw");
        extendFileSize();
    }

    @Override
    public void write(byte[] buffer) throws IOException {
        write(buffer, 0, buffer.length);
    }

    @Override
    public void close() throws IOException {
        if (file != null) {
            file.close();
        }
    }

    private void write(byte[] buffer, long offset, long size) throws IOException {
        if (size + index <= totalFileSize) {
            // Enough space.
            mappedByteBuffer.put(buffer, (int)offset, (int)size);
            index += size;
        } else {
            long availableSpace = totalFileSize - index;
            mappedByteBuffer.put(buffer, (int)offset, (int)availableSpace);
            extendFileSize();
            write(buffer, offset + availableSpace, size - availableSpace);
        }
    }

    private void extendFileSize() throws IOException {
        index = file.length();
        totalFileSize = index + pageSize;
        file.setLength(totalFileSize);
        mappedByteBuffer = file.getChannel().map(FileChannel.MapMode.READ_WRITE, index, pageSize);
    }
}
