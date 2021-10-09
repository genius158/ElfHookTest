#ifndef LOGGER_H
#define LOGGER_H

#include <android/log.h>

#define LOGGER(fmt, ...) __android_log_print(ANDROID_LOG_ERROR, "HOOK", fmt, ##__VA_ARGS__)
static void test(){
}
#endif