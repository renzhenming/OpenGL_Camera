//
// Created by renzhenming on 2018/12/21.
//

#include <CameraPreviewRender.h>
#include <cstring>
#include "Log.h"
#include "CameraPreviewRender.h"

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

    mCopier = new GPUTextureFrameCopier();
    mCopier->init();
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

CameraPreviewRender::~CameraPreviewRender(){

}