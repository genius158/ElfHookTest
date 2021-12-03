package com.yan.sotest;

/**
 * @author Bevan (Contact me: https://github.com/genius158)
 * @since 2021/5/19
 */
public class SoTest {
    static {
        System.loadLibrary("sotest2");
    }

    public static native void so();
}
