
cmake_minimum_required(VERSION 3.4.1)

#设置头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/header)

add_library( # Sets the name of the library.
             reviewer

             # Sets the library as a shared library.
             SHARED

             ${CMAKE_CURRENT_SOURCE_DIR}/src/main/cpp/CameraPreviewManager.cpp )


find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )


target_link_libraries( # Specifies the target library.
                       reviewer
                       EGL
                       GLESv2
                       android
                       ${log-lib} )