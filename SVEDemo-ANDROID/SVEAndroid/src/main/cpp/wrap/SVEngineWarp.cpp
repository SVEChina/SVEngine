//
// Created by 徐子昱 on 2018/6/27.
//

#include <memory>
#include "SVEngineWarp.h"
#include "app/SVInst.h"
#include "SVEngineSystem.h"


extern "C"
JNIEXPORT jlong JNICALL
Java_com_sve_engine_SVEEngineJNI_initNative(JNIEnv *env, jobject instance) {
    SVEngineSystem *app = new SVEngineSystem();
    app->initEngine();
    return reinterpret_cast<jlong>(app);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_startEngineNative(JNIEnv *env, jobject instance,
                                                   jlong _nativeAddress) {
    SVEngineSystem *app = reinterpret_cast<SVEngineSystem *>(_nativeAddress);
    if(app) {
        app->startEngine();
    }
}


extern "C"
JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_setSurfaceNative(JNIEnv *env, jobject instance,
                                                  jlong _nativeAddress,
                                                  jobject _suface) {
    ANativeWindow *t_pWidows = ANativeWindow_fromSurface(env, _suface);
    SVEngineSystem *app = reinterpret_cast<SVEngineSystem *>(_nativeAddress);
    app->setWindows(t_pWidows);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_addResPathNative(JNIEnv *env, jobject instance,
                                                  jlong _nativeAddress,
                                                  jstring _path_) {
    const char *_path = env->GetStringUTFChars(_path_, 0);
    SVEngineSystem *app = reinterpret_cast<SVEngineSystem *>(_nativeAddress);
    app->addResPath(SVString(_path));
    env->ReleaseStringUTFChars(_path_, _path);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_createSceneNative(JNIEnv *env, jobject instance,
                                                   jlong _nativeAddress) {
    SVEngineSystem *app = reinterpret_cast<SVEngineSystem *>(_nativeAddress);
    app->createScene(nullptr, nullptr);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_createRenderGLNative(JNIEnv *env, jobject instance,
                                                      jlong _nativeAddress, jlong _contextHandle,
                                                      jint _w, jint _h) {
    EGLContext t_context;
    if (_contextHandle == 0) {
        t_context = eglGetCurrentContext();
    } else {
        t_context = reinterpret_cast<EGLContext>(_contextHandle);
    }

    SVEngineSystem *app = reinterpret_cast<SVEngineSystem *>(_nativeAddress);
    app->createRenderGL(3, t_context, _w, _h);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_createRenderTargetNative(JNIEnv *env, jobject instance,
                                                          jlong _nativeAddress, jint _fboid,
                                                          jint _colorid, jint _w, jint _h) {
    SVEngineSystem *app = reinterpret_cast<SVEngineSystem *>(_nativeAddress);
    app->createRenderTarget(_fboid, _colorid, _w, _h);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_createRenderTargetTextureNative(JNIEnv *env, jobject instance,
                                                                 jlong _nativeAddress, jint _w,
                                                                 jint _h, jint _textureId) {
    SVEngineSystem *app = reinterpret_cast<SVEngineSystem *>(_nativeAddress);
    app->createRenderTextureTarget(_textureId, _w, _h);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_destoryRenderGLNative(JNIEnv *env, jobject instance,
                                                       jlong _nativeAddress) {
    SVEngineSystem *app = reinterpret_cast<SVEngineSystem *>(_nativeAddress);
    app->destoryRenderGL();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_destoryRenderTargetNative(JNIEnv *env, jobject instance,
                                                           jlong _nativeAddress) {
    SVEngineSystem *app = reinterpret_cast<SVEngineSystem *>(_nativeAddress);
    app->destoryRenderTarget();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_destorySceneNative(JNIEnv *env, jobject instance,
                                                    jlong _nativeAddress) {
    SVEngineSystem *app = reinterpret_cast<SVEngineSystem *>(_nativeAddress);
    app->destoryScene();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_suspendNative(JNIEnv *env, jobject instance,
                                               jlong _nativeAddress) {
    SVEngineSystem *app = reinterpret_cast<SVEngineSystem *>(_nativeAddress);
    app->suspend();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_resumeNative(JNIEnv *env, jobject instance, jlong _nativeAddress) {
    SVEngineSystem *app = reinterpret_cast<SVEngineSystem *>(_nativeAddress);
    app->resume();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_stopEngineNative(JNIEnv *env, jobject instance,
                                                  jlong _nativeAddress) {
    SVEngineSystem *app = reinterpret_cast<SVEngineSystem *>(_nativeAddress);
    app->stopEngine();
    delete app;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_createSteamNative(JNIEnv *env, jobject instance,
                                                   jlong _nativeAddress,
                                                   jstring _name_, jint _type, jint _formate,
                                                   jint _width,
                                                   jint _height, jint _angle) {

    const char *_name = env->GetStringUTFChars(_name_, 0);
    SVEngineSystem *app = reinterpret_cast<SVEngineSystem *>(_nativeAddress);
    app->createInStream(SVString(_name), _type, _formate, _width, _height, _angle);
    env->ReleaseStringUTFChars(_name_, _name);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_pushSteamNative(JNIEnv *env, jobject instance,
                                                 jlong _nativeAddress,
                                                 jstring _name_, jobject _srcData, jint _format,
                                                 jint _width, jint _height, jint _angle) {
    const char *_name = env->GetStringUTFChars(_name_, 0);
    SVEngineSystem *app = reinterpret_cast<SVEngineSystem *>(_nativeAddress);
    void *t_buf_point = env->GetDirectBufferAddress(_srcData);
    app->pushStreamData(SVString(_name), static_cast<char *>(t_buf_point), _width, _height, _format,
                        _angle);
    env->ReleaseStringUTFChars(_name_, _name);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_createSteamOutTexNaitve(JNIEnv *env, jobject instance,
                                                         jlong _nativeAddress, jstring _name_,
                                                         jint _type, jint _formate, jint _width,
                                                         jint _height, jint _angle) {

    const char *_name = env->GetStringUTFChars(_name_, 0);
    SVEngineSystem *app = reinterpret_cast<SVEngineSystem *>(_nativeAddress);
    app->createOutTexture(SVString(_name), 0, 0, _width, _height, _angle);

    env->ReleaseStringUTFChars(_name_, _name);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_destoryInSteamNative(JNIEnv *env, jobject instance,
                                                      jlong _nativeAddress, jstring _name_) {
    const char *_name = env->GetStringUTFChars(_name_, 0);
    SVEngineSystem *app = reinterpret_cast<SVEngineSystem *>(_nativeAddress);
    app->destoryInStream(SVString(_name));
    env->ReleaseStringUTFChars(_name_, _name);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_destoryOutSteamNative(JNIEnv *env, jobject instance,
                                                       jlong _nativeAddress, jstring _name_) {

    const char *_name = env->GetStringUTFChars(_name_, 0);
    SVEngineSystem *app = reinterpret_cast<SVEngineSystem *>(_nativeAddress);
    app->destoryOutstream(SVString(_name));
    env->ReleaseStringUTFChars(_name_, _name);
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_sve_engine_SVEEngineJNI_getTexIdNative(JNIEnv *env, jobject instance,
                                                jlong _nativeAddress) {
    SVEngineSystem *app = reinterpret_cast<SVEngineSystem *>(_nativeAddress);
    return app->getTexId();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_openFaceBeautyNative(JNIEnv *env, jobject instance,
                                                      jlong _nativeAddress, jint _lows) {
    SVEngineSystem *app = reinterpret_cast<SVEngineSystem *>(_nativeAddress);
    app->openFaceBeauty(_lows);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_updateFilterNative(JNIEnv *env, jobject instance,
                                                    jlong _nativeAddress,
                                                    jint _type, jint _smooth) {
    SVEngineSystem *app = reinterpret_cast<SVEngineSystem *>(_nativeAddress);
    app->updateFilter(_type, _smooth);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_sve_engine_SVEEngineJNI_createWatermarkNative(JNIEnv *env,  jobject instance, jlong _nativeAddress,
                                                 jobject _buffer, jint width, jint height) {
    void *t_buf_point = env->GetDirectBufferAddress(_buffer);
    SVEngineSystem *app = reinterpret_cast<SVEngineSystem *>(_nativeAddress);
    app->createWatermark(t_buf_point,width,height);
}