
cmake_minimum_required(VERSION 3.4.1)

#设置头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/header)

#设置c++文件路径变量
set(CPP_FILE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp)

add_library( reviewer
             SHARED
             ${CPP_FILE_DIR}/CameraPreviewManager.cpp
             ${CPP_FILE_DIR}/camera_preview/CameraPreviewController.cpp
             ${CPP_FILE_DIR}/camera_preview/CameraPreviewRender.cpp
             ${CPP_FILE_DIR}/log/Log.cpp
             ${CPP_FILE_DIR}/message_queue/handler.cpp
             ${CPP_FILE_DIR}/message_queue/message_queue.cpp
             ${CPP_FILE_DIR}/egl/egl.cpp
             ${CPP_FILE_DIR}/opengl/TextureFrameCopier.cpp
             ${CPP_FILE_DIR}/opengl/GPUTextureFrameCopier.cpp
             ${CPP_FILE_DIR}/opengl/TextureFrame.cpp
             ${CPP_FILE_DIR}/opengl/VideoGLSurfaceRender.cpp
)


find_library( log-lib
              log )


target_link_libraries( reviewer
                       EGL
                       GLESv2
                       android
                       ${log-lib} )