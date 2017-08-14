package com.example.mmap.reader;

import sun.misc.Unsafe;

import java.io.IOException;
import java.io.RandomAccessFile;
import java.lang.reflect.Field;
import java.nio.MappedByteBuffer;
import java.nio.channels.FileChannel;

public class MMapFileReader implements FileReader {
    private static final int DEFAULT_PAGE_SIZE = 4096;
    private static final int PAGE_MULTIPLIER = 20;
    private final int pageSize;
    private long index = 0;
    private int pageIndex = -1;
    private long totalFileSize = 0;
    private RandomAccessFile file = null;
    private MappedByteBuffer mappedByteBuffer = null;


    private static int getSystemPageSize() throws Exception {
        Field f = Unsafe.class.getDeclaredField("theUnsafe");
        f.setAccessible(true);
        Unsafe unsafe = (Unsafe)f.get(null);
        return unsafe.pageSize();
    }

    public MMapFileReader() {
        int tmpPageSize;
        try {
            tmpPageSize = getSystemPageSize();
        } catch (Exception e) {
            tmpPageSize = DEFAULT_PAGE_SIZE;
        }
        pageSize = tmpPageSize * PAGE_MULTIPLIER;
    }

    @Override
    public long open(String filename) throws IOException {
        file = new RandomAccessFile(filename, "r");
        loadNextPage(pageSize);
        totalFileSize = file.length();
        return totalFileSize;
    }

    @Override
    public int read(byte[] buffer, int size) throws IOException {
        return read(buffer, 0, size);
    }

    @Override
    public void close() throws IOException {
        if (file != null) {
            file.close();
        }
    }

    private void loadNextPage(long sizeToLoad) throws IOException {
        // Assuming index is already after the end of current page.
        mappedByteBuffer = file.getChannel().map(FileChannel.MapMode.READ_ONLY, index, sizeToLoad);
        pageIndex ++;
    }

    private int read(byte[] buffer, int offset, int size) throws IOException {
        if (index + size > totalFileSize) {
            size = (int)(totalFileSize - index);
        }
        long nextPageStart = Math.min((pageIndex + 1) * pageSize, totalFileSize);
        if (index + size > nextPageStart) {
            // Need to load next page.
            int availableSpace = (int)(nextPageStart - index);
            mappedByteBuffer.get(buffer, offset, availableSpace);
            index += availableSpace;
            int remaining = size - availableSpace;
            loadNextPage(Math.min(totalFileSize - index, pageSize));
            read(buffer, offset + availableSpace, remaining);
        } else {
            mappedByteBuffer.get(buffer, offset, size);
            index += size;
        }
        return size;
    }
}
