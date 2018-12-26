#include <CameraPreviewRender.h>
#include <CommonUtils.h>
#include "CameraPreviewController.h"


//构造函数
CameraPreviewController::CameraPreviewController() {
    LOGI("CameraPreviewController instance created");
    cameraFacingId = CAMERA_FACING_FRONT;
    window = NULL;
    previewSurface = EGL_NO_SURFACE;
    startTime = -1;
    queue = new MessageQueue("CameraPreviewHandler message queue");
    handler = new CameraPreviewHandler(this, queue);
}

void CameraPreviewController::initEGLContext(ANativeWindow *window, JavaVM *jvm, jobject obj,
                                             int screenWidth, int screenHeight,
                                             int cameraFacingId) {
    LOGI("initEGLContext");
    this->jvm = jvm;
    this->obj = obj;
    this->window = window;
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->cameraFacingId = cameraFacingId;

    //发送EGL创建的消息，进入消息队列
    handler->postMessage(new Message(MSG_EGL_THREAD_CREATE));
    //开启线程从队列中取消息
    pthread_create(&_threadId, 0, threadStartCallback, this);
}

void *CameraPreviewController::threadStartCallback(void *myself) {
    LOGI("pthread start");
    CameraPreviewController *previewController = (CameraPreviewController *) myself;
    previewController->processMessage();
    pthread_exit(0);
    LOGI("pthread exit");
    return 0;
}

void CameraPreviewController::processMessage() {
    bool renderingEnabled = true;
    while (renderingEnabled) {
        Message *msg = NULL;
        if (queue->dequeueMessage(&msg, true) > 0) {
            if (MESSAGE_QUEUE_LOOP_QUIT_FLAG == msg->execute()) {
                renderingEnabled = false;
            }
            delete msg;
        }
    }
}

//开始初始化EGL
//Fatal signal 5 (SIGTRAP), code 1 in tid xxxx有返回值却不返回
bool CameraPreviewController::initialize() {

    //初始化EGL
    LOGI("CameraPreviewController::initialize()");
    egl = new EGL();
    egl->init();
    previewSurface = egl->createWindowSurface(window);
    egl->makeCurrent(previewSurface);
    LOGI("egl initialize success");

    //初始化render用于绘制纹理
    renderer = new CameraPreviewRender();
    LOGI("CameraPreviewRender initialize success");

    //配置并返回相机参数
    this->configCamera();

    renderer->init(degress,cameraFacingId == CAMERA_FACING_FRONT,textureWidth,textureHeight,cameraWidth,cameraHeight);

    this->startCameraPreview();

    isInSwitchingCamera = false;
    return true;
}

void CameraPreviewController::startCameraPreview() {

    JNIEnv *env;
    if(jvm->AttachCurrentThread(&env,NULL) != JNI_OK){
        LOGE("CameraPreviewController AttachCurrentThread() failed");
        return;
    }
    if(env == NULL){
        LOGE("get JNIEnv failed");
        return;
    }
    jclass jclazz = env->GetObjectClass(obj);
    if(jclazz != NULL){
        jmethodID  startPreviewMethodId = env->GetMethodID(jclazz,"startPreviewFromNative","(I)V");
        if(startPreviewMethodId != NULL){
            env->CallVoidMethod(obj,startPreviewMethodId,renderer->getCameraTextureId());
        }
    }
    if(jvm->DetachCurrentThread() != JNI_OK){
        LOGE("CameraPreviewController DetachCurrentThread failed");
        return;
    }
    LOGI("CameraPreviewController::startCameraPreview success");
}

void CameraPreviewController::notifyFrameAvailable(){
    if (handler && !isInSwitchingCamera)
        handler->postMessage(new Message(MSG_RENDER_FRAME));
}

void CameraPreviewController::renderFrame(){
    if (egl != NULL && !isInSwitchingCamera) {
        if (startTime == -1) {
            startTime = getCurrentTime();
        }
        float position = ((float) (getCurrentTime() - startTime)) / 1000.0f;
        LOGI("CameraPreviewController::renderFrame time:%f",position);
        this->processVideoFrame(position);
        if (previewSurface != EGL_NO_SURFACE) {
            this->draw();
        }
    }
}

