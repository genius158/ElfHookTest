//
// Created by yan xianwei on 2021/5/19.
//
#include <stdio.h>
#include <jni.h>
#include <string.h>
#include <stdlib.h>

#include "inlineHook.h"

#include <sys/types.h>
#include <unistd.h>

#include "Logger.h"

static unsigned long find_database_of(char *soName)//获取目标so内存基址
{
    char filename[32];
    char cmdline[256];
    sprintf(filename, "/proc/self/maps");
    LOGGER("filename = %s", filename);
    FILE *fp = fopen(filename, "r");
    unsigned long revalue = 0;
    if (fp) {
        while (fgets(cmdline, 256, fp)) //逐行读取
        {
            if (strstr(cmdline, soName) && strstr(cmdline, "r-xp"))//筛选
            {
                LOGGER("cmdline = %s", cmdline);
                char *str = strstr(cmdline, "-");
                if (str) {
                    *str = '\0';
                    char num[32];
                    sprintf(num, "0x%s", cmdline);
                    revalue = strtoul(num, NULL, 0);
                    LOGGER("revalue = %lu", revalue);
                    return revalue;
                }
            }
            memset(cmdline, 0, 256); //清零
        }
        fclose(fp);
    }
    return 0L;
}
