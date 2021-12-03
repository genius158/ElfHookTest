#ifndef DlsymTestCallH
#define DlsymTestCallH


class DlsymTestCall {
private:
    DlsymTestCall *dlsymTest;

public:
    DlsymTestCall( DlsymTestCall *);

    DlsymTestCall *getDlsym();
};
#endif