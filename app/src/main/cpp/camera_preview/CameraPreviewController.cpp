#include <CameraPreviewRender.h>
#include "CameraPreviewController.h"


//构造函数
CameraPreviewController::CameraPreviewController() {
    LOGI("CameraPreviewController instance created");
    cameraFacingId = CAMERA_FACING_FRONT;
    window = NULL;
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

    this->configCamera();
    return true;
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
            cameraWidth = env->CallIntMethod(cameraConfigJClass,getWidthMethodId);

            //获取高度
            jmethodID getHeightMethodId = env->GetMethodID(cameraConfigJClass,"getTextureHeight","()I");
            cameraHeight = env->CallIntMethod(cameraConfigJClass,getHeightMethodId);

            textureWidth = 360;
            textureHeight = 640;

            LOGI("camera : {%d, %d}", cameraWidth, cameraHeight);
            LOGI("Texture : {%d, %d}", textureWidth, textureHeight);
        }
    }

    if(jvm->DetachCurrentThread() != JNI_OK){
        LOGE("configCamera: DetachCurrentThread() failed");
        return;
    }

    LOGI("configCamera success");
}

//析构函数
CameraPreviewController::~CameraPreviewController() {

}