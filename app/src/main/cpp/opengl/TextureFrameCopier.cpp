#include <TextureFrameCopier.h>
//
// Created by renzhenming on 2018/12/22.
//


TextureFrameCopier::TextureFrameCopier(){

}
TextureFrameCopier::~TextureFrameCopier(){

}
void TextureFrameCopier::destroy(){
    mInitialized = false;
    if (mProgramId) {
        glDeleteProgram(mProgramId);
        mProgramId = 0;
    }
}