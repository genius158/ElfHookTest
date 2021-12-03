//
// Created by didi on 2021/10/9.
//

#ifndef INLINEHOOKTEST_INLINEHOOKLOOPER_H
#define INLINEHOOKTEST_INLINEHOOKLOOPER_H

#include <jni.h>
#include "Logger.h"

#include <xdl.h>

// fix Cause: execute-only (no-read) memory access error; likely due to data in .text.

#define _page_size 4096
#define _uintval(p)               reinterpret_cast<uintptr_t>(p)
#define _ptr(p)                   reinterpret_cast<void *>(p)
#define _align_up(x, n)           (((x) + ((n) - 1)) & ~((n) - 1))
#define _align_down(x, n)         ((x) & -(n))
#define _page_size                4096
#define _page_align(n)            _align_up(static_cast<uintptr_t>(n), _page_size)
#define _ptr_align(x)             _ptr(_align_down(reinterpret_cast<uintptr_t>(x), _page_size))
#define _make_rwx(p, n) ::mprotect(_ptr_align(p), \
                                              _page_align(_uintval(p) + n) != _page_align(_uintval(p)) ? _page_align(n) + _page_size : _page_align(n), \
                                              PROT_READ | PROT_WRITE | PROT_EXEC)


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

