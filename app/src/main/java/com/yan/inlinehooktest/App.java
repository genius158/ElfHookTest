package com.yan.inlinehooktest;

import android.app.Application;
import android.content.Context;

public class App extends Application {
    @Override
    protected void attachBaseContext(Context base) {
        super.attachBaseContext(base);
        CrashManager.init(this);
    }

    @Override
    public void onCreate() {
        super.onCreate();
    }
}