void CameraPreviewController::draw(){
    egl->makeCurrent(previewSurface);
    renderer->drawToViewWithAutofit(screenWidth, screenHeight, textureWidth, textureHeight);
    if (!egl->swapBuffers(previewSurface)) {
        LOGE("eglSwapBuffers(previewSurface) returned error %d", eglGetError());
    }
}

void CameraPreviewController::processVideoFrame(float position){
    updateTexImage();
    renderer->processFrame(position);
}

void CameraPreviewController::updateTexImage(){
    JNIEnv *env;
    if (jvm->AttachCurrentThread(&env, NULL) != JNI_OK) {
        LOGE("CameraPreviewController updateTexImage AttachCurrentThread() failed");
        return;
    }
    if (env == NULL) {
        LOGI("getJNIEnv failed");
        return;
    }
    jclass jcls = env->GetObjectClass(obj);
    if (NULL != jcls) {
        jmethodID updateTexImageCallback = env->GetMethodID(jcls, "updateTexImageFromNative", "()V");
        if (NULL != updateTexImageCallback) {
            env->CallVoidMethod(obj, updateTexImageCallback);
        }
    }
    if (jvm->DetachCurrentThread() != JNI_OK) {
        LOGE("CameraPreviewController updateTexImage DetachCurrentThread() failed");
    }
}


//配置相机
void CameraPreviewController::configCamera(){
    LOGI("configCamera begin");
    JNIEnv *env;
    if (jvm->AttachCurrentThread(&env,NULL) != JNI_OK){
        LOGE("configCamera: AttachCurrentThread() failed");
        return;
    }
    if (env == NULL) {
        LOGI("getJNIEnv failed");
        return;
    }

    //获取到CameraPreviewManager.java的class
    jclass jclazz = env->GetObjectClass(obj);
    if (jclazz != NULL){
        jmethodID configCameraMethodId = env->GetMethodID(jclazz,"configCamera","(I)Lcom/rzm/opengl_camera/camera/configure/CameraConfigure;");
        if (configCameraMethodId != NULL){
            jobject cameraConfigObject = env->CallObjectMethod(obj,configCameraMethodId,cameraFacingId);
            jclass cameraConfigJClass = env->GetObjectClass(cameraConfigObject);

            //获取角度
            jmethodID getDegressMethodId = env->GetMethodID(cameraConfigJClass,"getDegress","()I");
            degress = env->CallIntMethod(cameraConfigObject,getDegressMethodId);

            //获取摄像头id
            jmethodID getFacingMethodId = env->GetMethodID(cameraConfigJClass,"getCameraFacingId","()I");
            cameraFacingId = env->CallIntMethod(cameraConfigObject,getFacingMethodId);

            //获取textureView宽度
            jmethodID getWidthMethodId = env->GetMethodID(cameraConfigJClass,"getTextureWidth","()I");
            cameraWidth = env->CallIntMethod(cameraConfigObject,getWidthMethodId);

            //获取高度
            jmethodID getHeightMethodId = env->GetMethodID(cameraConfigJClass,"getTextureHeight","()I");
            cameraHeight = env->CallIntMethod(cameraConfigObject,getHeightMethodId);

            textureWidth = 360;
            textureHeight = 640;

            LOGI("camera : {%d, %d}", cameraWidth, cameraHeight);
            LOGI("Texture : {%d, %d}", textureWidth, textureHeight);
            LOGI("degress %d:", degress);
            LOGI("cameraFacingId %d: ", cameraFacingId);
        }
    }

    if(jvm->DetachCurrentThread() != JNI_OK){
        LOGE("configCamera: DetachCurrentThread() failed");
        return;
    }

    LOGI("configCamera success");
}

/** 4:切换摄像头转向 **/
void CameraPreviewController::switchCameraFacing(){
    LOGI("CameraPreviewController::switchCameraFacing");
    isInSwitchingCamera = true;
    /*notify render thread that camera has changed*/
    if(cameraFacingId == CAMERA_FACING_BACK){
        cameraFacingId = CAMERA_FACING_FRONT;
    } else{
        cameraFacingId = CAMERA_FACING_BACK;
    }
    if (handler)
        handler->postMessage(new Message(MSG_SWITCH_CAMERA_FACING));
}

