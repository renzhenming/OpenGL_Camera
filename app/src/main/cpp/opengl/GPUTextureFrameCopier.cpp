#include <GPUTextureFrameCopier.h>
#include <matrix.h>
//
// Created by renzhenming on 2018/12/22.
//

GPUTextureFrameCopier::GPUTextureFrameCopier() {
    mVertexShader = NO_FILTER_VERTEX_SHADER;
    mFragmentShader = GPU_FRAME_FRAGMENT_SHADER;
}

GPUTextureFrameCopier::~GPUTextureFrameCopier() {

}

bool GPUTextureFrameCopier::init() {
    LOGI("GPUTextureFrameCopier::init");
    mProgramId = loadProgram(mVertexShader, mFragmentShader);
    if (!mProgramId) {
        LOGE("Could not create program.");
        return false;
    }
    //获取顶点坐标
    mVertexCoords = glGetAttribLocation(mProgramId, "vPosition");
    checkGlError("glGetAttribLocation vPosition");
    //获取材质坐标
    mTextureCoords = glGetAttribLocation(mProgramId, "vTexCords");
    checkGlError("glGetAttribLocation vTexCords");

    mUniformTexture = glGetUniformLocation(mProgramId, "yuvTexCoords");
    checkGlError("glGetAttribLocation yuvTexSampler");

    mUniformTexMatrix = glGetUniformLocation(mProgramId, "texMatrix");
    checkGlError("glGetUniformLocation mUniformTexMatrix");

    mUniformTransforms = glGetUniformLocation(mProgramId, "trans");
    checkGlError("glGetUniformLocation mUniformTransforms");

    mInitialized = true;
    LOGI("GPUTextureFrameCopier::init success");
    return true;
}

void GPUTextureFrameCopier::renderWithCoords(TextureFrame *textureFrame, GLuint texId,
                                             GLfloat *vertexCoords,
                                             GLfloat *textureCoords) {
    LOGI("GPUTextureFrameCopier::renderWithCoords");
    glBindTexture(GL_TEXTURE_2D, texId);
    checkGlError("GPUTextureFrameCopier glBindTexture");
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId, 0);
    checkGlError("glFramebufferTexture2D");
    glUseProgram(mProgramId);
    if (!mInitialized) {
        return;
    }
    glVertexAttribPointer(mVertexCoords, 2, GL_FLOAT, GL_FALSE, 0, vertexCoords);
    glEnableVertexAttribArray(mVertexCoords);
    glVertexAttribPointer(mTextureCoords, 2, GL_FLOAT, GL_FALSE, 0, textureCoords);
    glEnableVertexAttribArray(mTextureCoords);
    /* Binding the input texture */
    textureFrame->bindTexture(&mUniformTexture);

    float texTransMatrix[4 * 4];
    matrixSetIdentityM(texTransMatrix);
    glUniformMatrix4fv(mUniformTexMatrix, 1, GL_FALSE, (GLfloat *) texTransMatrix);

    float rotateMatrix[4 * 4];
    matrixSetIdentityM(rotateMatrix);
    glUniformMatrix4fv(mUniformTransforms, 1, GL_FALSE, (GLfloat *) rotateMatrix);

    glDrawArrays(GL_TRIANGLE_STRIP,0,4);

    glDisableVertexAttribArray(mVertexCoords);
    glDisableVertexAttribArray(mTextureCoords);
    glBindTexture(GL_TEXTURE_2D,0);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,0,0);
}