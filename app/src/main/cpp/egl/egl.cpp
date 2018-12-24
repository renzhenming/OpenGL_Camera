#include "egl.h"
#include "Log.h"

//构造方法
EGL::EGL(){
    LOGI("EGL instance created");
}

void EGL::release(){

}

bool EGL::init(){
    return this->init(NULL);
}

bool EGL::init(EGLContext sharedContext){
    EGLint configNum;

    if((display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY){
        LOGE("eglGetDisplay() returned error %d", eglGetError());
        return false;
    }
    if(!eglInitialize(display,0,0)){
        LOGE("eglInitialize() returned error %d", eglGetError());
		return false;
    }
    const EGLint attribs[] = { EGL_BUFFER_SIZE, 32, EGL_ALPHA_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE,
                                8, EGL_RED_SIZE, 8, EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,EGL_SURFACE_TYPE,
                                EGL_WINDOW_BIT, EGL_NONE };
    if(!eglChooseConfig(display,attribs,&configure,1,&configNum)){
        LOGE("eglChooseConfig() returned error %d", eglGetError());
		release();
		return false;
    }

    EGLint eglContextAttributes[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE };
    if(!(context = eglCreateContext(display,configure, sharedContext == NULL? EGL_NO_CONTEXT :
                sharedContext,eglContextAttributes))){
        LOGE("eglCreateContext() returned error %d", eglGetError());
        release();
        return false;
    }

    //pfneglPresentationTimeANDROID = (PFNEGLPRESENTATIONTIMEANDROID)eglGetProcAddress("eglPresentationTimeANDROID");
    //if (!pfneglPresentationTimeANDROID) {
    //	LOGE("eglPresentationTimeANDROID is not available!");
    //}
    return true;
}

EGLSurface EGL::createWindowSurface(ANativeWindow* window){
    EGLSurface surface = NULL;
    EGLint format;
    if (!eglGetConfigAttrib(display,configure,EGL_NATIVE_VISUAL_ID,&format)){
        LOGE("eglGetConfigAttrib() returned error %d", eglGetError());
        release();
        return surface;
    }
    ANativeWindow_setBuffersGeometry(window,0,0,format);
    if (!(surface = eglCreateWindowSurface(display,configure,window,0))){
        LOGE("eglCreateWindowSurface() returned error %d", eglGetError());
    }
    return surface;
}

bool EGL::makeCurrent(EGLSurface eglSurface){
    return eglMakeCurrent(display, eglSurface, eglSurface, context);
}

bool EGL::swapBuffers(EGLSurface eglSurface){
    return eglSwapBuffers(display, eglSurface);
}

//析构方法
EGL::~EGL(){

}