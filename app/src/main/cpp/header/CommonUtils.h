//
// Created by renzhenming on 2018/12/23.
//

#ifndef OPENGLCAMERA_COMMONUTILS_H
#define OPENGLCAMERA_COMMONUTILS_H

#include <sys/time.h>

static inline long getCurrentTime()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

#endif //OPENGLCAMERA_COMMONUTILS_H
