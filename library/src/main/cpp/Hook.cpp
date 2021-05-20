#include <jni.h>
#include "Logger.h"

#include "HookProxy.h"



void hook(JNIEnv *env, jobject obj) {
    hookDlOpen(env);

    registerInlinePthreadCreate(env);

    inlineHookSoTestSo(env);
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

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *res) {
    JNIEnv *env = NULL;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }

    if (registerNativeImpl(env) == 0) {
        return -1;
    } else {
        return JNI_VERSION_1_6;
    }
}
//**************************************************************************************************