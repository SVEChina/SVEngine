package com.sve.engine;


import android.graphics.PixelFormat;
import android.graphics.SurfaceTexture;
import android.media.ImageReader;
import android.opengl.EGLContext;
import android.opengl.GLES20;
import android.os.Handler;
import android.os.Looper;
import android.view.Surface;

import com.sve.engine.gl.SVEGLUtils;
import com.sve.engine.gl.SVERenderHelper;

import java.nio.ByteBuffer;


public class SVEEngine {

    private SVECamera mCamera;
    private SVEGLCore m_glCore = null;
    private SVEWindowSurface m_surface = null;
    private SVEImageRead m_imageRead = null;
    private int m_width = 720;
    private int m_height = 1280;
    private String mName = "camera";
    private onSVImageBufferCallBack mCallBack;
    private SVERenderHelper mRenderHelpe = null;
    private SurfaceTexture mSurfaceTexture = null;
    private int m_textOutId = 0;
    private static SVEEngine mSVEEngineSingleton = null;
    private boolean isSuspend = true;
    private boolean isFaceBeauty = false;
    private boolean isInit = false;
    private boolean isEngine = false;
    private SVEEngineJNI m_SVEJni=null;

    public enum FILTER_TYPE {
        SVI_EBEAUTY_FILTER,          //美颜
        SVI_SHADOWS_FILTER,
        SVI_CONTRAST_FILTER,
        SVI_SATURATION_FILTER,
        SVI_ACUTANCE_FILTER,
        SVI_BRIGHTNESS_FILTER,
        SVI_WHITENING_FILTER,   //美白
        SVI_HIGHLIGHTS_FILTER,
        SVI_GAMMA_FILTER,
        SVI_REDSHIFT_FILTER,
        SVI_GREENSHIFT_FILTER,
        SVI_BLUESHIFT_FILTER,
        SVI_SDREDSHIFT_FILTER,
        SVI_SDGREENSHIFT_FILTER,
        SVI_SDBLUESHIFT_FILTER,
        SVI_HHREDSHIFT_FILTER,
        SVI_HHGREENSHIFT_FILTER,
        SVI_HHBLUESHIFT_FILTER,
        SVI_TEMPERATURE_FILTER,
        SVI_TINT_FILTER
    }

    public interface onSVImageBufferCallBack {
        public void onSVBufferCallBack(ByteBuffer _data, int width, int height, int stride);
    }

    public static SVEEngine getinstance() {
        if (mSVEEngineSingleton == null) {
            mSVEEngineSingleton = new SVEEngine();
        }
        return mSVEEngineSingleton;
    }

    public SVEEngine() {
        m_SVEJni=new SVEEngineJNI();
    }


    public void init(final String _path, final int _width, final int _height, final int inputImgW, final int inputImgH) {
        if (mRenderHelpe != null) {
            return;
        }
        if (mRenderHelpe == null) {
            m_width = _width;
            m_height = _height;
            m_imageRead = new SVEImageRead(_width, _height, new onSVImageBufferCallBack() {
                @Override
                public void onSVBufferCallBack(ByteBuffer _data, int width, int height, int stride) {
                    if (!isEngine) {
                        isEngine = true;
                    }
                    if (isSuspend) {
                        return;
                    }
                    if (mCallBack != null) {
                        mCallBack.onSVBufferCallBack(_data, width, height, stride);
                    }
                }
            });

            init();
            addResPath(_path);
            setSurface(m_imageRead.getInputSurface());
            startEngine();
            m_glCore = new SVEGLCore();
            m_glCore.makeNothingCurrent();
            createRenderGL(m_glCore.getEGLContext(), m_width, m_height);
            createScene();
            createRenderTarget(0, 0, m_width, m_height);
            createSteam(1, 0, inputImgW, inputImgH, 270);
            m_SVEJni.createSteamOutTex(mName, 1, 0, _width, _height, 0);
            isInit = true;
        }
    }

    public void init(final String _path, final int _width, final int _height,Surface surface){
        m_width = _width;
        m_height = _height;
        init();
        addResPath(_path);
        setSurface(surface);
        startEngine();
        m_glCore = new SVEGLCore();
        m_glCore.makeNothingCurrent();
        createRenderGL(m_glCore.getEGLContext(), m_width, m_height);
        createScene();
        createRenderTarget(0, 0, m_width, m_height);
        isInit = true;
    }

    public void setCallBackImagBuffer(onSVImageBufferCallBack _callBack) {
        this.mCallBack = _callBack;
    }

    private void createSuffaceOffSize() {
        ImageReader mImageReader = ImageReader.newInstance(720, 1280, PixelFormat.RGBA_8888, 2);
        m_surface = new SVEWindowSurface(m_glCore, mImageReader.getSurface(), false);
    }

