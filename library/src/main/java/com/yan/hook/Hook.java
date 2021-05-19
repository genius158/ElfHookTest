package com.yan.hook;


import android.util.Log;

import java.util.concurrent.atomic.AtomicBoolean;

public class Hook {

    static {
        System.loadLibrary("yanhook");
    }

    static AtomicBoolean sIsRunning = new AtomicBoolean(false);

    public static void hook() {
        if (sIsRunning.compareAndSet(false, true)) {
            nHook();
        }
    }

    private static native void nHook();

    public static void printStackTrace() {
        StackTraceElement[] traceElements = Thread.currentThread().getStackTrace();
        for (int i = 0; i < traceElements.length; i++) {
            Log.e("stacktrace", traceElements[i].getMethodName() + "  " + traceElements[i].getLineNumber());
        }

    }
}