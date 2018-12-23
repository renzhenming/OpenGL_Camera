//
// Created by renzhenming on 2018/12/22.
//

#ifndef OPENGL_CAMERA_GPUTEXTUREFRAMECOPIER_H
#define OPENGL_CAMERA_GPUTEXTUREFRAMECOPIER_H

#include "TextureFrameCopier.h"

//这种写法针对#extension GL_OES_EGL_image_external : require无效
#define GET_STR(x) #x


static char* GPU_FRAME_VERTEX_SHADER =
        "attribute vec4 vPosition;\n"
        "attribute vec4 vTexCords;\n"
        "varying vec2 yuvTexCoords;\n"
        "uniform highp mat4 trans; \n"
        "void main() {\n"
        "  yuvTexCoords = vTexCords.xy;\n"
        "  gl_Position = trans * vPosition;\n"
        "}\n";

static char* GPU_FRAME_FRAGMENT_SHADER =
        "#extension GL_OES_EGL_image_external : require\n"
        "precision mediump float;\n"
        "uniform samplerExternalOES yuvTexSampler;\n"
        "varying vec2 yuvTexCoords;\n"
        "void main() {\n"
        "  gl_FragColor = texture2D(yuvTexSampler, yuvTexCoords);\n"
        "}\n";

//static const char *GPU_FRAME_VERTEX_SHADER = GET_STR(
//        attribute vec4 vPosition;
//        attribute vec4 vTexCords;
//        varying vec2 yuvTexCoords;
//        uniform highp mat4 trans;
//        void main() {
//            yuvTexCoords = vTexCords.xy;
//            gl_Position = trans * vPosition;
//        }
//);
//
///**
// *  ERROR: 0:1: 'samplerExternalOES' : requires extension GL_OES_EGL_image_external to be enabled
// *  使用GL_OES_EGL_image_external扩展处理，来增强GLSL
// *  定义扩展的的纹理取样器amplerExternalOES
// */
//static const char *GPU_FRAME_FRAGMENT_SHADER = GET_STR(
//        #extension GL_OES_EGL_image_external : require
//        precision mediump float;
//        uniform samplerExternalOES yuvTexSampler;
//        varying vec2 yuvTexCoords;
//        void main() {
//            gl_FragColor = texture2D(yuvTexSampler, yuvTexCoords);
//        }
//);

class GPUTextureFrameCopier : public TextureFrameCopier {
public:
    GPUTextureFrameCopier();

    virtual ~GPUTextureFrameCopier();

    virtual bool init();

    virtual void renderWithCoords(TextureFrame* textureFrame, GLuint texId, GLfloat* vertexCoords, GLfloat* textureCoords);
};

#endif //OPENGL_CAMERA_GPUTEXTUREFRAMECOPIER_H
