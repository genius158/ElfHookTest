package com.yan.inlinehooktest;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import com.yan.hook.Hook;
import com.yan.sotest.SoTest;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
//        SoTest.so();
        Hook.hook();
    }

    @Override
    protected void onResume() {
        super.onResume();
        SoTest.so();
    }
}