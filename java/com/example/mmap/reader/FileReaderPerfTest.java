package com.example.mmap.reader;

import java.text.NumberFormat;
import java.util.Locale;

public class FileReaderPerfTest {
    private static int bufferSize = 10000;
    private static long iteration = 100000;
    private static NumberFormat numberFormat = NumberFormat.getNumberInstance(Locale.US);
    private static String GetOpsPerSec(long start, long end) {
        return numberFormat.format(bufferSize * iteration * 1000 / (end - start));
    }

    public static void main(String args[]) throws Exception {
        byte[] buffer = new byte[bufferSize];
        long start, end;

        MMapFileReader mMapFileReader = new MMapFileReader();
        mMapFileReader.open("tmp_mmap");
        start = System.currentTimeMillis();
        for (int i = 0; i < iteration; i ++) {
            mMapFileReader.read(buffer, bufferSize);
        }
        mMapFileReader.close();
        end = System.currentTimeMillis();
        System.out.println("MMap File Reader Test");
        System.out.println("Run the throughput: " + GetOpsPerSec(start, end) + " ops/sec");

        VanillaFileReader vanillaFileReader = new VanillaFileReader();
        vanillaFileReader.open("tmp_vanilla");
        start = System.currentTimeMillis();
        for (int i = 0; i < iteration; i ++) {
            vanillaFileReader.read(buffer, bufferSize);
        }
        vanillaFileReader.close();
        end = System.currentTimeMillis();
        System.out.println("Vanilla File Reader Test");
        System.out.println("Run the throughput: " + GetOpsPerSec(start, end) + " ops/sec");
    }
}
