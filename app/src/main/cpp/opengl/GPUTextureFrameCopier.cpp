#include <GPUTextureFrameCopier.h>
//
// Created by renzhenming on 2018/12/22.
//

GPUTextureFrameCopier::GPUTextureFrameCopier(){
    mVertexShader = NO_FILTER_VERTEX_SHADER;
    mFragmentShader = GPU_FRAME_FRAGMENT_SHADER;
}
GPUTextureFrameCopier::~GPUTextureFrameCopier(){

}

bool GPUTextureFrameCopier::init(){
    LOGI("GPUTextureFrameCopier::init");
    mProgramId = loadProgram(mVertexShader,mFragmentShader);
    if (!mProgramId) {
        LOGE("Could not create program.");
        return false;
    }
    //获取顶点坐标
    mVertexCoords = glGetAttribLocation(mProgramId,"vPosition");
    checkGlError("glGetAttribLocation vPosition");
    //获取材质坐标
    mTextureCoords = glGetAttribLocation(mProgramId,"vTexCords");
    checkGlError("glGetAttribLocation vTexCords");

    mUniformTexture = glGetUniformLocation(mProgramId,"yuvTexCoords");
    checkGlError("glGetAttribLocation yuvTexSampler");

    mUniformTexMatrix = glGetUniformLocation(mProgramId, "texMatrix");
    checkGlError("glGetUniformLocation mUniformTexMatrix");

    mUniformTransforms = glGetUniformLocation(mProgramId, "trans");
    checkGlError("glGetUniformLocation mUniformTransforms");

    mInitialized = true;
    LOGI("GPUTextureFrameCopier::init success");
    return true;
}

void GPUTextureFrameCopier::renderWithCoords(TextureFrame *textureFrame, GLuint texId, GLfloat *vertexCoords,
                              GLfloat *textureCoords){

}