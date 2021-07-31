package com.yan.hook;


import androidx.annotation.Keep;

import java.util.Map;
import java.util.WeakHashMap;

/**
 * 获取线程被启动是的堆栈
 */

@Keep
public class Thread4StartStack {

    private static final WeakHashMap<Thread, StringBuilder> STACK_MAP = new WeakHashMap<>();

    private final static ThreadLocal<StringBuilder> STACK_LOCAL = new ThreadLocal<StringBuilder>() {
        @Override
        protected StringBuilder initialValue() {
            return new StringBuilder();
        }
    };

    @Keep
    public static String getAllStacks() {
        StringBuilder sb = new StringBuilder();
        for (Map.Entry<Thread, StringBuilder> entry : STACK_MAP.entrySet()) {
            Thread thread;
            if (entry == null || (thread = entry.getKey()) == null) continue;
            sb.append(thread).append("\n").append(entry.getValue()).append("\n");
        }
        return sb.toString();
    }

    /**
     * 由 JNI 调用
     *
     * @param tag native日志 TAG
     */
    @Keep
    protected static void record4Jni(String tag, StackTraceElement[] stackTraceElements, String nativeStack) {
        StringBuilder sb = STACK_LOCAL.get();
        if (sb == null) return;
        int index = 0;
        for (StackTraceElement ste : stackTraceElements) {
            //dalvik.system.VMStack.getThreadStackTrace(Native Method)
            //java.lang.Thread.getStackTrace(Thread.java:1565)
            index++;
            if (index == 1 && "dalvik.system.VMStack".equals(ste.getClassName())) {
                continue;
            } else if (index == 2 && Thread.class.getCanonicalName().equals(ste.getClassName()) && "getStackTrace".equals(ste.getMethodName())) {
                continue;
            }
            sb.append(ste.toString()).append("\n");
        }
        sb.append("---native---").append("\n").append(nativeStack.replaceAll(".+libyhook.so[^" + tag + "]+", ""));
        synchronized (STACK_MAP) {
            STACK_MAP.put(Thread.currentThread(), sb);
        }
    }

}