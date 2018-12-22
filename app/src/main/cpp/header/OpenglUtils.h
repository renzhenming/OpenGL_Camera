//
// Created by renzhenming on 2018/12/22.
//

#ifndef OPENGL_CAMERA_OPENGLUTILS_H
#define OPENGL_CAMERA_OPENGLUTILS_H
#include <GLES2/gl2.h>
#include <stdlib.h>
#include <Log.h>

static inline void checkGlError(const char* op){
    for (GLint error = glGetError(); error; error = glGetError()) {
        LOGE("after %s() glError (0x%x)\n", op, error);
    }
}

static inline GLuint loadShader(GLenum shaderType, const char* source) {
    GLuint shader = glCreateShader(shaderType);
    if (shader){
        glShaderSource(shader,1,&source,NULL);
        glCompileShader(shader);
        GLint compileStatus = 0;
        glGetShaderiv(shader,GL_COMPILE_STATUS,&compileStatus);
        if(!compileStatus){
            GLint infoLength = 0;
            glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&infoLength);
            if(infoLength){
                char *buf = (char *)malloc(infoLength);
                if(buf){
                    glGetShaderInfoLog(shader,infoLength,NULL,buf);
                    LOGE("Could not compile shader %d:\n%s\n", shaderType, buf);
                    free(buf);
                }
            }
            glDeleteShader(shader);
            shader = 0;
        }
    }
    return shader;
}

static inline GLuint loadProgram(const char* vertexSource, const char* fragmentSource) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER,vertexSource);
    if(!vertexShader){
        return 0;
    }
    LOGI("gl_vertex_shader load success");
    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER,fragmentSource);
    if(!fragmentShader){
        return 0;
    }
    LOGI("gl_fragment_shader load success");
    GLuint program = glCreateProgram();
    if(program){
        glAttachShader(program,vertexShader);
        checkGlError("glAttachShader");
        glAttachShader(program,fragmentShader);
        checkGlError("glAttachShader");
        glLinkProgram(program);
        GLint linkStatus=0;
        glGetProgramiv(program,GL_LINK_STATUS,&linkStatus);
        if(linkStatus != GL_TRUE){
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    LOGE("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    LOGI("gl_program load success");
    return program;
}

#endif //OPENGL_CAMERA_OPENGLUTILS_H
