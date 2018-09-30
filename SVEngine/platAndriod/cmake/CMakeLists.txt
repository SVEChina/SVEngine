
cmake_minimum_required(VERSION 3.4.1)



############SVEEngine 基本配置##############

#set(CMAKE_VERBOSE_MAKEFILE on)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)

include_directories(include)

##############文件路径##############
set(SVE_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../src)
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
)


file(GLOB SDK_SVE_ACT_INCLUDE         ${SVE_SRC_DIR}/act/*.h)
file(GLOB SDK_SVE_ACT_SRC             ${SVE_SRC_DIR}/act/*.cpp)
file(GLOB SDK_SVE_APP_INCLUDE         ${SVE_SRC_DIR}/app/*.h)
file(GLOB SDK_SVE_APP_SRC             ${SVE_SRC_DIR}/app/*.cpp)
file(GLOB SDK_SVE_BASE_INCLUDE        ${SVE_SRC_DIR}/app/base/*.h)
file(GLOB SDK_SVE_BASE_SRC            ${SVE_SRC_DIR}/base/*.cpp)
file(GLOB SDK_SVE_BASESYS_INCLUDE     ${SVE_SRC_DIR}/basesys/*.h)
file(GLOB SDK_SVE_BASESYS_SRC         ${SVE_SRC_DIR}/basesys/*.cpp)
file(GLOB SDK_SVE_CALLBACK_INCLUDE    ${SVE_SRC_DIR}/callback/*.h)
file(GLOB SDK_SVE_CALLBACK_SRC        ${SVE_SRC_DIR}/callback/*.cpp)
file(GLOB SDK_SVE_CORE_INCLUDE        ${SVE_SRC_DIR}/core/*.h)
file(GLOB SDK_SVE_CORE_SRC            ${SVE_SRC_DIR}/core/*.cpp)
file(GLOB SDK_SVE_DETECT_INCLUDE      ${SVE_SRC_DIR}/detect/*.h)
file(GLOB SDK_SVE_DETECT_SRC          ${SVE_SRC_DIR}/detect/*.cpp)
file(GLOB SDK_SVE_EDIT_INCLUDE        ${SVE_SRC_DIR}/edit/*.h)
file(GLOB SDK_SVE_EDIT_SRC            ${SVE_SRC_DIR}/edit/*.cpp)
file(GLOB SDK_SVE_EVENT_INCLUDE       ${SVE_SRC_DIR}/event/*.h)
file(GLOB SDK_SVE_EVENT_SRC           ${SVE_SRC_DIR}/event/*.cpp)
file(GLOB SDK_SVE_FILE_INCLUDE        ${SVE_SRC_DIR}/file/*.h)
file(GLOB SDK_SVE_FILE_SRC            ${SVE_SRC_DIR}/file/*.cpp)
file(GLOB SDK_SVE_GAME_INCLUDE        ${SVE_SRC_DIR}/game/*.h)
file(GLOB SDK_SVE_GAME_SRC            ${SVE_SRC_DIR}/game/*.cpp)
file(GLOB SDK_SVE_LOGICSYS_INCLUDE    ${SVE_SRC_DIR}/logicsys/*.h)
file(GLOB SDK_SVE_LOGICSYS_SRC        ${SVE_SRC_DIR}/logicsys/*.cpp)
file(GLOB SDK_SVE_MEDIA_INCLUDE       ${SVE_SRC_DIR}/media/*.h)
file(GLOB SDK_SVE_MEDIA_SRC           ${SVE_SRC_DIR}/media/*.cpp)
file(GLOB SDK_SVE_MTL_INCLUDE         ${SVE_SRC_DIR}/mtl/*.h)
file(GLOB SDK_SVE_MTL_SRC             ${SVE_SRC_DIR}/mtl/*.cpp)
file(GLOB SDK_SVE_NODE_INCLUDE        ${SVE_SRC_DIR}/node/*.h)
file(GLOB SDK_SVE_NODE_SRC            ${SVE_SRC_DIR}/node/*.cpp)
file(GLOB SDK_SVE_OPERATE_INCLUDE     ${SVE_SRC_DIR}/operate/*.h)
file(GLOB SDK_SVE_OPERATE_SRC         ${SVE_SRC_DIR}/operate/*.cpp)
file(GLOB SDK_SVE_PARSE_INCLUDE       ${SVE_SRC_DIR}/parse/*.h)
file(GLOB SDK_SVE_PARSE_SRC           ${SVE_SRC_DIR}/parse/*.cpp)
file(GLOB SDK_SVE_PARTICLE_INCLUDE    ${SVE_SRC_DIR}/particle/*.h)
file(GLOB SDK_SVE_PARTICLE_SRC        ${SVE_SRC_DIR}/particle/*.cpp)
file(GLOB SDK_SVE_RENDERCORE_INCLUDE  ${SVE_SRC_DIR}/rendercore/*.h 
                                      ${SVE_SRC_DIR}/rendercore/**/*.h)
