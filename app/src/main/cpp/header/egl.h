#ifndef rzm_opengl_camera_manager_egl_egl
#define rzm_opengl_camera_manager_egl_egl
#include <EGL/egl.h>
#include <EGL/eglext.h>

class EGL {
private:
    EGLDisplay display;
    EGLConfig configure;
    EGLContext context;
    PFNEGLPRESENTATIONTIMEANDROID pfneglPresentationTimeANDROID;
public:
    EGL();
    bool init();
    bool init(EGLContext sharedContext);
    void release();
    virtual ~EGL();
};

#endif // rzm_opengl_camera_manager_egl_egl