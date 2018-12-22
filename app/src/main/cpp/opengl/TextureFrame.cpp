//
// Created by renzhenming on 2018/12/22.
//

#include <Log.h>
#include "TextureFrame.h"

TextureFrame::TextureFrame() {

}

TextureFrame::~TextureFrame() {

}

bool TextureFrame::checkGlError(const char* op) {
    GLint error;
    for (error = glGetError(); error; error = glGetError()) {
        LOGI("error::after %s() glError (0x%x)", op, error);
        return true;
    }
    return false;
}