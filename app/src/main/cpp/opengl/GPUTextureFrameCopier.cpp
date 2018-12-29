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

    //创建一个帧缓冲的纹理和创建普通纹理差不多
    glBindTexture(GL_TEXTURE_2D, texId);
    checkGlError("GPUTextureFrameCopier glBindTexture");
    //把纹理texId附加到帧缓冲上 帧缓冲区数据渲染到纹理上
    //参数解析：target：创建的帧缓冲类型的目标（绘制、读取或两者都有，GL_READ_FRAMEBUFFER GL_DRAW_FRAMEBUFFER GL_FRAME_BUFFER）
    //        attachment：附加的附件的类型。现在附加的是一个颜色附件。需要注意，最后的那个0是暗示可以附加1个以上颜色的附件
    //                    (GL_DEPTH_ATTACHMENT GL_STENCIL_ATTACHMENT GL_DEPTH_STENCIL_ATTACHMENT)
    //        textarget：附加的纹理类型。
    //        texture：附加的实际纹理。
    //        level：Mipmap level。设置为0。
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

    //设置Shader内Uniform变量的值
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