file(GLOB SDK_SVE_RENDERCORE_SRC      ${SVE_SRC_DIR}/rendercore/*.cpp 
                                      ${SVE_SRC_DIR}/rendercore/**/*.cpp)
file(GLOB SDK_SVE_SDATA_INCLUDE       ${SVE_SRC_DIR}/sdata/*.h)
file(GLOB SDK_SVE_SDATA_SRC           ${SVE_SRC_DIR}/sdata/*.cpp)
file(GLOB SDK_SVE_TEXTURE_INCLUDE     ${SVE_SRC_DIR}/texture/*.h)
file(GLOB SDK_SVE_TEXTURE_SRC         ${SVE_SRC_DIR}/texture/*.cpp)
file(GLOB SDK_SVE_UI_INCLUDE          ${SVE_SRC_DIR}/ui/*.h)
file(GLOB SDK_SVE_UI_SRC              ${SVE_SRC_DIR}/ui/*.cpp)
file(GLOB SDK_SVE_VIDEO_INCLUDE       ${SVE_SRC_DIR}/video/*.h)
file(GLOB SDK_SVE_VIDEO_SRC           ${SVE_SRC_DIR}/video/*.cpp)
file(GLOB SDK_SVE_WORK_INCLUDE        ${SVE_SRC_DIR}/work/*.h)
file(GLOB SDK_SVE_WORK_SRC            ${SVE_SRC_DIR}/work/*.cpp)
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
file(GLOB SVE_ROOT_LIST "*.h")
list(APPEND SVE_ROOT_LIST ${SDK_SVE_ACT_SRC})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_ACT_INCLUDE})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_APP_SRC})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_APP_INCLUDE})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_BASE_SRC})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_BASE_INCLUDE})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_BASESYS_SRC})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_BASESYS_INCLUDE})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_CALLBACK_SRC})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_CALLBACK_INCLUDE})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_CORE_SRC})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_CORE_INCLUDE})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_DETECT_SRC})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_DETECT_INCLUDE})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_EDIT_SRC})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_EDIT_INCLUDE})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_EVENT_SRC})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_EVENT_INCLUDE})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_FILE_SRC})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_FILE_INCLUDE})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_GAME_SRC})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_GAME_INCLUDE})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_LOGICSYS_SRC})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_LOGICSYS_INCLUDE})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_MEDIA_SRC})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_MEDIA_INCLUDE})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_MTL_SRC})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_MTL_INCLUDE})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_NODE_SRC})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_NODE_INCLUDE})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_OPERATE_SRC})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_OPERATE_INCLUDE})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_PARSE_SRC})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_PARSE_INCLUDE})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_PARTICLE_INCLUDE})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_PARTICLE_SRC})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_RENDERCORE_INCLUDE})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_RENDERCORE_SRC})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_SDATA_SRC})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_SDATA_INCLUDE})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_TEXTURE_SRC})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_TEXTURE_INCLUDE})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_THIRD_SRC})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_THIRD_INCLUDE})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_UI_SRC})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_UI_INCLUDE})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_VIDEO_SRC})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_VIDEO_INCLUDE})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_WORK_SRC})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_WORK_INCLUDE})
##################################code list###############################

############ thrid spine list ###########
list(APPEND SVE_ROOT_LIST ${SDK_SVE_THIRD_SPINE_INCLUDE})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_THIRD_SPINE_SRC_CPP})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_THIRD_SPINE_SRC_C})
############t hrid spine list ###########

########third rapidjson list #########
list(APPEND SVE_ROOT_LIST ${SDK_SVE_THIRD_RAPIDJSON_INCLUDE})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_THIRD_RAPIDJSON_SRC})
########third rapidjson list #########

######## third libpng list #########
list(APPEND SVE_ROOT_LIST ${SDK_SVE_THIRD_LIBPNG_INCLUDE})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_THIRD_LIBPNG_SOURCES})
list(APPEND SVE_ROOT_LIST ${SDK_SVE_THIRD_LIBPNG_IMP_SOURCES})
######## third libpng  list #########


add_library(SVEngine
      SHARED
      ${SDK_SVE_SRC}
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

