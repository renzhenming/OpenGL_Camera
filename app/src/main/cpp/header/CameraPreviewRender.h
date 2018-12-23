//
// Created by renzhenming on 2018/12/21.
//

#ifndef rzm_opengl_camera_manager_camera_preview_CameraPreviewRender
#define rzm_opengl_camera_manager_camera_preview_CameraPreviewRender

#include <GLES2/gl2.h>
#include "GPUTextureFrameCopier.h"
#include <VideoGLSurfaceRender.h>
#include <GPUTextureFrame.h>
#include "Log.h"

class CameraPreviewRender {
protected:
    /** 从安卓相机获取到的参数 **/
    int degress;
    bool isVFlip;
    int textureWidth;
    int textureHeight;
    int cameraWidth;
    int cameraHeight;

    int textureCoordsSize;
    GLfloat* textureCoords;

    //把Camera的纹理拷贝到inputTexId
    GPUTextureFrameCopier* mCopier;

    //把outputTexId渲染到View上
    VideoGLSurfaceRender* mRenderer;

    //camera捕捉到的TextureFrame 是SamplerOES格式的
    GPUTextureFrame *mGPUTextureFrame;

    //利用mCopier转换为Sampler2D格式的inputTexId
    GLuint mInputTexId;

    //利用mProcessor转换为处理过的outputTexId
    GLuint mOutputTexId;

    //用于旋转的纹理id
    GLuint mRotateTexId;

    //暂停状态下的保留的那一帧Texture
    GLuint mPausedTexId;
    //暂停的时候增加的FilterId 当切换为普通预览的时候需要去掉
    int mMixFilterId;

    //在copy以及processor中的FBO
    GLuint FBO;

    void fillTextureCoords();
    float flip(float i);
public:
    CameraPreviewRender();
    virtual ~CameraPreviewRender();
    void init(int degress, bool isVFlip, int textureWidth, int textureHeight, int cameraWidth, int cameraHeight);
    int getCameraTextureId();
    void processFrame(float position);
};


static GLfloat CAMERA_TRIANGLE_VERTICES[8] = {
        -1.0f, -1.0f,	// 0 top left
        1.0f, -1.0f,	// 1 bottom left
        -1.0f, 1.0f,  // 2 bottom right
        1.0f, 1.0f,	// 3 top right
};

static GLfloat CAMERA_TEXTURE_NO_ROTATION[8] = {
        0.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f
};

static GLfloat CAMERA_TEXTURE_ROTATED_90[8] = {
        1.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        0.0f, 0.0f
};

static GLfloat CAMERA_TEXTURE_ROTATED_180[8] = {
        1.0f, 0.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
};

static GLfloat CAMERA_TEXTURE_ROTATED_270[8] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 1.0f
};

#endif //rzm_opengl_camera_manager_camera_preview_CameraPreviewRender
