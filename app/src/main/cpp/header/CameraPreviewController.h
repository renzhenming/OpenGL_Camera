
#ifndef rzm_opengl_camera_manager_camera_preview_CameraPreviewController
#define rzm_opengl_camera_manager_camera_preview_CameraPreviewController

#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <pthread.h>

#include "Log.h"
#include "handler.h"
#include "message_queue.h"
#include "egl.h"
#include "CameraPreviewRender.h"

//定义前后摄像头默认id
#define CAMERA_FACING_BACK										0
#define CAMERA_FACING_FRONT										1

//先声明，不然会报错
class CameraPreviewHandler;


class CameraPreviewController {
protected:
    ANativeWindow* window;
    JavaVM *jvm;

    //CameraPreviewManager.java 的对象
    jobject obj;
    int screenWidth;
    int screenHeight;

	//设置的摄像头预览界面的宽高
	int cameraWidth;
	int cameraHeight;

	int textureWidth;
	int textureHeight;

	//摄像头需要旋转的角度，旋转之后才能摆正
	int degress;

    //摄像头id
    int cameraFacingId;

	int64_t startTime = -1;

    CameraPreviewHandler* handler;
    MessageQueue* queue;
    pthread_t _threadId;

    bool isInSwitchingCamera;

    //EGL对象
    EGL *egl;

    //预览surface
    EGLSurface previewSurface;

    //它负责处理:拷贝纹理(copier)、处理纹理(processor)、输出纹理(render) 核心操作
    CameraPreviewRender* renderer;

    static void* threadStartCallback(void *myself);
    void processMessage();


protected:
	//配置相机
	void configCamera();
	void startCameraPreview();
	virtual void processVideoFrame(float position);
	void updateTexImage();
	void draw();
    void releaseCamera();
	void deleteGlobalRef();
public:
    //构造函数
    CameraPreviewController();
    virtual ~CameraPreviewController();

    /** 准备EGL Context与EGLThread **/
    void initEGLContext(ANativeWindow* window, JavaVM *jvm, jobject obj, int screenWidth, int screenHeight, int cameraFacingId);
	/** 销毁EGLContext与EGLThread **/
	virtual void destroyEGLContext();
    /** 当Camera捕捉到新的一帧图像会调用 **/
    void notifyFrameAvailable();

    /** 切换摄像头转向 **/
    void switchCameraFacing();

    virtual bool initialize();

    void renderFrame();

    void switchCamera();
	//销毁EGL资源并且调用Andorid销毁Camera
	virtual void destroy();

	void destroyPreviewSurface();

    void createWindowSurface(ANativeWindow* window);
    void destroyWindowSurface();

    void createPreviewSurface();
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
			case MSG_RENDER_FRAME:
                LOGI("handleMessage MSG_RENDER_FRAME");
                previewController->renderFrame();
                break;
			case MSG_EGL_CREATE_PREVIEW_SURFACE:
				previewController->createPreviewSurface();
				break;
			case MSG_SWITCH_CAMERA_FACING:
                LOGI("handleMessage MSG_SWITCH_CAMERA_FACING");
				previewController->switchCamera();
				break;
			case MSG_EGL_DESTROY_PREVIEW_SURFACE:
				previewController->destroyPreviewSurface();
				break;
			case MSG_EGL_THREAD_EXIT:
				previewController->destroy();
				break;

			}
		}
	};

#endif