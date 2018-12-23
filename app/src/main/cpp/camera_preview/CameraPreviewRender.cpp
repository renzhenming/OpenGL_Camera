//
// Created by renzhenming on 2018/12/21.
//

#include <CameraPreviewRender.h>
#include <cstring>


CameraPreviewRender::CameraPreviewRender(){
    textureCoordsSize = 8;
}

void CameraPreviewRender::init(int degress, bool isVFlip, int textureWidth, int textureHeight, int cameraWidth, int cameraHeight){
    LOGI("CameraPreviewRender::init()");
    this->degress = degress;
    this->isVFlip = isVFlip;
    this->textureWidth = textureWidth;
    this->textureHeight = textureHeight;
    this->cameraWidth = cameraWidth;
    this->cameraHeight = cameraHeight;

    textureCoords = new GLfloat[textureCoordsSize];
    this->fillTextureCoords();

    //把Camera的纹理拷贝到inputTexId
    mCopier = new GPUTextureFrameCopier();
    mCopier->init();

    //把outputTexId渲染到View上
    mRenderer = new VideoGLSurfaceRender();
    //textureWidth textureHeight是设置的需要的宽高，
    // cameraWidth cameraHeight是设置给相机的预览宽高
    mRenderer->init(textureWidth,textureHeight);

    mGPUTextureFrame = new GPUTextureFrame();
    mGPUTextureFrame->createTexture();

    glGenTextures(1,&mInputTexId);
    checkGlError("glGenTextures mInputTexId");
    glBindTexture(GL_TEXTURE_2D,mInputTexId);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    GLint format = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D,0,format,(GLsizei)textureWidth,(GLsizei)textureHeight,0,format,GL_UNSIGNED_BYTE,0);
    //相当于解绑
    glBindTexture(GL_TEXTURE_2D,0);

    glGenTextures(1,&mOutputTexId);
    glBindTexture(GL_TEXTURE_2D,mOutputTexId);
    checkGlError("glGenTextures mOutputTexId");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D,0,format,(GLsizei)textureWidth,(GLsizei)textureHeight,0,format,GL_UNSIGNED_BYTE,0);
    glBindTexture(GL_TEXTURE_2D,0);

    glGenFramebuffers(1,&FBO);
    checkGlError("glGenFramebuffers");

    glGenTextures(1,&mRotateTexId);
    checkGlError("glGenTextures mRotateTexId");
    glBindTexture(GL_TEXTURE_2D,mRotateTexId);
    checkGlError("glBindTexture mRotateTexId");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    if(degress == 90 || degress == 270){
        glTexImage2D(GL_TEXTURE_2D,0,format,cameraHeight,cameraWidth,0,format,GL_UNSIGNED_BYTE,0);
    }else{
        glTexImage2D(GL_TEXTURE_2D,0,format,cameraWidth,cameraHeight,0,format,GL_UNSIGNED_BYTE,0);
    }
    glBindTexture(GL_TEXTURE_2D,0);

    mMixFilterId = -1;
    glGenTextures(1,&mPausedTexId);
    checkGlError("glGenTextures mPausedTexId");
    glBindTexture(GL_TEXTURE_2D,mPausedTexId);
    checkGlError("glBindTexture mPausedTexId");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D,0,format,(GLsizei)textureWidth,(GLsizei)textureHeight,0,format,GL_UNSIGNED_BYTE,0);
    glBindTexture(GL_TEXTURE_2D,0);

    LOGI("CameraPreviewRender::init success");
}

void CameraPreviewRender::fillTextureCoords(){
    GLfloat* tempTextureCoords;
    switch (degress) {
        case 90:
            tempTextureCoords = CAMERA_TEXTURE_ROTATED_90;
            break;
        case 180:
            tempTextureCoords = CAMERA_TEXTURE_ROTATED_180;
            break;
        case 270:
            tempTextureCoords = CAMERA_TEXTURE_ROTATED_270;
            break;
        case 0:
        default:
            tempTextureCoords = CAMERA_TEXTURE_NO_ROTATION;
            break;
    }
    memcpy(textureCoords, tempTextureCoords, textureCoordsSize * sizeof(GLfloat));

    //如果是前置摄像头，需要做镜像处理
    if(isVFlip){
        textureCoords[1] = flip(textureCoords[1]);
        textureCoords[3] = flip(textureCoords[3]);
        textureCoords[5] = flip(textureCoords[5]);
        textureCoords[7] = flip(textureCoords[7]);
    }
}

float CameraPreviewRender::flip(float i){
    if (i == 0.0f) {
        return 1.0f;
    }
    return 0.0f;
}


int CameraPreviewRender::getCameraTextureId(){
    if (mGPUTextureFrame) {
        return mGPUTextureFrame->getDecodeTexId();
    }
    return -1;
}

CameraPreviewRender::~CameraPreviewRender(){

}