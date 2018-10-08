package com.sve.engine;

import android.os.Build;
import android.view.Surface;

import android.opengl.EGLContext;

import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.nio.ByteBuffer;


public class SVEEngineJNI {

    public interface onSVENativeCallBackEvent{
         public void  gameCallBack(String _param);
    }
    static {
        System.loadLibrary("SVEngine");             //模块名称
        System.loadLibrary("SVEEngineNative");         //模块名称
    }


    private  long mSVEAddress;

    private onSVENativeCallBackEvent m_CallBack=null;

    public void setCallBack(onSVENativeCallBackEvent _CallBack){
        m_CallBack=_CallBack;
    }

    public  void initEngine() {
        mSVEAddress = initNative();
    }

    public  void statEngine() {

        startEngineNative(mSVEAddress);
    }

    public  void setSurface(Surface _surface) {
        setSurfaceNative(mSVEAddress, _surface);
    }

    public  void addResPath(String _path) {
        addResPathNative(mSVEAddress, _path);
    }

    public  void createScene() {
        createSceneNative(mSVEAddress);
    }

    public void touchPos(float _x,float _y){
        touchPosNative(mSVEAddress,_x,_y);
    }

    public  void createRenderGL(EGLContext _context, int _w, int _h) {
        long fieldValue = 0;

        if (android.os.Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            fieldValue = _context.getNativeHandle();
        } else {
            try {
                Field privateStringMethod = _context.getClass().getSuperclass().getDeclaredField("mHandle");
                privateStringMethod.setAccessible(true);
                fieldValue = (long) privateStringMethod.getLong(_context);
            } catch (NoSuchFieldException e) {
                fieldValue = 0;
                e.printStackTrace();
            } catch (IllegalAccessException e) {
                fieldValue = 0;
                e.printStackTrace();
            }
        }
        createRenderGLNative(mSVEAddress, fieldValue, _w
                , _h);
    }

    public  void createRenderTarget(int _fboid, int _colorid, int _w, int _h) {
        createRenderTargetNative(mSVEAddress, _fboid, _colorid, _w, _h);
    }

    public  void createRenderTargetTexture(int _w, int _h, int _texId) {
        createRenderTargetTextureNative(mSVEAddress, _w, _h, _texId);
    }

    public  void createSteam(String _name, int _type, int _formate, int _width, int _height, int _angle) {
        createSteamNative(mSVEAddress, _name, _type, _formate, _width, _height, _angle);
    }

    public  void pushSteam(String _name, ByteBuffer _srcData, int _format, int _width, int _height, int _angle) {
        pushSteamNative(mSVEAddress, _name, _srcData, _format, _width, _height, _angle);
    }

    public  void destoryRenderGL() {
        destoryRenderGLNative(mSVEAddress);
    }

    public  void destoryRenderTarget() {
        destoryRenderTargetNative(mSVEAddress);
    }

    public  void destoryScene() {
        destorySceneNative(mSVEAddress);
    }

    public  void suspend() {
        suspendNative(mSVEAddress);
    }

    public  void resume() {
        resumeNative(mSVEAddress);
    }

    public  void stopEngine() {
        stopEngineNative(mSVEAddress);
    }

    public  void createSteamOutTex(String _name, int _type, int _formate, int _width, int _height, int _angle) {
        createSteamOutTexNaitve(mSVEAddress, _name, _type, _formate, _width, _height, _angle);
    }

    public  void openFaceBeauty(int _lows) {
        openFaceBeautyNative(mSVEAddress, _lows);
    }

    public  void updateFilter(int _type, int _smooth) {
        updateFilterNative(mSVEAddress, _type, _smooth);

    }

    public  void closeFaceBeauty() {
        closeFaceBeautyNative(mSVEAddress);
    }


    public  void destoryInSteam(String _name) {
        destoryInSteamNative(mSVEAddress, _name);
    }

    public  void destoryOutSteamNative(String _name) {
        destoryOutSteamNative(mSVEAddress, _name);
    }

    public  int getTexId() {
        return getTexIdNative(mSVEAddress);
    }

    public static native long initNative();

    public static native void startEngineNative(long _nativeAddress);

    public static native void setSurfaceNative(long _nativeAddress, Surface _suface);

    public static native void addResPathNative(long _nativeAddress, String _path);

    public static native void createSceneNative(long _nativeAddress);

    public static native void createRenderGLNative(long _nativeAddress, long _contextHandle, int _w, int _h);

    public static native void createRenderTargetNative(long _nativeAddress, int _fboid, int _colorid, int _w, int _h);

    public static native void createRenderTargetTextureNative(long _nativeAddress, int _w, int _h, int _textureId);

    public static native void openFaceBeautyNative(long _nativeAddress, int _lows);

    public static native void updateFilterNative(long _nativeAddress, int _type, int _smooth);

    public static native void closeFaceBeautyNative(long _nativeAddress);

    public static native void destoryRenderGLNative(long _nativeAddress);

    public static native void destoryRenderTargetNative(long _nativeAddress);

    public static native void destorySceneNative(long _nativeAddress);

    public static native void suspendNative(long _nativeAddress);

    public static native void resumeNative(long _nativeAddress);

    public static native void stopEngineNative(long _nativeAddress);

    public static native void createSteamNative(long _nativeAddress, String _name, int _type, int _formate, int _width, int _height, int _angle);

    public static native void pushSteamNative(long _nativeAddress, String _name, ByteBuffer _srcData, int _format, int _width, int _height, int _angle);

    public static native void createSteamOutTexNaitve(long _nativeAddress, String _name, int _type, int _formate, int _width, int _height, int _angle);

    public static native void destoryInSteamNative(long _nativeAddress, String _name);

    public static native void destoryOutSteamNative(long _nativeAddress, String _name);

    public static native int getTexIdNative(long _nativeAddress);

    public static native void touchPosNative(long _nativeAddress,float _x,float _y);

}
