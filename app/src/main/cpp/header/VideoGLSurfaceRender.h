//
// Created by renzhenming on 2018/12/23.
//

#ifndef OPENGLCAMERA_VIDEOGLSURFACERENDER_H
#define OPENGLCAMERA_VIDEOGLSURFACERENDER_H

#include <GLES2/gl2.h>

static char* OUTPUT_VIEW_VERTEX_SHADER =
        "attribute vec4 position;    \n"
        "attribute vec2 texcoord;   \n"
        "varying vec2 v_texcoord;     \n"
        "void main(void)               \n"
        "{                            \n"
        "   gl_Position = position;  \n"
        "   v_texcoord = texcoord;  \n"
        "}                            \n";

static char* OUTPUT_VIEW_FRAG_SHADER =
        "varying highp vec2 v_texcoord;\n"
        "uniform sampler2D yuvTexSampler;\n"
        "void main() {\n"
        "  gl_FragColor = texture2D(yuvTexSampler, v_texcoord);\n"
        "}\n";

class VideoGLSurfaceRender{
protected:
    char* mVertexShader;
    char* mFragmentShader;

    //用于glViewPort设置绘制区域
    GLint _backingLeft;
    GLint _backingTop;
    GLint _backingWidth;
    GLint _backingHeight;

    //渲染程序对象
    GLuint mProgramId;
    GLuint mVertexCoords;
    GLuint mTextureCoords;
    GLint mUniformTexture;
    bool mInitialized;
public:
    VideoGLSurfaceRender();
    virtual ~VideoGLSurfaceRender();
    bool init(int width, int height);
    void renderToAutoFitTexture(GLuint inputTexId, int width, int height, GLuint outputTexId);
    void renderToViewWithAutofit(GLuint texID, int screenWidth, int screenHeight, int texWidth, int texHeight);
    float calcCropRatio(int screenWidth, int screenHeight, int texWidth, int texHeight);
    void dealloc();
};

#endif //OPENGLCAMERA_VIDEOGLSURFACERENDER_H
