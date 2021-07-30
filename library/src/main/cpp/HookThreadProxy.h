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

JavaVM *jvm;

static jclass jclassThreadInitStack;
static jmethodID jmethodRecord;
static jclass jclassThread;
static jmethodID jmethodIdCurrentThread;
static jmethodID jmethodIdGetStacktrace;

/**
 * @return 是否执行attach，主动attach的线程需要detach
 */
bool getJNIEnv(JNIEnv *&env) {
    LOGGER("getJNIEnv jvm %p", jvm);
    int getEnvStat = jvm->GetEnv((void **) &env, JNI_VERSION_1_6);
    LOGGER("getJNIEnv jvm %d", getEnvStat);
    if (getEnvStat == JNI_EDETACHED) {
        int status = jvm->AttachCurrentThread(&env, NULL);
        LOGGER("getJNIEnv jvm status %d", status);
        return true;

    } else if (getEnvStat == JNI_OK) {
    } else if (getEnvStat == JNI_EVERSION) {
    }
    return false;
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
    LOGGER("thread_call START ------------ %d  name:%s", gettid(), name);

    if (jmethodRecord != NULL) {
        JNIEnv *env;
        bool needDetach = getJNIEnv(env);
        env->CallStaticVoidMethod(jclassThreadInitStack, jmethodRecord, env->NewStringUTF(TAG),
                                  member->js_stacktrace,
                                  env->NewStringUTF(member->native_stack));
        env->DeleteGlobalRef(member->js_stacktrace);
        if (needDetach) {
            jvm->DetachCurrentThread();
        }
    }

// 直接打印
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
        bool needDetach = getJNIEnv(env);
        jobject data = env->CallStaticObjectMethod(printer, methodGetStack, args->js_stacktrace);
        jboolean jFalse = true;
        const char *stackData = env->GetStringUTFChars(static_cast<jstring>(data), &jFalse);
        env->DeleteGlobalRef(args->js_stacktrace);

        char *pc = new char[4096];
        strcpy(pc, stackData);
        args->txt_stacktrace = pc;
        LOGGER("thread_call java ---------------------------------- ");
        LOGGER("thread_call java %s", stackData);
        if (needDetach) {
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
    bool needDetach = getJNIEnv(env);

    jobject jcurrentThread = env->CallStaticObjectMethod(jclassThread, jmethodIdCurrentThread);
    LOGGER("thread_call 11111 ");
    jobject jstacktrace = env->CallObjectMethod(jcurrentThread,
                                                jmethodIdGetStacktrace);
    LOGGER("thread_call ppppppp %p", jstacktrace);
    member->js_stacktrace = env->NewGlobalRef(jstacktrace);
    if (needDetach) {
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


    jclassThreadInitStack = (jclass) env->NewGlobalRef(
            env->FindClass("com/yan/hook/ThreadInitStack"));

    jmethodRecord = env->GetStaticMethodID(jclassThreadInitStack, "record4Jni",
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