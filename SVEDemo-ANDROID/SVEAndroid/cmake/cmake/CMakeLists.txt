
cmake_minimum_required(VERSION 3.4.1)



############SVEEngine 基本配置##############

#set(CMAKE_VERBOSE_MAKEFILE on)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)

include_directories(include)

##############文件路径##############
set(SVE_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../SVEngine/SVEngine/src)
set(SVE_EXPORT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../cpp)

file(GLOB_RECURSE SDK_SVE_EXPORT_SRC
${SVE_EXPORT_DIR}/*.cpp
${SVE_EXPORT_DIR}/*.c
${SVE_EXPORT_DIR}/*.c++
${SVE_EXPORT_DIR}/*.cc
)

set(SVE_LIBS_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/../lib_third)
##############文件路径##############

##############添加配置文件 给变量初始化 该变量可以控制SVEConfig.h中的变量#############
##############添加配置文件 给变量初始化 该变量可以控制SVEConfig.h中的变量############
############## 加入一个配置头文件，用于处理 CMake 对源码的设置 ##############
ADD_DEFINITIONS(-DCONFIG_IS_LOAD_FREETYPE)
option(CONFIG_IS_OPEN_GLESv3 "Enable CONFIG_IS_OPEN_GLESv3" ON)
option(CONFIG_IS_LOAD_ASSIMP "Enable CONFIG_IS_LOAD_ASSIMP" ON)


IF (${CONFIG_IS_OPEN_GLESv3} STREQUAL "ON")
#使用GLESv3
    set(GLES_VERSION_STR GLESv3)
ELSE ()
#使用GLESv2
    set(GLES_VERSION_STR GLESv2)
ENDIF (${CONFIG_IS_OPEN_GLESv3} STREQUAL "OFF")
############## 加入一个配置头文件，用于处理 CMake 对源码的设置 ##############

configure_file (
     "${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
     "${SVE_SRC_DIR}/../SVEConfig.h"
       @ONLY
)

############SVEEngine 基本配置##############

##########code link############
file(GLOB_RECURSE SDK_SVE_SRC 
${SVE_SRC_DIR}/*.cpp 
${SVE_SRC_DIR}/*.c
${SVE_SRC_DIR}/*.c++
${SVE_SRC_DIR}/*.cc
${SVE_SRC_DIR}/*.h
)


########## code link ############

#source_group("sve\\rendercore" FILES ${SDK_SVE_RENDERCORE_INCLUDE} ${SDK_SVE_RENDERCODE_SRC})

######## third spine #########
file(GLOB SDK_SVE_THIRD_SPINE_INCLUDE       ${SVE_SRC_DIR}/third/spine/include/**/*.h)
file(GLOB SDK_SVE_THIRD_SPINE_SRC_CPP       ${SVE_SRC_DIR}/third/spine/src/**/*.cpp)
file(GLOB SDK_SVE_THIRD_SPINE_SRC_C         ${SVE_SRC_DIR}/third/spine/src/**/*.c)
######## third spine #########

######## third rapidjson #########
file(GLOB SDK_SVE_THIRD_RAPIDJSON_INCLUDE   ${SVE_SRC_DIR}/third/rapidjson/**/*.h )
file(GLOB SDK_SVE_THIRD_RAPIDJSON_SRC       ${SVE_SRC_DIR}/third/rapidjson/**/*.c )
######## third rapidjson #########

######## third libpng #########
file(GLOB SDK_SVE_THIRD_LIBPNG_INCLUDE       ${SVE_SRC_DIR}/third/libpng/*.h)

file(GLOB SDK_SVE_THIRD_LIBPNG_SOURCES       ${SVE_SRC_DIR}/third/libpng/*.c 
                                             ${SVE_SRC_DIR}/third/libpng/*.cpp)

file(GLOB SDK_SVE_THIRD_LIBPNG_IMP_SOURCES   ${SVE_SRC_DIR}/third/libpng/**/*.c 
                                             ${SVE_SRC_DIR}/third/libpng/**/*.cpp 
                                             ${SVE_SRC_DIR}/third/libpng/**/*.h)
######## third libpng #########

##################################code list###############################

##################################code list###############################

############ thrid spine list ###########

############t hrid spine list ###########

########third rapidjson list #########

########third rapidjson list #########

######## third libpng list #########

######## third libpng  list #########


add_library(SVEngine
      SHARED
      ${SDK_SVE_SRC}
      ${SDK_SVE_EXPORT_SRC}
      ${SVE_SRC_DIR}/../SVEConfig.h
      )

############# 外部库 ###############

set_target_properties(SVEngine
                        PROPERTIES
                        LIBRARY_OUTPUT_DIRECTORY
                        "${SVE_SRC_DIR}/../lib/${ANDROID_ABI}")

############### assimpd库 ################
   add_library(libassimpd SHARED IMPORTED)
   set_target_properties(libassimpd
                               PROPERTIES
                               IMPORTED_LOCATION
                               ${SVE_LIBS_DIR}/lib/${ANDROID_ABI}/libassimpd.so)
   include_directories(SVEngine PUBLIC 
   ${SVE_LIBS_DIR}/include/3d/assimpd)
############### assimpd库 ################




############### freetype库 ################
add_library(libfreetype SHARED IMPORTED)
set_target_properties(libfreetype
                            PROPERTIES
                            IMPORTED_LOCATION
                            ${SVE_LIBS_DIR}/lib/${ANDROID_ABI}/libfreetype.so)
include_directories(SVEngine PUBLIC 
${SVE_LIBS_DIR}/include/freetype/include 
${SVE_LIBS_DIR}/include/freetype/include/freetype)
############### freetype库 ################



######### 配置头文件 ##############

target_include_directories(
SVEngine PUBLIC 
${SVE_SRC_DIR}/third/libpng
${SVE_SRC_DIR}/third/spine/include  
)
target_include_directories(SVEngine PUBLIC
${SVE_SRC_DIR}/
)

######### 配置头文件 ##############
    find_library( # Sets the name of the path variable.
                  log-lib
                  log )
 target_link_libraries( # Specifies the target library.
                        SVEngine
                        ${GLES_VERSION_STR}
                        libfreetype
                        android
                        EGL
                        OpenSLES
                        jnigraphics
                        z
                        ${log-lib}
                        )
############### 编译代码 ################

