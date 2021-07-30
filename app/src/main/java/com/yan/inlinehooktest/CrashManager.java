package com.yan.inlinehooktest;

import android.app.Application;
import android.content.pm.PackageManager;
import android.util.Log;

import org.json.JSONObject;

import java.io.File;
import java.io.FileWriter;

//import xcrash.ICrashCallback;
//import xcrash.TombstoneParser;
//import xcrash.XCrash;

/**
 * @author wangjieming
 * @date 2019-07-31.
 * <p>
 * 管理
 */
public class CrashManager {

    static final String TAG = "CrashManager";


    private Application application;


    private static boolean inited = false;
    private static CrashManager crashManager;

    public static void init(Application application) {
        if (inited) {
            return;
        }
        inited = true;
        crashManager = new CrashManager(application);
        crashManager.initXCrash();
    }

    private CrashManager(Application application) {
        this.application = application;
    }


    private void initXCrash() {
//        ICrashCallback callback = new ICrashCallback() {
//            @Override
//            public void onCrash(String logPath, String emergency) {
//                Log.d(TAG, "log path: " + (logPath != null ? logPath : "(null)") + ", emergency: " + (
//                        emergency != null ? emergency : "(null)"));
//
//                if (BuildConfig.DEBUG) debug(logPath, emergency);
//            }
//        };

        int version = 0;
        try {
            version = application.getPackageManager()
                    .getPackageInfo(application.getPackageName(), 0).versionCode;
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
        }

//        XCrash.InitParameters parameters = new XCrash.InitParameters()
//                .setAppVersion(String.valueOf(version))
//                .setJavaRethrow(true)
//                .setJavaLogCountMax(10)
//                .setJavaDumpAllThreadsCountMax(10)
//                .setJavaCallback(callback)
//                .setNativeRethrow(true)
//                .setNativeLogCountMax(10)
//                .setNativeDumpAllThreadsCountMax(10)
//                .setNativeCallback(callback)
//                .setPlaceholderCountMax(3)
//                .setPlaceholderSizeKb(512)
//                .setLogFileMaintainDelayMs(1000);
//
//        XCrash.init(application, parameters);
    }


    private void debug(String logPath, String emergency) {
//        Log.d(TAG, "debug debug debug   " + logPath + "    " + emergency);
//        FileWriter writer = null;
//        try {
//            File debug = new File(application.getFilesDir() + "/tombstonesss/" + System.currentTimeMillis() + "debug.json");
//            if (!debug.getParentFile().exists()) debug.getParentFile().mkdirs();
//            debug.createNewFile();
//            writer = new FileWriter(debug, false);
//            writer.write(new JSONObject(TombstoneParser.parse(logPath, emergency)).toString());
//        } catch (Exception e) {
//            Log.d(TAG, "debug failed", e);
//        } finally {
//            if (writer != null) {
//                try {
//                    writer.flush();
//                    writer.close();
//                } catch (Exception ignored) {
//                }
//            }
//        }
    }
}
