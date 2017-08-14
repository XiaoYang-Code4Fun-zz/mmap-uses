package com.example.mmap.writer;

import java.text.NumberFormat;
import java.util.Locale;

public class FileAppenderPerfTest {
    private static int bufferSize = 10000;
    private static long iteration = 100000;
    private static NumberFormat numberFormat = NumberFormat.getNumberInstance(Locale.US);
    private static String GetOpsPerSec(long start, long end) {
        return numberFormat.format(bufferSize * iteration * 1000 / (end - start));
    }

    public static void main(String args[]) throws Exception {
        byte[] buffer = new byte[bufferSize];

        MMapFileAppender mMapFileAppender = new MMapFileAppender();
        mMapFileAppender.open("tmp_mmap");
        long start = System.currentTimeMillis();
        for (int i = 0; i < iteration; i ++) {
            mMapFileAppender.write(buffer);
        }
        mMapFileAppender.close();
        long end = System.currentTimeMillis();
        System.out.println("MMap File Appender Test");
        System.out.println("Run the throughput: " + GetOpsPerSec(start, end) + " ops/sec");

        VanillaFileAppender vanillaFileAppender = new VanillaFileAppender();
        vanillaFileAppender.open("tmp_vanilla");
        start = System.currentTimeMillis();
        for (int i = 0; i < iteration; i ++) {
            vanillaFileAppender.write(buffer);
        }
        vanillaFileAppender.close();
        end = System.currentTimeMillis();
        System.out.println("Vanilla File Appender Test");
        System.out.println("Run the throughput: " + GetOpsPerSec(start, end) + " ops/sec");
    }
}
