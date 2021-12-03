


#include "DlsymTest.h"
#include "DlsymTestCall.h"

DlsymTestCall::DlsymTestCall(DlsymTestCall *dlsym) {
    dlsymTest = dlsym;
}

DlsymTestCall *DlsymTestCall::getDlsym() {
    return dlsymTest;
}
