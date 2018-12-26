
#include <jni.h>
#include <string>

#include "com_rzm_opengl_camera_camera_manager_CameraPreviewManager.h"
#include "CameraPreviewController.h"
#include <android/native_window.h>
#include <android/native_window_jni.h>

static CameraPreviewController *cameraController = NULL;

/*
 * Class:     com_rzm_opengl_camera_camera_manager_CameraPreviewManager
 * Method:    initEGLContext
 * Signature: (Landroid/view/Surface;III)V
 */
JNIEXPORT void JNICALL Java_com_rzm_opengl_1camera_camera_manager_CameraPreviewManager_initEGLContext
  (JNIEnv *env, jobject _jobject, jobject surface, jint width, jint height, jint facingId){
        cameraController = new CameraPreviewController();
        JavaVM *jvm = NULL;
        env->GetJavaVM(&jvm);
        jobject obj = env->NewGlobalRef(_jobject);
        if (surface != NULL && NULL != cameraController) {
        	ANativeWindow* window = ANativeWindow_fromSurface(env, surface);
        	if (window != NULL) {
        		cameraController->initEGLContext(window, jvm, obj, width, height, facingId);
        	}
        }
  }

/*
 * Class:     com_rzm_opengl_camera_camera_manager_CameraPreviewManager
 * Method:    switchCameraFacing
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_rzm_opengl_1camera_camera_manager_CameraPreviewManager_switchCameraFacing
        (JNIEnv * env, jobject obj) {
    if(NULL != cameraController) {
        cameraController->switchCameraFacing();
    }
}


/*
 * Class:     com_rzm_opengl_camera_camera_manager_CameraPreviewManager
 * Method:    initWindowSurface
 * Signature: (Landroid/view/Surface;)V
 */
JNIEXPORT void JNICALL Java_com_rzm_opengl_1camera_camera_manager_CameraPreviewManager_initWindowSurface
  (JNIEnv *, jobject, jobject);

/*
 * Class:     com_rzm_opengl_camera_camera_manager_CameraPreviewManager
 * Method:    resetRenderSize
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_com_rzm_opengl_1camera_camera_manager_CameraPreviewManager_resetRenderSize
        (JNIEnv * env, jobject obj, jint screenWidth, jint screenHeight) {
    if(NULL != cameraController) {
        cameraController->resetRenderSize(screenWidth, screenHeight);
    }
}

/*
* Class:     com_rzm_opengl_camera_camera_manager_CameraPreviewManager
* Method:    createWindowSurface
* Signature: (Landroid/view/Surface;)V
*/
JNIEXPORT void JNICALL Java_com_rzm_opengl_1camera_camera_manager_CameraPreviewManager_createWindowSurface
        (JNIEnv * env, jobject obj, jobject surface) {
    if (surface != 0 && NULL != cameraController) {
        ANativeWindow* window = ANativeWindow_fromSurface(env, surface);
        if (window != NULL) {
            cameraController->createWindowSurface(window);
        }
    }
}
/*
 * Class:     com_rzm_opengl_camera_camera_manager_CameraPreviewManager
 * Method:    destroyWindowSurface
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_rzm_opengl_1camera_camera_manager_CameraPreviewManager_destroyWindowSurface
        (JNIEnv * env, jobject obj) {
    if(NULL != cameraController) {
        cameraController->destroyWindowSurface();
    }
}

/*
 * Class:     com_rzm_opengl_camera_camera_manager_CameraPreviewManager
 * Method:    destroyEGLContext
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_rzm_opengl_1camera_camera_manager_CameraPreviewManager_destroyEGLContext
        (JNIEnv * env, jobject obj) {
    if(NULL != cameraController) {
        cameraController->destroyEGLContext();
        delete cameraController;
        cameraController = NULL;
    }
}

/*
 * Class:     com_rzm_opengl_camera_camera_manager_CameraPreviewManager
 * Method:    notifyFrameAvailable
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_rzm_opengl_1camera_camera_manager_CameraPreviewManager_notifyFrameAvailable
  (JNIEnv *env, jobject _jobject){
    if(cameraController != NULL) {
        cameraController->notifyFrameAvailable();
    }
}