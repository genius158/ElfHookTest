#ifndef DlsymTesth
#define DlsymTesth

#include "DlsymTestCall.h"
#include "StructTest.h"

class DlsymTest : public Test {
public:
    DlsymTest();

    static DlsymTest *newIns();

    static DlsymTest newInsC();

    DlsymTestCall *PrintTest();

private:
    int int1 ;
private:
    enum {
        USAGE_SW_READ_NEVER = 1,
        USAGE_SW_READ_RARELY = 2,
        USAGE_SW_READ_OFTEN = 3,
        USAGE_SW_READ_MASK = 4,
    };

    int test;
};

#endif