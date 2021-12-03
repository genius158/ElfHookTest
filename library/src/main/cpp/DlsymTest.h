
// fix Cause: execute-only (no-read) memory access error; likely due to data in .text.

#ifndef DlsymTest_h
#define DlsymTest_h

#include "xdl.h"

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


struct DlsymTestCall {
    DlsymTestCall *dlsymTest;
};


class DlsymTestCall2 {
private:
    DlsymTestCall2 *dlsymTest;

public:

    virtual   DlsymTestCall2 *getDlsym();
};

struct DlsymTest {
    int left;
    int top;
    int right;
    int bottom;

    int test;
};


void Dlsym() {
    //ArchivePathEntry: pathPrefix="", path="/lib/armeabi-v7a/libdlsymtest.so"
    void *handle = xdl_open("libdlsymtest.so");

    //0000758c T _ZN13DlsymTestCall8getDlsymEv DlsymTestCall::getDlsym()
    //0000757c T _ZN13DlsymTestCallC1EPS_
    //0000757c T _ZN13DlsymTestCallC2EPS_
    //000074fc T _ZN9DlsymTest9PrintTestEv DlsymTest::PrintTest()
    //000074cc T _ZN9DlsymTestC1Ev
    //000074cc T _ZN9DlsymTestC2Ev



    void *getDlsym = xdl_sym(handle, "_ZN9DlsymTest9PrintTestEv");
    DlsymTestCall *test2 = ((DlsymTestCall *(*)()) getDlsym)();

    DlsymTestCall2* tc22 = reinterpret_cast<DlsymTestCall2 *>(test2);

    //_ZN9DlsymTest6newInsEv
    //DlsymTest::newIns()
    void *newIns = xdl_sym(handle, "_ZN9DlsymTest6newInsEv");
    DlsymTest *dlsymTest = ((DlsymTest *(*)()) newIns)();

    void *newInsC = xdl_sym(handle, "_ZN9DlsymTest7newInsCEv");
    DlsymTest dlsymTestC = ((DlsymTest (*)()) newInsC)();

    //_ZN9DlsymTestC2Ev
    //DlsymTest::DlsymTest()
    void *init = xdl_sym(handle, "_ZN9DlsymTestC1Ev");
    if (init == nullptr) {
        init = xdl_sym(handle, "_ZN9DlsymTestC12v");
    }
    if (init != nullptr) {
        DlsymTest test = ((DlsymTest (*)()) init)();

    }

}

#endif