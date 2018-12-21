
#ifndef rzm_opengl_camera_manager_camera_preview_CameraPreviewController
#define rzm_opengl_camera_manager_camera_preview_CameraPreviewController

#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <pthread.h>

#include "Log.h"
#include "handler.h"
#include "message_queue.h"
#include "egl.h"

//定义前后摄像头默认id
#define CAMERA_FACING_BACK										0
#define CAMERA_FACING_FRONT										1

//先声明，不然会报错
class CameraPreviewHandler;


class CameraPreviewController {
protected:
    ANativeWindow* window;
    JavaVM *jvm;
    jobject obj;
    int screenWidth;
    int screenHeight;

    //摄像头id
    int cameraFacingId;

    CameraPreviewHandler* handler;
    MessageQueue* queue;
    pthread_t _threadId;

    static void* threadStartCallback(void *myself);
    void processMessage();

    //EGL对象
    EGL *egl;
public:
    //构造函数
    CameraPreviewController();
    virtual ~CameraPreviewController();

    //初始化EGL
    void initEGLContext(ANativeWindow* window, JavaVM *jvm, jobject obj, int screenWidth, int screenHeight, int cameraFacingId);
    virtual bool initialize();
};

enum RenderThreadMessage {
        MSG_RENDER_FRAME = 0,
        MSG_EGL_THREAD_CREATE,
		MSG_EGL_CREATE_PREVIEW_SURFACE,
        MSG_SWITCH_CAMERA_FACING,
		MSG_EGL_DESTROY_PREVIEW_SURFACE,
        MSG_EGL_THREAD_EXIT
};

class CameraPreviewHandler: public Handler {
	private:
		CameraPreviewController* previewController;

	public:
		CameraPreviewHandler(CameraPreviewController* previewController, MessageQueue* queue) :
				Handler(queue) {
			this->previewController = previewController;
		}
		void handleMessage(Message* msg) {
			int what = msg->getWhat();
			switch (what) {
			case MSG_EGL_THREAD_CREATE:
			    LOGI("handleMessage MSG_EGL_THREAD_CREATE");
				previewController->initialize();
				break;
			case MSG_EGL_CREATE_PREVIEW_SURFACE:
				//previewController->createPreviewSurface();
				break;
			case MSG_SWITCH_CAMERA_FACING:
				//previewController->switchCamera();
				break;
			case MSG_EGL_DESTROY_PREVIEW_SURFACE:
				//previewController->destroyPreviewSurface();
				break;
			case MSG_EGL_THREAD_EXIT:
				//previewController->destroy();
				break;
			case MSG_RENDER_FRAME:
				//previewController->renderFrame();
				break;
			}
		}
	};

#endif