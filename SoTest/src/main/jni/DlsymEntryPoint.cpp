#include "DlsymTest.h"

static void mian() {
    DlsymTestCall *tcall = (new DlsymTest())->PrintTest();
    tcall->getDlsym();
}