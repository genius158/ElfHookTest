package com.yan.inlinehooktest;

import android.os.Bundle;
import android.os.Looper;
import android.util.Log;
import android.webkit.WebView;

import androidx.appcompat.app.AppCompatActivity;

import com.example.forktest.ForkTest;
import com.yan.hook.Hook;
import com.yan.sotest.SoTest;

import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.InputStreamReader;
import java.io.PrintWriter;

public class MainActivity extends AppCompatActivity {


    @Override
    protected void onCreate(Bundle savedInstanceState) {
//        CrashManager.init(getApplication());
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        System.loadLibrary("dlsymtest");

        SoTest.so();
        ForkTest.load();
        Hook.hook();

        WebView web = findViewById(R.id.web);
//        web.loadUrl("https://baidu.com");

        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    Thread.sleep(10000);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }).start();
    }

    @Override
    public void onDetachedFromWindow() {
        super.onDetachedFromWindow();
    }

    @Override
    protected void onResume() {
        super.onResume();
        ForkTest.fork();

        callback("asdfasdfsafsdfsdfdsfsdfas" + Math.random());
        Log.e("v8ErrorMsg", v8ErrorMsg.toString());

        ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
        PrintWriter printWriter = new PrintWriter(outputStream);
        new Exception().printStackTrace(printWriter);
        byte[] stack = outputStream.toByteArray();
        printWriter.close();
    }

    private static final int MAX_LENGTH = 60;
    private static StringBuilder v8ErrorMsg = new StringBuilder();

    public void callback(String msg) {
        if (msg == null) return;
        int preNextLen = v8ErrorMsg.length() + msg.length();
        // 控制数据 最大 20k多一点
        if (preNextLen > MAX_LENGTH) {
            v8ErrorMsg.delete(0, preNextLen - MAX_LENGTH);
        }
        v8ErrorMsg.append(msg).append("\n");
    }
}