#include <jni.h>
#include "Logger.h"

#include "HookProxy.h"
#include "HookThreadProxy.h"
#include "InlineHookLooper.h"

class Test {
public:
    void test();


};

void Test::test() {
    LOGGER("test test test test test test test test test test test test V test");
}

template<class T>
T square(T number) {
    return number * number;
}

static pthread_key_t gTLSKey = 22;

void hook(JNIEnv *env, jobject obj) {

//    registerInlinePthreadCreate(env);



    LOGGER("fork start -------------------------  %f", square(12.2));

//    exit(0);
    pthread_key_create(&gTLSKey, NULL);
    void *ptr = pthread_getspecific(gTLSKey);
    if (ptr == nullptr) {
        pthread_setspecific(gTLSKey, new Test());
    }
    ptr = pthread_getspecific(gTLSKey);
    if (ptr != nullptr) {
        (static_cast<Test *>(ptr))->test();
    }

}

static const JNINativeMethod sMethods[] = {
        {
                "nHook",
                "()V",
                (void *) hook
        },
};

static int registerNativeImpl(JNIEnv *env) {
    jclass clazz = env->FindClass("com/yan/hook/Hook");
    if (clazz == NULL) {
        return JNI_FALSE;
    }


    if (env->RegisterNatives(clazz, sMethods, sizeof(sMethods) / sizeof(sMethods[0])) < 0) {
        hook4Looper();
        return JNI_FALSE;
    } else {
        hook4Looper();
        return JNI_TRUE;
    }

}

void eeeeee(const char *tag, const char *fmt, ...) {
    va_list arg;
    va_start(arg, fmt);
    char buffer[10];
    vsnprintf(buffer, 10, fmt, arg);
//    FILE *file = fopen("/test.txt", "w+");
//    vfprintf(file, fmt, arg);
    __android_log_print(ANDROID_LOG_ERROR, "TESTTEST", "test %s", buffer);

    va_list cpy;
    va_copy(cpy, arg);
//    __android_log_vprint(ANDROID_LOG_ERROR, "TESTTEST", fmt, cpy);

    va_end(cpy);

    va_list arg2;
    va_start(arg2, fmt);
//    __android_log_vprint(ANDROID_LOG_ERROR, tag, fmt, arg2);
    va_end(arg2);

}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *res) {
    eeeeee("TAG", "%s %d",
           "sddsfasdfasfafafffaffaasdfasdfasdfasdfasdfasdfasdfasdfasdfasffsdfsdafsadfasdfasdfasdfadsff",
           34);

    sample_signal_register();

    thread_hook(vm);

    JNIEnv *env = NULL;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }

    if (registerNativeImpl(env) == 0) {
        return -1;
    } else {
        return JNI_VERSION_1_6;
    }
//    return JNI_VERSION_1_6;
}

//**************************************************************************************************