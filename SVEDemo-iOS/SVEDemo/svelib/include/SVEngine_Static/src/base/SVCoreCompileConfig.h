//
// SVCoreCompileConfig.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_CORE_COMPILE_CONFIG_H
#define SV_CORE_COMPILE_CONFIG_H

#include <stdio.h>

#define LOG_ENABLE

#if defined(__ANDROID__)
#define SV_ANDROID
#define SV_API

#include <jni.h>
#include <android/log.h>
#include <android/bitmap.h>
#include <GLES3/gl3.h>
#define __gl2_h_

#include <GLES2/gl2ext.h>
#include <GLES3/gl3ext.h>
#include <EGL/egl.h>
#include <EGl/eglext.h>
#include <EGl/eglplatform.h>

#ifdef LOG_ENABLE
#define  LOG_TAG  "SVEngine"
#define  SV_LOG_DEBUG(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  SV_LOG_INFO(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define  SV_LOG_ERROR(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define  SV_LOG_ERROR_TE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#else
#define  SV_LOG_DEBUG(...)
#define  SV_LOG_INFO(...)
#define  SV_LOG_ERROR(...)
#endif


#endif


//MAC平台 或 IPHONE平台
#if defined(__MACOSX__) || defined(__APPLE__)

#define SV_API

#include <TargetConditionals.h>

#if TARGET_OS_IPHONE

#define SV_IOS
//#define CONFIG_IS_LOAD_ASSIMP
//#define CONFIG_IS_LOAD_FREETYPE
#include <UIKit/UIKit.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#include <Foundation/Foundation.h>
#include <OpenGLES/EAGL.h>
#include <GLKit/GLKit.h>
#include <OpenGLES/ES3/gl.h>
#include <OpenGLES/ES3/glext.h>

#elif TARGET_OS_MAC

#define SV_OSX
#include <AppKit/AppKit.h>
#include <AppKit/NSOpenGL.h>
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#include <GLKit/GLKit.h>
#include <OpenGL/OpenGL.h>

#endif

#ifdef LOG_ENABLE
#define  SV_LOG_INFO(...)   printf(__VA_ARGS__);
#define  SV_LOG_ERROR(...)  printf(__VA_ARGS__);
#define  SV_LOG_DEBUG(...)  printf(__VA_ARGS__);
#else
#define  SV_LOG_DEBUG(...) ;
#define  SV_LOG_INFO(...)  ;
#define  SV_LOG_ERROR(...) ;
#endif  //LOG_ENABLE

#endif  //__MACOSX__ || __APPLE__

#define _SV_MTL_MAX_TEXTURES_ 4

#define _SV_HAS_S64


//namesapce

namespace sv{ };
namespace sv{ namespace util{ } };
namespace sv{ namespace data{ } };
namespace sv{ namespace logic{ } };
namespace sv{ namespace render{ } };
namespace sv{ namespace mtl{ } };
namespace sv{ namespace node{ } };
namespace sv{ namespace effect{ } };
namespace sv{ namespace ui{ } };
namespace sv{ namespace detect{ } };

using namespace sv;
using namespace sv::util;
using namespace sv::data;
using namespace sv::logic;
using namespace sv::render;
using namespace sv::mtl;
using namespace sv::node;
using namespace sv::effect;
using namespace sv::ui;
using namespace sv::detect;

#endif // SV_CORE_COMPILE_CONFIG_H
