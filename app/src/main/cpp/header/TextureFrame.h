//
// Created by renzhenming on 2018/12/22.
//

#ifndef OPENGL_CAMERA_TEXTUREFRAME_H
#define OPENGL_CAMERA_TEXTUREFRAME_H
#include <GLES2/gl2.h>

class TextureFrame {
protected:
    bool checkGlError(const char* op);

public:
    TextureFrame();
    virtual ~TextureFrame();

    virtual bool createTexture() = 0;
    virtual void updateTexImage() = 0;
    virtual bool bindTexture(GLint* uniformSamplers) = 0;
    virtual void dealloc() = 0;
};

#endif //OPENGL_CAMERA_TEXTUREFRAME_H
