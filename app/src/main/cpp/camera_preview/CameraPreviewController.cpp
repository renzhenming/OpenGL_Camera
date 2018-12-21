#include "CameraPreviewController.h"


//构造函数
CameraPreviewController::CameraPreviewController(){
    LOGI("CameraPreviewController instance created");
	cameraFacingId = CAMERA_FACING_FRONT;
	window = NULL;
	queue = new MessageQueue("CameraPreviewHandler message queue");
	handler = new CameraPreviewHandler(this, queue);
}

void CameraPreviewController::initEGLContext(ANativeWindow* window, JavaVM *jvm, jobject obj, int screenWidth, int screenHeight, int cameraFacingId){
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

void* CameraPreviewController::threadStartCallback(void *myself) {
    LOGI("pthread start");
	CameraPreviewController *previewController = (CameraPreviewController*) myself;
	previewController->processMessage();
	pthread_exit(0);
	LOGI("pthread exit");
	return 0;
}

void CameraPreviewController::processMessage() {
	bool renderingEnabled = true;
	while (renderingEnabled) {
		Message* msg = NULL;
		if(queue->dequeueMessage(&msg, true) > 0){
		    if(MESSAGE_QUEUE_LOOP_QUIT_FLAG == msg->execute()){
		    		renderingEnabled = false;
		    }
		    delete msg;
		}
	}
}

//开始初始化EGL
//Fatal signal 5 (SIGTRAP), code 1 in tid xxxx有返回值却不返回
bool CameraPreviewController::initialize(){
    LOGI("CameraPreviewController::initialize()");

    egl = new EGL();
    egl->init();
    LOGI("egl initialize success");
    return true;
}

//析构函数
CameraPreviewController::~CameraPreviewController(){

}