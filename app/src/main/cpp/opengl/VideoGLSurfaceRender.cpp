//
// Created by renzhenming on 2018/12/23.
//

#include <Log.h>
#include <OpenglUtils.h>
#include "VideoGLSurfaceRender.h"

VideoGLSurfaceRender::VideoGLSurfaceRender(){
    mVertexShader = OUTPUT_VIEW_VERTEX_SHADER;
    mFragmentShader = OUTPUT_VIEW_FRAG_SHADER;

}
VideoGLSurfaceRender::~VideoGLSurfaceRender(){

}
bool VideoGLSurfaceRender::init(int width, int height){
    LOGI("VideoGLSurfaceRender::init");
    this->_backingLeft = 0;
    this->_backingTop = 0;
    this->_backingWidth = width;
    this->_backingHeight = height;
    mProgramId = loadProgram(mVertexShader, mFragmentShader);
    if(!mProgramId){
        LOGE("Could not create VideoGLSurfaceRender program.");
        return false;
    }
    mVertexCoords = glGetAttribLocation(mProgramId,"position");
    checkGlError("glGetAttribLocation position");
    mTextureCoords = glGetAttribLocation(mProgramId,"texcoord");
    checkGlError("glGetAttribLocation texcoord");
    mUniformTexture = glGetUniformLocation(mProgramId,"yuvTexSampler");
    checkGlError("glGetUniformLocation yuvTexSampler");
    mInitialized = true;
    return true;
}