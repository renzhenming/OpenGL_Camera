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
    //这个纹理，只分配内存，而不去填充它。纹理填充会在渲染到帧缓冲的时候去做(末尾的NULL表示我们只预分配空间，而不实际加载纹理)
    glTexImage2D(GL_TEXTURE_2D,0,format,(GLsizei)textureWidth,(GLsizei)textureHeight,0,format,GL_UNSIGNED_BYTE,0);
    //相当于解绑,恢复OpenGL默认状态的代码,这样下边的操作就不再针对这个texture，所以创建一个新的texture之前，我们做一步
    //恢复状态的操作，OpenGL是状态机，当使用glBindTexture绑定一张纹理后，如果不再绑定新的纹理，则OpenGL之后的操作都会对应此纹理
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

    //创建一个帧缓冲对象,如果不显示的创建帧缓冲对象，那么其实我们是在系统默认的帧缓冲对象中操作的
    //帧缓冲包括OpenGL使用的颜色缓冲区(color buffer)、深度缓冲区(depth buffer)、模板缓冲区(stencil buffer)等缓冲区
    //默认的帧缓冲区由窗口系统创建,就是目前我们一直使用的绘图命令的作用对象，称之为窗口系统提供的帧缓冲区(window-system-provided framebuffer)
    //OpenGL也允许我们手动创建一个帧缓冲区，并将渲染结果重定向到这个缓冲区。在创建时允许我们自定义帧缓冲区的一些特性，
    // 这个自定义的帧缓冲区，称之为应用程序帧缓冲区(application-created framebuffer object ),使用自定义的帧缓冲区，
    // 可以实现镜面，离屏渲染，以及很酷的后处理效果，同默认的帧缓冲区一样，自定义的帧缓冲区也包含颜色缓冲区、深度和模板缓冲区
    //FBO中包含两种类型的附加图像(framebuffer-attachable): 纹理图像和RenderBuffer图像(texture images and renderbuffer images)。
    // 附加纹理时OpenGL渲染到这个纹理图像，在着色器中可以访问到这个纹理对象；附加RenderBuffer时，OpenGL执行离屏渲染(offscreen rendering)。
    //FBO可以附加多个缓冲区，而且可以灵活地在缓冲区中切换

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
        //函数中后面三个参数format、type、data表示的是内存中图像像素的信息，包括格式，类型和指向内存的指针。
        // 而internalFormat表示的是OpenGL内存存储纹理的格式，表示的是纹理中颜色成分的格式
        //上面填写的纹理格式GL_RGBA，以及GL_UNSIGNED_BYTE表示纹理包含红绿蓝和透明值，并且每个成分用无符号字节表示。
        // cameraWidth,cameraHeight表示我们分配的纹理大小，注意这个纹理需要和我们渲染的屏幕大小保持一致，
        // 如果需要绘制与屏幕不一致的纹理，使用glViewport函数进行调节。可以看后边，的确调节了
        // （processFrame方法中）
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
    // When a framebuffer object is bound, the previous binding
    // is automatically broken.
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
    //值零保留，缓冲区设置为零有效地解除绑定先前绑定的
    // 任何缓冲区对象，并恢复该缓冲区对象目标的客户端内存使用（如果该目标支持的话）
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