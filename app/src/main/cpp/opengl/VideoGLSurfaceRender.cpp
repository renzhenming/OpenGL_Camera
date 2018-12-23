//
// Created by renzhenming on 2018/12/23.
//

#include <Log.h>
#include "VideoGLSurfaceRender.h"

VideoGLSurfaceRender::VideoGLSurfaceRender(){
    mVertexShader = OUTPUT_VIEW_VERTEX_SHADER;
    mFragmentShader = OUTPUT_VIEW_FRAG_SHADER;

}
VideoGLSurfaceRender::~VideoGLSurfaceRender(){

}
bool VideoGLSurfaceRender::init(int width, int height){
    LOGI("VideoGLSurfaceRender::init");
    return true;
}