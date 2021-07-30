package com.yan.hook;


import androidx.annotation.Keep;

import java.util.concurrent.atomic.AtomicBoolean;

public class Hook {

    static {
        System.loadLibrary("yhook");
    }

    static AtomicBoolean sIsRunning = new AtomicBoolean(false);

    public static void hook() {
        nHook();
    }

    private static native void nHook();

    @Keep
    protected static String getStringFromStack(StackTraceElement[] stackTraceElements) {
        StringBuilder sb = new StringBuilder();
        for (StackTraceElement ste : stackTraceElements) {
            sb.append(ste.toString()).append("\n");
        }
        return sb.toString();
    }


}