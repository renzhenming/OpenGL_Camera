//
// Created by renzhenming on 2018/12/23.
//

#ifndef OPENGLCAMERA_VIDEOGLSURFACERENDER_H
#define OPENGLCAMERA_VIDEOGLSURFACERENDER_H

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
public:
    VideoGLSurfaceRender();
    virtual ~VideoGLSurfaceRender();
    bool init(int width, int height);
};

#endif //OPENGLCAMERA_VIDEOGLSURFACERENDER_H
