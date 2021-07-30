#include <jni.h>
#include "Logger.h"

#include "HookProxy.h"
#include "HookThreadProxy.h"


void hook(JNIEnv *env, jobject obj) {

//    registerInlinePthreadCreate(env);

    LOGGER("fork start -------------------------");

//    exit(0);
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
        return JNI_FALSE;
    } else {
        return JNI_TRUE;
    }
}
void eeeeee(const char *tag, const char *fmt, ...) {
    va_list arg;
    va_start(arg, fmt);
    char buffer[10];
    vsnprintf(buffer,10, fmt, arg);
//    FILE *file = fopen("/test.txt", "w+");
//    vfprintf(file, fmt, arg);
    __android_log_print(ANDROID_LOG_ERROR, "TESTTEST", "test %s", buffer);

    va_list cpy;
    va_copy(cpy,arg);
//    __android_log_vprint(ANDROID_LOG_ERROR, "TESTTEST", fmt, cpy);

    va_end(cpy);

    va_list arg2;
    va_start(arg2, fmt);
//    __android_log_vprint(ANDROID_LOG_ERROR, tag, fmt, arg2);
    va_end(arg2);

}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *res) {
    eeeeee("TAG", "%s %d", "sddsfasdfasfafafffaffaasdfasdfasdfasdfasdfasdfasdfasdfasdfasffsdfsdafsadfasdfasdfasdfadsff", 34);

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