#ifndef FFMPEG_XLOG_H
#define FFMPEG_XLOG_H

#ifdef ANDROID

#include <android/log.h>

#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,"OpenGLCamera",__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,"OpenGLCamera",__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,"OpenGLCamera",__VA_ARGS__)
#else
#define LOGD(...) printf("OpenGLCamera",__VA_ARGS__)
#define LOGI(...) printf("OpenGLCamera",__VA_ARGS__)
#define LOGE(...) printf("OpenGLCamera",__VA_ARGS__)

#endif

//类后要加;error: expected ';' after class
class Log {

};

#endif