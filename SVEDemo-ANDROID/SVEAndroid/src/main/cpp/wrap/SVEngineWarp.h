//
// Created by 徐子昱 on 2018/6/27.
//

#ifndef _Included_com_sve_engine_SVEEngineJNI
#define _Included_com_sve_engine_SVEEngineJNI
#include <jni.h>
#include "android/native_window_jni.h"

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_startEngineNative(JNIEnv *env, jobject instance, jlong nativeAddress);

JNIEXPORT jlong JNICALL
Java_com_sve_engine_SVEEngineJNI_initNative(JNIEnv *env, jobject instance);

JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_setSurfaceNative(JNIEnv *env, jobject instance, jlong _nativeAddress,
                                            jobject _suface);

JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_addResPathNative(JNIEnv *env, jobject instance, jlong _nativeAddress,
                                                  jstring _path_);

JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_createSceneNative(JNIEnv *env, jobject instance, jlong _nativeAddress);


JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_createRenderGLNative(JNIEnv *env, jobject instance,
                                                      jlong _nativeAddress, jlong _contextHandle,
                                                      jint _w, jint _h);

JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_createRenderTargetNative(JNIEnv *env, jobject instance,
                                                          jlong _nativeAddress, jint _fboid,
                                                          jint _colorid, jint _w, jint _h);
JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_createRenderTargetTextureNative(JNIEnv *env, jobject instance,
                                                                 jlong _nativeAddress, jint _w,
                                                                 jint _h, jint _textureId);

JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_destoryRenderGLNative(JNIEnv *env, jobject instance,
                                                       jlong _nativeAddress);

JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_destoryRenderTargetNative(JNIEnv *env, jobject instance,
                                                           jlong _nativeAddress);

JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_destorySceneNative(JNIEnv *env, jobject instance,
                                                    jlong _nativeAddress);

JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_suspendNative(JNIEnv *env, jobject instance, jlong _nativeAddress);

JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_resumeNative(JNIEnv *env, jobject instance, jlong _nativeAddress);

JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_stopEngineNative(JNIEnv *env, jobject instance, jlong _nativeAddress);

JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_createSteamNative(JNIEnv *env, jobject instance, jlong _nativeAddress,
                                                   jstring _name_, jint _type, jint _formate, jint _width,
                                                   jint _height,jint _angle);

JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_pushSteamNative(JNIEnv *env, jobject instance, jlong _nativeAddress,
                                                 jstring _name_, jobject _srcData, jint _format,
                                                 jint _width, jint _height,jint _angle);

JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_createSteamOutTexNaitve(JNIEnv *env, jobject instance,
                                                         jlong _nativeAddress, jstring _name_,
                                                         jint _type, jint _formate, jint _width,
                                                         jint _height, jint _angle);
JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_destoryInSteamNative(JNIEnv *env, jobject instance,
                                                      jlong _nativeAddress, jstring _name_);
JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_destoryOutSteamNative(JNIEnv *env, jobject instance,
                                                       jlong _nativeAddress, jstring _name_);

JNIEXPORT jint JNICALL
Java_com_sve_engine_SVEEngineJNI_getTexIdNative(JNIEnv *env, jobject instance, jlong _nativeAddress);

JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_openFaceBeautyNative(JNIEnv *env, jobject instance,
                                                      jlong _nativeAddress, jint _lows);

JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_updateFilterNative(JNIEnv *env, jobject instance, jlong _nativeAddress,
                                                    jint _type, jint _smooth);
JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_loadGameNative(JNIEnv *env, jobject instance, jlong _nativeAddress,
                                                jstring _path_);
JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_startGameNative(JNIEnv *env, jobject instance, jlong _nativeAddress, jstring _path_,jint _sum);

JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_touchPosNative(JNIEnv *env, jobject instance, jlong _nativeAddress,
                                                jfloat _x, jfloat _y);
JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_walletPosNative(JNIEnv *env, jobject instance, jlong _nativeAddress,
                                                 jfloat _x, jfloat _y);
JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_stopGameNative(JNIEnv *env, jobject instance,
                                                jlong _nativeAddrees);
JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_setTotalMoneyNative(JNIEnv *env,  jobject instance, jlong _nativeAddress,
                                                     jstring _text_);
JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_removeOnePacketNative(JNIEnv *env,  jobject instance,
                                                       jlong _nativeAddress, jint _value);

#ifdef __cplusplus
}
#endif

#endif //_Included_com_sve_engine_SVEEngineJNI