    public void setSurfaceTextureDraw(final SurfaceTexture _surfaceTexture) {
        if (m_surface == null) {
            m_surface = new SVEWindowSurface(m_glCore, _surfaceTexture);
            m_surface.makeCurrent();
            mRenderHelpe = new SVERenderHelper();
            mRenderHelpe.setInputDataFormat(m_width, m_height, 0);
            m_surface.swapBuffers();
        } else {
            m_surface.releaseEglSurface();
            m_surface.createWindowSurface(_surfaceTexture);
        }
    }

    public void destorySuface() {
        m_surface.releaseEglSurface();
    }

    private void init() {
        m_SVEJni.initEngine();
    }

    private void startEngine() {
        m_SVEJni.statEngine();
    }

    private void stopEngine() {
        m_SVEJni.stopEngine();
    }

    private void addResPath(String _path) {
        m_SVEJni.addResPath(_path);
    }

    public void suspend() {

        if (!isInit) {
            return;
        }
        if (isSuspend) {
            return;
        }
        isSuspend = true;
        m_SVEJni.suspend();

    }

    public void resume() {

        if (!isInit) {
            return;
        }
        if (!isSuspend) {
            return;
        }
        isSuspend = false;
        m_SVEJni.resume();

    }

    private void destoryRenderGL() {
        m_SVEJni.destoryRenderGL();
    }

    private void createRenderTarget(int _fboid, int _coloid, int _width, int _height) {
        m_SVEJni.createRenderTarget(_fboid, _coloid, _width, _height);
    }

    private void createRenderTargetTexture(int _w, int _h, int _texId) {
        m_SVEJni.createRenderTargetTexture(_w, _h, _texId);
    }

    private void createScene() {
        m_SVEJni.createScene();
    }

    public void drawTex(int x,int y,final int w, final int h) {
        if (!isEngine) {
            return;
        }
        if (m_surface == null) {
            return;
        }
        if (!isInit) {
            return;
        }
        if (isSuspend) {
            return;
        }
        if (m_SVEJni.getTexId() == 0) {
            return;
        }
        if (mRenderHelpe == null || m_surface == null || !m_surface.makeCurrent()) {
            return;
        }

        mRenderHelpe.drawTexture(m_SVEJni.getTexId(), 0,x,y, w, h);
        m_surface.swapBuffers();
    }

    private void destoryScene() {
        m_SVEJni.destoryScene();
    }

    private void setSurface(Surface _surface) {
        m_SVEJni.setSurface(_surface);
    }

    private void createRenderGL(EGLContext _context, int _w, int _h) {
        m_SVEJni.createRenderGL(_context, _w, _h);
    }

    private void destoryRenderTarget() {
        m_SVEJni.destoryRenderTarget();
    }

    private void createSteam(int _type, int _formate, int _width, int _height, int _angle) {
        m_SVEJni.createSteam(mName, _type, _formate, _width, _height, _angle);
    }

    private ByteBuffer mDataBuffer = null;
    private byte[] mFlipCameraData = null;

    public void pushSteam(byte[] _srcData, int _format, int _width, int _height, boolean isMirror, int _angle) {
        if (!isInit) {
            return;
        }
        if (isSuspend) {
            return;
        }

        int size = (int) (_width * _height * 1.5f);
        if (mFlipCameraData == null) {
            mFlipCameraData = new byte[size];
        }

        if (mDataBuffer == null) {
            mDataBuffer = ByteBuffer.allocateDirect(size);//分配空间
        }
        mDataBuffer.rewind();
        mDataBuffer.position(0);
        if (isMirror) {
            YUVMirror.yuv90MirrorJava(_srcData, mFlipCameraData, _width, _height);
            mDataBuffer.put(mFlipCameraData);
        } else {
            mDataBuffer.put(_srcData);
        }
        mDataBuffer.position(0);

        m_SVEJni.pushSteam(mName, mDataBuffer, _format, _width, _height, _angle);
    }

    public void openFaceBeauty(int _lows) {
        if (!isInit) {
            return;
        }
        if (isSuspend) {
            return;
        }
        if (isFaceBeauty) {
            return;
        }
        isFaceBeauty = true;
        m_SVEJni.openFaceBeauty(_lows);
    }


    public void updateFilter(FILTER_TYPE _type, int _smooth) {
        if (!isInit) {
            return;
        }
        if (isSuspend) {
            return;
        }
        m_SVEJni.updateFilter(_type.ordinal(), _smooth);
    }

    public void closeFaceBeauty() {
        if (!isInit) {
            return;
        }
        if (isSuspend) {
            return;
        }
        isFaceBeauty = false;
        m_SVEJni.closeFaceBeauty();
    }

    public void createWatermark(ByteBuffer buffer,int width,int height){
        m_SVEJni.createWatermark(buffer,width,height);
    }

}