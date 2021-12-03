//
// Created by didi on 2021/10/9.
//

#ifndef INLINEHOOKTEST_INLINEHOOKLOOPER_H
#define INLINEHOOKTEST_INLINEHOOKLOOPER_H

#include <jni.h>
#include "Logger.h"

#include <xdl.h>


//_ZN7android20SimpleLooperCallback11handleEventEiiPv

static int (*handleEventOld)(int fd, int events, void *data) = nullptr;

static int handleEventProxy(int fd, int events, void *data) {
    LOGGER("handleEventProxy handleEventProxy %d  handleEventOld %p", events,handleEventOld);
    return handleEventOld(fd, events, data);
}

static void hook4Looper() {
    void *libUtils = xdl_open("libutils.so");
    void *handleEvent = xdl_sym(libUtils, "_ZN7android20SimpleLooperCallback11handleEventEiiPv");
    LOGGER("handleEvent handleEvent %p", handleEvent);
    if (handleEvent != nullptr) {
//        registerInlineHook((uint32_t) handleEvent, (uint32_t) handleEventProxy,
//                               (uint32_t **) &handleEventOld) ;

        MSHookFunction((void *) handleEvent, (void *) handleEventProxy,
                       (void **) &handleEventOld);
//        MSHookFunction((void *) func, (void *) new_so, (void **) &old_so);

    }
    //_ZN7android18WeakMessageHandler13handleMessageERKNS_7MessageE
    void *handleMessage = xdl_sym(libUtils,
                                  "_ZN7android18WeakMessageHandler13handleMessageERKNS_7MessageE");
    LOGGER("handleMessage handleMessage %p", handleMessage);
    if (handleMessage != nullptr) {

    }


//    inlineHookAll();

}


#endif //INLINEHOOKTEST_INLINEHOOKLOOPER_H

