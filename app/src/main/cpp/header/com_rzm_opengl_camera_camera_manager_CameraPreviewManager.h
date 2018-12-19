/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class com_rzm_opengl_camera_camera_manager_CameraPreviewManager */

#ifndef _Included_com_rzm_opengl_camera_camera_manager_CameraPreviewManager
#define _Included_com_rzm_opengl_camera_camera_manager_CameraPreviewManager
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     com_rzm_opengl_camera_camera_manager_CameraPreviewManager
 * Method:    switchCameraFacing
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_rzm_opengl_1camera_camera_manager_CameraPreviewManager_switchCameraFacing
  (JNIEnv *, jobject);

/*
 * Class:     com_rzm_opengl_camera_camera_manager_CameraPreviewManager
 * Method:    initEGLContext
 * Signature: (Landroid/view/Surface;III)V
 */
JNIEXPORT void JNICALL Java_com_rzm_opengl_1camera_camera_manager_CameraPreviewManager_initEGLContext
  (JNIEnv *, jobject, jobject, jint, jint, jint);

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
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     com_rzm_opengl_camera_camera_manager_CameraPreviewManager
 * Method:    destroyWindowSurface
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_rzm_opengl_1camera_camera_manager_CameraPreviewManager_destroyWindowSurface
  (JNIEnv *, jobject);

/*
 * Class:     com_rzm_opengl_camera_camera_manager_CameraPreviewManager
 * Method:    destroyEGLContext
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_rzm_opengl_1camera_camera_manager_CameraPreviewManager_destroyEGLContext
  (JNIEnv *, jobject);

/*
 * Class:     com_rzm_opengl_camera_camera_manager_CameraPreviewManager
 * Method:    notifyFrameAvailable
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_com_rzm_opengl_1camera_camera_manager_CameraPreviewManager_notifyFrameAvailable
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif