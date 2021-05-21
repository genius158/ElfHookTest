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
                LOGGER("str = %s", str);
                LOGGER("cmdline = %s", cmdline);
                if (str) {
                    *str = '\0';
                    char num[32];
                    sprintf(num, "0x%s", cmdline);
                    LOGGER("num %s" ,num);
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

/**
 * 通过/proc/$pid/maps，获取模块基址
 */
unsigned long find_database_of2(pid_t pid, char *soName) {
    FILE *mapFile = NULL;
    unsigned long base = 0L;
    char mapFilePath[256];
    char lineTxt[1024];

    LOGGER("Pid is %d\n", pid);

    if (pid < 0) {
        sprintf(mapFilePath, "/proc/self/maps");
    } else {
        sprintf(mapFilePath, "/proc/%d/maps", pid);
    }

    mapFile = fopen(mapFilePath, "r");
    if (NULL == mapFile) {
        return base;
    }

    LOGGER("Get map.\n");

    //循环遍历maps文件，找到相应模块，截取地址信息
    while (fgets(lineTxt, sizeof(lineTxt), mapFile) != NULL) {
        if (strstr(lineTxt, soName)) {
            LOGGER("%s\n", lineTxt);
            char *address = strtok(lineTxt, "-");
            if (address) {
                base = strtoul(address, NULL, 16);

                if (base == 0x8000) {
                    base = 0;
                }
                break;
            }
        }
    }
    fclose(mapFile);
    return base;
}