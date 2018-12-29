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
    //这个纹理，只分配内存，而不去填充它。纹理填充会在渲染到帧缓冲的时候去做
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

    //创建一个缓存对象
    glGenFramebuffers(1,&FBO);
    checkGlError("glGenFramebuffers");

    //生成的纹理是无维度的；第一次绑定至的纹理目标的维度为自己的维度
    glGenTextures(1,&mRotateTexId);
    checkGlError("glGenTextures mRotateTexId");
    //指定纹理要绑定到的目标，必须是GL_TEXTURE_2D或者GL_TEXTURE_CUBE_MAP。
    //所以这里是二维纹理,将纹理名绑定至当前活动纹理单元目标。当一个纹理与目标绑定时，该目标之前的绑定关系将自动被打破
    glBindTexture(GL_TEXTURE_2D,mRotateTexId);
    checkGlError("glBindTexture mRotateTexId");
    //The target texture, which must be either GL_TEXTURE_1D or GL_TEXTURE_2D.
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    if(degress == 90 || degress == 270){
        //参数
        //   target：The target texture. Must be GL_TEXTURE_2D.
        //   level: The level-of-detail number. Level 0 is the base image level. Level n is the n th mipmap reduction image.
        //   internalformat: The number of color components in the texture
        //   border:The width of the border. Must be either 0 or 1.
        //   type:The data type of the pixel data. The following symbolic values are accepted: GL_UNSIGNED_BYTE,
        //        GL_BYTE, GL_BITMAP, GL_UNSIGNED_SHORT, GL_SHORT, GL_UNSIGNED_INT, GL_INT, and GL_FLOAT.
        //纹理图像使用glteximage2d定义,函数指定二维纹理图像
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

//glBindFramebuffer离屏渲染
void CameraPreviewRender::processFrame(float position){
    LOGI("CameraPreviewRender::processFrame start");
    //绑定创建的缓存对象FBO（FrameBufferObject），绑定一个帧缓存对象后，
    // 所有对OpenGL的操作都会针对这个帧缓存对象执行
    glBindFramebuffer(GL_FRAMEBUFFER,FBO);
    checkGlError("glBindFramebuffer FBO");

    //degress = 90说明是竖屏后置摄像头
    //degress = 270说明是竖屏前置摄像头
    //cameraHeight和cameraWidth是获取到的手机支持的预览宽高，且宽>高

    //如果你想将你的屏幕渲染到一个更小或更大的纹理上，你需要（在渲染到你的帧缓冲之前）
    // 再次调用glViewport，使用纹理的新维度作为参数，否则只有一小部分的纹理或屏幕会被渲染到这个纹理上。
    if(degress == 90 || degress == 270){
        //所以竖屏状态下，设置cameraHeight为预览界面的宽
        glViewport(0,0,cameraHeight,cameraWidth);
    }else{
        //横屏
        glViewport(0,0,cameraWidth,cameraHeight);
    }
    GLfloat* vertexCoords = this->getVertexCoords();
    mCopier->renderWithCoords(mGPUTextureFrame,mRotateTexId,vertexCoords,textureCoords);

    int rotateWidth = cameraWidth;
    int rotateHeight = cameraHeight;

    if(degress == 90 || degress == 270){
        rotateWidth = cameraHeight;
        rotateHeight = cameraWidth;
    }
    mRenderer->renderToAutoFitTexture(mRotateTexId,rotateWidth,rotateHeight,mInputTexId);
    //为了让所有的渲染操作对主窗口产生影响我们必须通过绑定为0来使默认帧缓冲被激活
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    LOGI("CameraPreviewRender::processFrame success");
}

void CameraPreviewRender::drawToViewWithAutofit(int videoWidth, int videoHeight, int texWidth, int texHeight){
    mRenderer->renderToViewWithAutofit(mInputTexId, videoWidth, videoHeight, texWidth, texHeight);
}

GLfloat* CameraPreviewRender::getVertexCoords(){
    return CAMERA_TRIANGLE_VERTICES;
}


int CameraPreviewRender::getCameraTextureId(){
    if (mGPUTextureFrame) {
        return mGPUTextureFrame->getDecodeTexId();
    }
    return -1;
}

void CameraPreviewRender::setDegress(int degress, bool isVFlip){
    this->degress = degress;
    this->isVFlip = isVFlip;
    this->fillTextureCoords();
}

void CameraPreviewRender::dealloc(){
    if(mCopier){
        mCopier->destroy();
        delete mCopier;
        mCopier = NULL;
    }
    if(mRenderer){
        LOGI("delete mRenderer..");
        mRenderer->dealloc();
        delete mRenderer;
        mRenderer = NULL;
    }
    if (mInputTexId) {
        LOGI("delete mInputTexId ..");
        glDeleteTextures(1, &mInputTexId);
    }
    if(mOutputTexId){
        LOGI("delete mOutputTexId ..");
        glDeleteTextures(1, &mOutputTexId);
    }
    if(mPausedTexId){
        LOGI("delete mPausedTexId..");
        glDeleteTextures(1, &mPausedTexId);
    }
    if(FBO){
        LOGI("delete FBO..");
        glDeleteBuffers(1, &FBO);
    }
    if(textureCoords){
        delete[] textureCoords;
        textureCoords = NULL;
    }
    LOGI("CameraPreviewRender::dealloc success");
}

CameraPreviewRender::~CameraPreviewRender(){

}