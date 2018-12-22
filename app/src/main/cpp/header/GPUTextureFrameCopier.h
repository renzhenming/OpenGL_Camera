//
// Created by renzhenming on 2018/12/22.
//

#ifndef OPENGL_CAMERA_GPUTEXTUREFRAMECOPIER_H
#define OPENGL_CAMERA_GPUTEXTUREFRAMECOPIER_H

#include "TextureFrameCopier.h"

#define GET_STR(x) #x

static const char *GPU_FRAME_VERTEX_SHADER = GET_STR(
        attribute vec4 vPosition;
        attribute vec4 vTexCords;
        varying vec2 yuvTexCoords;
        uniform highp mat4 trans;
        void main() {
            yuvTexCoords = vTexCords.xy;
            gl_Position = trans * vPosition;
        }
);
static const char *GPU_FRAME_FRAGMENT_SHADER = GET_STR(
        //#extension GL_OES_EGL_image_external : require
        precision mediump float;
        uniform samplerExternalOES yuvTexSampler;
        varying vec2 yuvTexCoords;
        void main() {
            gl_FragColor = texture2D(yuvTexSampler, yuvTexCoords);
        }
);

class GPUTextureFrameCopier : public TextureFrameCopier {
public:
    GPUTextureFrameCopier();

    virtual ~GPUTextureFrameCopier();

    virtual bool init();

    virtual void renderWithCoords(TextureFrame* textureFrame, GLuint texId, GLfloat* vertexCoords, GLfloat* textureCoords);
};

#endif //OPENGL_CAMERA_GPUTEXTUREFRAMECOPIER_H
