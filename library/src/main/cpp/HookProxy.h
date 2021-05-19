#ifndef HOOK_PROXY_H
#define HOOK_PROXY_H

#include <pthread.h>
#include <xdl.h>
#include <xhook.h>

#ifdef __arm__

#include "inlineHook.h"
#include <SubstrateHook.h>

#else

#include <And64InlineHook.hpp>

#endif

#include "Logger.h"
#include "SoTool.c"


#define SO_LOAD_SYMBOL_O_1 "__loader_android_dlopen_ext"
#define SO_LOAD_SYMBOL_O_2 "__loader_dlopen"

#define SO_LOAD_SYMBOL_N_1 "android_dlopen_ext"
#define SO_LOAD_SYMBOL_N_2 "dlopen"

#define SO_LOAD_SYMBOL "dlopen"

static void *(*dlopen_origin_O)(const char *, int, const void *) = nullptr;

static void *(*dlopen_origin_N)(const char *, int) = nullptr;

static void *(*dlopen_origin)(const char *, int) = nullptr;

static void *
(*android_dlopen_origin_O)(const char *, int, const void *, const void *) = nullptr;

static void *
(*android_dlopen_origin_N)(const char *, int, const void *) = nullptr;

static void *(*dlopen_ext_N)(const char *, int, const void *, const void *) = nullptr;


static void *dlopen_proxy_O(const char *filename, int flags, const void *caller_addr) {
    void *result = dlopen_origin_O(filename, flags, caller_addr);
    LOGGER("dlopen_proxy_O  %s", filename);
    return result;
}

static void *dlopen_proxy_N(const char *filename, int flags) {
    void *result = dlopen_origin_N(filename, flags);
    LOGGER("dlopen_proxy_N  %s", filename);
    return result;
}

static void *dlopen_proxy(const char *filename, int flags) {
    void *result = dlopen_origin(filename, flags);
    LOGGER("dlopen_proxy  %s", filename);
    return result;
}

static void dynamicHookSoTest(const char *filename);

static void inlineHookSoTestSo2(void *func);

static void *
android_dlopen_proxy_O(const char *filename,
                       int flags,
                       const void *extinfo,
                       const void *caller_addr) {
    LOGGER("android_dlopen_proxy_O  %s", filename);
    void *result = android_dlopen_origin_O(filename, flags, extinfo, caller_addr);

    if (strstr(filename, "libsotest.so")) {
        dynamicHookSoTest(filename);
    }

    return result;
}

// 动态HOOK
static void dynamicHookSoTest(const char *filename) {
    LOGGER("dynamicHookSoTest dynamicHookSoTest ");

    void *handler = xdl_open(filename);
    if (handler == NULL)return;
    void *Mehthod_So = xdl_sym(handler, "_ZN6SoTest2soEv");
    if (Mehthod_So != NULL) {
        // 主动调用一次 libsotest.so so方法
        ((void (*)(void)) Mehthod_So)();

        inlineHookSoTestSo2(Mehthod_So);
    }

}

static void *
android_dlopen_proxy_N(const char *filename,
                       int flags,
                       const void *extinfo) {
    LOGGER("android_dlopen_proxy_N  %s", filename);
    void *result = android_dlopen_origin_N(filename, flags, extinfo);
    return result;
}

static const void *sSoLoad_O[][3] = {
        {
                SO_LOAD_SYMBOL_O_1,
                (void *) android_dlopen_proxy_O,
                (void **) &android_dlopen_origin_O
        },
        {
                SO_LOAD_SYMBOL_O_2,
                (void *) dlopen_proxy_O,
                (void **) &dlopen_origin_O
        },
};

static const void *sSoLoad_N[][3] = {
        {
                SO_LOAD_SYMBOL_N_1,
                (void *) android_dlopen_proxy_N,
                (void **) &android_dlopen_origin_N
        },
        {
                SO_LOAD_SYMBOL_N_2,
                (void *) dlopen_proxy_N,
                (void **) &dlopen_origin_N
        },
};

static const void *sSoLoad[][3] = {
        {
                SO_LOAD_SYMBOL,
                (void *) dlopen_proxy,
                (void **) &dlopen_origin
        },
};

void hookDlOpen(JNIEnv *env) {
    int api_level = android_get_device_api_level();
    if (api_level >= __ANDROID_API_O__) {
        for (int i = 0; i < sizeof(sSoLoad_O) / sizeof(sSoLoad_O[0]); i++) {
            xhook_register(".*\\.so$", (const char *) sSoLoad_O[i][0],
                           (void *) sSoLoad_O[i][1], (void **) sSoLoad_O[i][2]);
        }
    } else if (api_level >= __ANDROID_API_N__) {
        for (int i = 0; i < sizeof(sSoLoad_N) / sizeof(sSoLoad_N[0]); i++) {
            xhook_register(".*\\.so$", (const char *) sSoLoad_N[i][0],
                           (void *) sSoLoad_N[i][1], (void **) sSoLoad_N[i][2]);
        }
    } else {
        for (int i = 0; i < sizeof(sSoLoad) / sizeof(sSoLoad[0]); i++) {
            xhook_register(".*\\.so$", (const char *) sSoLoad[i][0],
                           (void *) sSoLoad[i][1], (void **) sSoLoad[i][2]);
        }
    }

    xhook_enable_debug(1);
    xhook_refresh(0);
}


static int (*thread_create_original)(pthread_t *thread_out, pthread_attr_t const *attr,
                                     void *(*start_routine)(void *), void *arg) = pthread_create;

static int thread_create_proxy(pthread_t *thread_out, pthread_attr_t const *attr,
                               void *(*start_routine)(void *), void *arg) {
    LOGGER("thread_create_proxy thread_create_proxy thread_create_proxy");

    return thread_create_original(thread_out, attr, start_routine, arg);
}

void registerInlinePthreadCreate(JNIEnv *env) {
#ifdef __arm__
    LOGGER("registerInlinePthreadCreate __arm__");

    MSHookFunction((void *) pthread_create, (void *) thread_create_proxy,
                   (void **) &thread_create_original);

//    if(registerInlineHook((uint32_t) pthread_create, (uint32_t) thread_create_proxy,
//                       (uint32_t **) &thread_create_original)!=ELE7EN_OK){
//            LOGGER("registerInlineHook fialed");
//
//    }
//    inlineHookAll();

#else
    LOGGER("registerInlinePthreadCreate 64  support until api 28");

    A64HookFunction((void *) pthread_create, (void *) thread_create_proxy,
                    (void **) &thread_create_original);
#endif
}


unsigned long funcSo = NULL;

void (*old_so)() = NULL;

void new_so() {
    LOGGER("new_so new_so new_so");
    LOGGER("old_so %p", old_so);
//    old_so();
}

//////////////////// start 尝试hook libsotest.so 的 so 方法 基于地址、偏移量 //////////////////////

void inlineHookSoTestSo(JNIEnv *env) {
    LOGGER("inlineHookSoTestSo  old_so %p", old_so);

#ifdef __arm__
    unsigned long base = find_database_of("libsotest.so");
    if (base > 0L) {
        funcSo = base + 0x7048 + 1;

        MSHookFunction((void *) funcSo, (void *) new_so, (void **) &old_so);
    }

#else
#endif

}
//////////////////// end 尝试hook libsotest.so 的 so 方法 //////////////////////


void inlineHookSoTestSo2(void *func) {
    LOGGER("inlineHookSoTestSo2 old_so %p", old_so);

#ifdef __arm__
    MSHookFunction((void *) func, (void *) new_so, (void **) &old_so);
#else
#endif

}

#endif