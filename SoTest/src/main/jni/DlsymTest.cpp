#include "DlsymTest.h"

#include <android/log.h>

DlsymTest::DlsymTest() {
    __android_log_print(6, "DlsymTest", " init<>  init<>  init<>");
    left = 1;
    top = 2;
    right = 3;
    bottom = 4;
    int1 = -1;
    test = 12345;
}

DlsymTest *DlsymTest::newIns() {
    return new DlsymTest();
}

DlsymTest DlsymTest::newInsC() {
    return {};
}

DlsymTestCall *DlsymTest::PrintTest() {
    __android_log_print(6, "DlsymTest", "PrintTest PrintTest ");
    auto *call1 = new DlsymTestCall(nullptr);
    __android_log_print(6, "DlsymTest", "PrintTest %p", call1);

    auto *call2 = new DlsymTestCall(call1);
    return call2;
}
