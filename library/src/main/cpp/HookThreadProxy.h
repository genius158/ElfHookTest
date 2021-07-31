#ifndef HookThreadProxy
#define HookThreadProxy

#include <pthread.h>

#include <xdl.h>
#include <xhook.h>
#include <uwind.h>
#include <sys/prctl.h>
#include <unistd.h>
#include "Logger.h"
#include <stdio.h>
#include <string.h>

#define GET_JNI_ENV_STATUS_NORMAL 0
#define GET_JNI_ENV_STATUS_NEED_DETACH -2
#define GET_JNI_ENV_STATUS_ERROR -3

JavaVM *jvm;

static jclass jclassThread4StartStack;
static jmethodID jmethodRecord;
static jclass jclassThread;
static jmethodID jmethodIdCurrentThread;
static jmethodID jmethodIdGetStacktrace;

/**
 * @return 0 Normal -2 线程需要detach -3 其他
 */
int getJNIEnv(JNIEnv *&env) {
    int getEnvStat = jvm->GetEnv((void **) &env, JNI_VERSION_1_6);
    LOGGER("getJNIEnv jvm %d", getEnvStat);
    if (getEnvStat == JNI_EDETACHED) {
        int status = jvm->AttachCurrentThread(&env, NULL);
        LOGGER("getJNIEnv jvm status %d", status);
        if (status == 0) {
            return GET_JNI_ENV_STATUS_NEED_DETACH;
        } else {
            return GET_JNI_ENV_STATUS_ERROR;
        }

    } else if (getEnvStat == JNI_OK) {
        return GET_JNI_ENV_STATUS_NORMAL;
    }
    return GET_JNI_ENV_STATUS_ERROR;
}


static int (*thread_create_original)(pthread_t *thread_out, pthread_attr_t const *attr,
                                     void *(*start_routine)(void *), void *arg) = pthread_create;


/* 原线程执行函数和参数 */
struct ThreadCall_Args {
    void *(*thread_call_original)(void *);

    void *arg;
    char *native_stack;
    jobject js_stacktrace;
    char *txt_stacktrace = NULL;
};

static void logJavaStack(ThreadCall_Args *args);


static char *TAG = "TAGT";


static void log(char *stack) {
    /* 获取第一个子字符串 */
    char *token = strtok(stack, "\n");
    /* 继续获取其他的子字符串 */
    while (token != NULL) {
        LOGGER("thread_call %s", token);
        token = strtok(NULL, "\n");
    }
}

static void *thread_call(void *arg) {
    struct ThreadCall_Args *member = (struct ThreadCall_Args *) arg;
    char name[32];
    prctl(PR_GET_NAME, name);

    if (jmethodRecord != NULL && jclassThread4StartStack != NULL && jmethodRecord != NULL) {
        JNIEnv *env;
        int status = getJNIEnv(env);
        if (status != GET_JNI_ENV_STATUS_ERROR){
            env->CallStaticVoidMethod(jclassThread4StartStack, jmethodRecord, env->NewStringUTF(TAG),
                                      member->js_stacktrace,
                                      env->NewStringUTF(member->native_stack));
            env->DeleteGlobalRef(member->js_stacktrace);
        }

        if (status == GET_JNI_ENV_STATUS_NEED_DETACH) {
            jvm->DetachCurrentThread();
        }
    }

// 直接打印
//    LOGGER("thread_call START ------------ %d  name:%s", gettid(), name);
//    log(member->native_stack);
//    LOGGER("thread_call  \n\n\n\n END");

    // recall original
    member->thread_call_original(member->arg);
    return NULL;
}

jclass printer = NULL;
jmethodID methodGetStack = NULL;

static void logJavaStack(ThreadCall_Args *args) {
    if (args->txt_stacktrace != NULL) {
        LOGGER("thread_call java ---------------------------------- ");
        LOGGER("thread_call java %s", args->txt_stacktrace);
    } else {
        JNIEnv *env;
        int status = getJNIEnv(env);
        if (status != GET_JNI_ENV_STATUS_ERROR){
            jobject data = env->CallStaticObjectMethod(printer, methodGetStack, args->js_stacktrace);
            jboolean jFalse = true;
            const char *stackData = env->GetStringUTFChars(static_cast<jstring>(data), &jFalse);
            env->DeleteGlobalRef(args->js_stacktrace);

            char *pc = new char[4096];
            strcpy(pc, stackData);
            args->txt_stacktrace = pc;
            LOGGER("thread_call java ---------------------------------- ");
            LOGGER("thread_call java %s", stackData);
        }
        if (status == GET_JNI_ENV_STATUS_NEED_DETACH) {
            jvm->DetachCurrentThread();
        }
    }
}


static int thread_create_proxy(pthread_t *thread_out, pthread_attr_t const *attr,
                               void *(*start_routine)(void *), void *arg) {
    LOGGER("xunwind_tag -------------------------------------------------");
    char *stack = xunwind_cfi_get(-1, -1, NULL, TAG);

    struct ThreadCall_Args *member = (struct ThreadCall_Args *) malloc(
            sizeof(struct ThreadCall_Args));

    member->thread_call_original = start_routine;
    member->arg = arg;
    member->native_stack = stack;
    member->txt_stacktrace = NULL;

    JNIEnv *env;
    int status = getJNIEnv(env);
    if (status != GET_JNI_ENV_STATUS_ERROR) {
        jobject jcurrentThread = env->CallStaticObjectMethod(jclassThread, jmethodIdCurrentThread);
        LOGGER("thread_call 11111 ");
        jobject jstacktrace = env->CallObjectMethod(jcurrentThread,
                                                    jmethodIdGetStacktrace);
        LOGGER("thread_call ppppppp %p", jstacktrace);
        member->js_stacktrace = env->NewGlobalRef(jstacktrace);
    }
    if (status == GET_JNI_ENV_STATUS_NEED_DETACH) {
        jvm->DetachCurrentThread();
    }
    return thread_create_original(thread_out, attr, thread_call, member);
}

void thread_hook(JavaVM *vm) {
    jvm = vm;
    JNIEnv *env = nullptr;
    getJNIEnv(env);
    if (!env)return;

    printer = (jclass) env->NewGlobalRef(env->FindClass("java/util/Arrays"));
    methodGetStack = env->GetStaticMethodID(printer, "toString",
                                            "([Ljava/lang/Object;)Ljava/lang/String;");


    jclassThread4StartStack = (jclass) env->NewGlobalRef(
            env->FindClass("com/yan/hook/Thread4StartStack"));

    jmethodRecord = env->GetStaticMethodID(jclassThread4StartStack, "record4Jni",
                                           "(Ljava/lang/String;[Ljava/lang/StackTraceElement;Ljava/lang/String;)V");

    jclassThread = (jclass) env->NewGlobalRef(env->FindClass("java/lang/Thread"));
    jmethodIdCurrentThread = (jmethodID) env->GetStaticMethodID(jclassThread, "currentThread",
                                                                "()Ljava/lang/Thread;");
    jmethodIdGetStacktrace = (jmethodID) env->GetMethodID(jclassThread, "getStackTrace",
                                                          "()[Ljava/lang/StackTraceElement;");

    xhook_register(".*\\.so$", "pthread_create",
                   (void *) &thread_create_proxy,
                   (void **) &thread_create_original);

    xhook_enable_debug(0);
    xhook_refresh(0);
}

#endif