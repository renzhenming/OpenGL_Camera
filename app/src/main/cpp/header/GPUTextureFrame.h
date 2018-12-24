//
// Created by renzhenming on 2018/12/23.
//

#ifndef OPENGLCAMERA_GPUTEXTUREFRAME_H
#define OPENGLCAMERA_GPUTEXTUREFRAME_H

#include <GLES2/gl2.h>
#include "TextureFrame.h"

class GPUTextureFrame :public TextureFrame{
private:
    GLuint mTextureId;
    int initTexture();
public:
    GPUTextureFrame();
    virtual ~GPUTextureFrame();

    bool createTexture();
    bool bindTexture(GLint* uniformSamplers);
    void updateTexImage();
    void dealloc();

    GLuint getDecodeTexId() {
        return mTextureId;
    };
};

#endif //OPENGLCAMERA_GPUTEXTUREFRAME_H
