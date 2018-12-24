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

void VideoGLSurfaceRender::renderToAutoFitTexture(GLuint inputTexId, int width, int height, GLuint outputTexId){
    float textureAspectRatio = (float)height / (float)width;
    float viewAspectRatio = (float)_backingHeight / (float)_backingWidth;
    float xOffset = 0.0f;
    float yOffset = 0.0f;
    if(textureAspectRatio > viewAspectRatio){
        //Update Y Offset
        int expectedHeight = (int)((float)height*_backingWidth/(float)width+0.5f);
        yOffset = (float)(expectedHeight-_backingHeight)/(2*expectedHeight);
//		LOGI("yOffset is %.3f", yOffset);
    } else if(textureAspectRatio < viewAspectRatio){
        //Update X Offset
        int expectedWidth = (int)((float)(height * _backingWidth) / (float)_backingHeight + 0.5);
        xOffset = (float)(width - expectedWidth)/(2*width);
//		LOGI("xOffset is %.3f", xOffset);
    }

    glViewport(_backingLeft, _backingTop, _backingWidth, _backingHeight);

    if (!mInitialized) {
        LOGE("ViewRenderEffect::renderEffect effect not initialized!");
        return;
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputTexId, 0);
    checkGlError("PassThroughRender::renderEffect glFramebufferTexture2D");
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        LOGI("failed to make complete framebuffer object %x", status);
    }

    glUseProgram(mProgramId);
    const GLfloat _vertices[] = { -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f };
    glVertexAttribPointer(mVertexCoords, 2, GL_FLOAT, 0, 0, _vertices);
    glEnableVertexAttribArray(mVertexCoords);
    GLfloat texCoords[] = { xOffset, yOffset, 1.0 - xOffset, yOffset, xOffset, 1.0 - yOffset,
                            1.0 - xOffset, 1.0 - yOffset };
    glVertexAttribPointer(mTextureCoords, 2, GL_FLOAT, 0, 0, texCoords);
    glEnableVertexAttribArray(mTextureCoords);

    /* Binding the input texture */
    glActiveTexture (GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, inputTexId);
    glUniform1i(mUniformTexture, 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableVertexAttribArray(mVertexCoords);
    glDisableVertexAttribArray(mTextureCoords);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, 0, 0);
}

// 肯定是竖屏录制
void VideoGLSurfaceRender::renderToViewWithAutofit(GLuint texID, int screenWidth, int screenHeight, int texWidth, int texHeight) {
    LOGI("VideoGLSurfaceRender::renderToViewWithAutofit");
    glViewport(0, 0, screenWidth, screenHeight);

    if (!mInitialized) {
        LOGE("ViewRenderEffect::renderEffect effect not initialized!");
        return;
    }

    float cropRatio = calcCropRatio(screenWidth, screenHeight, texWidth, texHeight);

    if (cropRatio < 0)
        cropRatio = 0.0f;

    glUseProgram(mProgramId);
    static const GLfloat _vertices[] = { -1.0f, -1.0f, 1.0f, -1.0f, -1.0f, 1.0f,
                                         1.0f, 1.0f };
    glVertexAttribPointer(mVertexCoords, 2, GL_FLOAT, 0, 0, _vertices);
    glEnableVertexAttribArray(mVertexCoords);
    static const GLfloat texCoords[] = { 0.0f, cropRatio, 1.0f, cropRatio, 0.0f, 1.0f-cropRatio,
                                         1.0f, 1.0f-cropRatio };
    glVertexAttribPointer(mTextureCoords, 2, GL_FLOAT, 0, 0, texCoords);
    glEnableVertexAttribArray(mTextureCoords);

    /* Binding the input texture */
    glActiveTexture (GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
    glUniform1i(mUniformTexture, 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glDisableVertexAttribArray(mVertexCoords);
    glDisableVertexAttribArray(mTextureCoords);
    glBindTexture(GL_TEXTURE_2D, 0);
    LOGI("VideoGLSurfaceRender::renderToViewWithAutofit");
}

float VideoGLSurfaceRender::calcCropRatio(int screenWidth, int screenHeight, int texWidth, int texHeight) {
    int fitHeight = (int)((float)texHeight*screenWidth/texWidth+0.5f);

    float ratio = (float)(fitHeight-screenHeight)/(2*fitHeight);

    return ratio;
}