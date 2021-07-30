package com.yan.hook;


import android.util.Log;

import androidx.annotation.Keep;


@Keep
public class ThreadInitStack {

    private static ThreadLocal<StringBuilder> STACK_LOCAL = new ThreadLocal<StringBuilder>() {
        @Override
        protected StringBuilder initialValue() {
            return new StringBuilder();
        }
    };

    @Keep
    protected static void record(StackTraceElement[] stackTraceElements, String nativeStack) {
        StringBuilder sb = STACK_LOCAL.get();
        if (sb == null) return;
        for (StackTraceElement ste : stackTraceElements) {
            sb.append(ste.toString()).append("\n");
        }
        sb.append("---native---").append("\n").append(nativeStack);

        Log.e("stack",sb.toString());
    }

}