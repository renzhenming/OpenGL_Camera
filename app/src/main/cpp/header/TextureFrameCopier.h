//
// Created by renzhenming on 2018/12/22.
//

#ifndef OPENGL_CAMERA_TEXTUREFRAMECOPIER_H
#define OPENGL_CAMERA_TEXTUREFRAMECOPIER_H

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "TextureFrame.h"
#include "OpenglUtils.h"

#define GET_STR(x) #x


static char* NO_FILTER_VERTEX_SHADER =
        "attribute vec4 vPosition;\n"
        "attribute vec4 vTexCords;\n"
        "varying vec2 yuvTexCoords;\n"
        "uniform highp mat4 texMatrix;\n"
        "uniform highp mat4 trans; \n"
        "void main() {\n"
        "  yuvTexCoords = (texMatrix*vTexCords).xy;\n"
        "  gl_Position = trans * vPosition;\n"
        "}\n";

static char* NO_FILTER_FRAGMENT_SHADER =
        "varying vec2 yuvTexCoords;\n"
        "uniform sampler2D yuvTexSampler;\n"
        "void main() {\n"
        "  gl_FragColor = texture2D(yuvTexSampler, yuvTexCoords);\n"
        "}\n";

//static const char *NO_FILTER_VERTEX_SHADER = GET_STR(
//        attribute vec4 vPosition;
//        attribute vec4 vTexCords;
//        varying vec2 yuvTexCoords;
//        uniform highp mat4 texMatrix;
//        uniform highp mat4 trans;
//        void main() {
//            yuvTexCoords = (texMatrix * vTexCords).xy;
//            gl_Position = trans * vPosition;
//        }
//);
//static const char *NO_FILTER_FRAGMENT_SHADER = GET_STR(
//        varying
//        vec2 yuvTexCoords;
//        uniform
//        sampler2D yuvTexSampler;
//        void main() {
//            gl_FragColor = texture2D(yuvTexSampler, yuvTexCoords);
//        }
//);

class TextureFrameCopier {
protected:
    const char *mVertexShader;
    const char *mFragmentShader;
    bool mInitialized;
    GLuint mProgramId;

    //顶点坐标
    GLuint mVertexCoords;
    //材质坐标
    GLuint mTextureCoords;;

    GLint mUniformTexture;

    GLint mUniformTexMatrix;
    GLint mUniformTransforms;
public:
    TextureFrameCopier();

    virtual ~TextureFrameCopier();

    virtual bool init() = 0;

    virtual void renderWithCoords(TextureFrame *textureFrame, GLuint texId, GLfloat *vertexCoords,
                                  GLfloat *textureCoords)  = 0;

    virtual void destroy();
};

#endif //OPENGL_CAMERA_TEXTUREFRAMECOPIER_H
