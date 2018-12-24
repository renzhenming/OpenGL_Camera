//
// Created by renzhenming on 2018/12/23.
//

#include <Log.h>
#include "GPUTextureFrame.h"


GPUTextureFrame::GPUTextureFrame(){
    LOGI("GPUTextureFrame instance init");
    mTextureId = 0;
}
GPUTextureFrame::~GPUTextureFrame(){

}

bool GPUTextureFrame::createTexture(){
    LOGI("GPUTextureFrame::createTexture begin");
    mTextureId = 0;
    int result = initTexture();
    if(result < 0){
        LOGE("GPUTextureFrame init texture failed");
        if (mTextureId) {
            glDeleteTextures(1, &mTextureId);
            return false;
        }
    }
    LOGI("GPUTextureFrame::createTexture success");
    return true;
}

int GPUTextureFrame::initTexture(){
    glGenTextures(1,&mTextureId);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES,mTextureId);
    if (checkGlError("glBindTexture GL_TEXTURE_EXTERNAL_OES")) {
        return -1;
    }
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    if (checkGlError("glTexParameteri GL_TEXTURE_MAG_FILTER")) {
        return -1;
    }
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    if (checkGlError("glTexParameteri GL_TEXTURE_MIN_FILTER")) {
        return -1;
    }
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    if (checkGlError("glTexParameteri GL_TEXTURE_WRAP_S")) {
        return -1;
    }
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    if (checkGlError("glTexParameteri GL_TEXTURE_WRAP_S")) {
        return -1;
    }
    return 1;
}

bool GPUTextureFrame::bindTexture(GLint* uniformSamplers) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES,mTextureId);
    glUniform1i(*uniformSamplers,0);
    return true;
}

void GPUTextureFrame::updateTexImage(){

}
void GPUTextureFrame::dealloc(){

}