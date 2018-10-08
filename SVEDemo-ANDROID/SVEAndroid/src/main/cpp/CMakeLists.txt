#
# Copyright (C) The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

# Generate one static lib and one shared lib, copy them into
# ${project_dir}/distribution for other indepdendent applications
# to use.
cmake_minimum_required(VERSION 3.4.1)

#set(CMAKE_VERBOSE_MAKEFILE on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
file(GLOB_RECURSE SDK_SVE_SRC_JNI
${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
${CMAKE_CURRENT_SOURCE_DIR}/*.c
${CMAKE_CURRENT_SOURCE_DIR}/*.c++
${CMAKE_CURRENT_SOURCE_DIR}/*.cc
)

set(LIB_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}../../../../../../SVEngine)
########### 目录配置 ############

set(LIB_ENGINE_DIR ${CMAKE_CURRENT_SOURCE_DIR}../../../../../../SVEngine/platAndriod/cmake)


########### 配置加载目录    ############

set(LIB_ENGINE_BUILD_DIR $ENV{HOME}/tmp)
file(MAKE_DIRECTORY ${LIB_ENGINE_DIR})



##########  添加加载目录    ############
add_subdirectory(${LIB_ENGINE_DIR} ${LIB_SRC_DIR} )

include_directories(include)


add_library(
             SVEEngineNative
             SHARED
             ${SDK_SVE_SRC_JNI}
             )
add_library(SVEngineLib SHARED IMPORTED)
set_target_properties(SVEngineLib
                      PROPERTIES
                      IMPORTED_LOCATION
                      ${LIB_SRC_DIR}/lib/${ANDROID_ABI}/libSVEngine.so)

target_include_directories(SVEEngineNative PUBLIC
${LIB_SRC_DIR}/src
)

find_library( # Sets the name of the path variable.
              log-lib

              log )
target_link_libraries( # Specifies the target library.
                       SVEEngineNative
                       SVEngineLib
                       android
                       EGL
                       GLESv3
                       GLESv2
                       jnigraphics
                       z
                       ${log-lib} )