void CameraPreviewController::switchCamera(){
    this->releaseCamera();
    this->configCamera();
    renderer->setDegress(degress, cameraFacingId == CAMERA_FACING_FRONT);
    this->startCameraPreview();
    isInSwitchingCamera = false;
    LOGI("CameraPreviewController::switchCamera success");
}

void CameraPreviewController::releaseCamera(){
    LOGI("CameraPreviewController::releaseCamera");
    JNIEnv *env;
    if (jvm->AttachCurrentThread(&env, NULL) != JNI_OK) {
        LOGE("%s: AttachCurrentThread() failed", __FUNCTION__);
        return;
    }
    if (env == NULL) {
        LOGI("getJNIEnv failed");
        return;
    }
    jclass jcls = env->GetObjectClass(obj);
    if (NULL != jcls) {
        jmethodID releaseCameraCallback = env->GetMethodID(jcls, "releaseCameraFromNative", "()V");
        if (NULL != releaseCameraCallback) {
            env->CallVoidMethod(obj, releaseCameraCallback);
        }
    }
    if (jvm->DetachCurrentThread() != JNI_OK) {
        LOGE("%s: DetachCurrentThread() failed", __FUNCTION__);
        return;
    }
}

void CameraPreviewController::destroyEGLContext() {
    if (handler){
        handler->postMessage(new Message(MSG_EGL_THREAD_EXIT));
        handler->postMessage(new Message(MESSAGE_QUEUE_LOOP_QUIT_FLAG));
    }
    pthread_join(_threadId, 0);
    if (queue) {
        queue->abort();
        delete queue;
        queue = NULL;
    }
    delete handler;
    handler = NULL;
    LOGI("CameraPreviewController destroyEGLContext success");
}

void CameraPreviewController::destroy() {
    this->destroyPreviewSurface();
    if(renderer){
        renderer->dealloc();
        delete renderer;
        renderer = NULL;
    }
    this->releaseCamera();
    egl->release();
    egl = NULL;
    this->deleteGlobalRef();
    LOGI("CameraPreviewController::destroy success");
}

void CameraPreviewController::destroyPreviewSurface() {
    if (EGL_NO_SURFACE != previewSurface) {
        egl->releaseSurface(previewSurface);
        previewSurface = EGL_NO_SURFACE;
        if(window){
            ANativeWindow_release(window);
            window = NULL;
        }
    }
}

void CameraPreviewController::deleteGlobalRef() {
    int status = 0;
    bool needAttach = false;
    JNIEnv *env;
    status = jvm->GetEnv((void **) (&env), JNI_VERSION_1_4);
    if (status < 0) {
        if (jvm->AttachCurrentThread(&env, NULL) != JNI_OK) {
            LOGE("CameraPreviewController deleteGlobalRef: AttachCurrentThread() failed");
            return;
        }
        needAttach = true;
    }
    if (obj) {
        env->DeleteGlobalRef(obj);
    }
    if (needAttach) {
        if (jvm->DetachCurrentThread() != JNI_OK) {
            LOGE("CameraPreviewController deleteGlobalRef DetachCurrentThread() failed");
            return;
        }
    }
}

void CameraPreviewController::createWindowSurface(ANativeWindow* window) {
    LOGI("CameraPreviewController::createWindowSurface");
    if(this->window == NULL){
        this->window = window;
        if (handler)
            handler->postMessage(new Message(MSG_EGL_CREATE_PREVIEW_SURFACE));
    }
}

void CameraPreviewController::destroyWindowSurface() {
    LOGI("CameraPreviewController::destroyWindowSurface");
    if (handler)
        handler->postMessage(new Message(MSG_EGL_DESTROY_PREVIEW_SURFACE));
}

void CameraPreviewController::createPreviewSurface() {
    previewSurface = egl->createWindowSurface(window);
    egl->makeCurrent(previewSurface);
}

void CameraPreviewController::resetRenderSize(int screenWidth, int screenHeight){
    LOGI("CameraPreviewController::resetSize screenWidth:%d; screenHeight:%d", screenWidth, screenHeight);
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
}

//析构函数
CameraPreviewController::~CameraPreviewController() {

}