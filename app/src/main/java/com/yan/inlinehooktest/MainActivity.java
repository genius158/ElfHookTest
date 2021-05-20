package com.yan.inlinehooktest;

import android.os.Bundle;

import androidx.appcompat.app.AppCompatActivity;

import com.yan.hook.Hook;
import com.yan.sotest.SoTest;

public class MainActivity extends AppCompatActivity {


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        CrashManager.init(getApplication());
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        SoTest.so();
        Hook.hook();
    }

    @Override
    protected void onResume() {
        super.onResume();
        SoTest.so();
    }
}