//
// Created by didi on 2021/7/6.
//

#include "ForkTest.h"
#include <jni.h>
#include <pthread.h>
#include <stdlib.h>

#include <stdio.h>
#include <unistd.h>

#include <atomic>
#include <cmath>
#include <map>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <pthread.h>

#include <android/log.h>

#ifndef LOG_TAG
#define LOG_TAG "DiminaLog"

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#endif


void * thread_run(void * arg)
{
    pthread_t newthid;

    newthid = pthread_self();
    LOGE("thread_call ForkTest.cpp this is a new thread, thread ID = %d\n",(int)arg);
    sleep(20000);
    LOGE("thread_call ForkTest.cpp  end end end end end end end end end end");

    return NULL;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_forktest_ForkTest_fork(JNIEnv *env, jclass clazz) {
    LOGE("thread_call ForkTest.cpp main thread ,ID is %d\n",pthread_self());


    pthread_t thread;
    pthread_attr_t attr;
    pthread_attr_init( &attr );
    pthread_attr_setdetachstate(&attr,1);
    pthread_create(&thread, &attr, thread_run, 0); //第二个参数决定了分离属性

}

