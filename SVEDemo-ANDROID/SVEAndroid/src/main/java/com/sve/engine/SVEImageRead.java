package com.sve.engine;

import android.graphics.Bitmap;
import android.graphics.PixelFormat;
import android.media.Image;
import android.media.ImageReader;
import android.os.Build;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.os.SystemClock;
import android.util.Log;
import android.view.Surface;

import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;


public class SVEImageRead {
    private static final String TAG = "ImageEncoderCore";
    private static final boolean VERBOSE = true;

    private static final int MAX_IMAGE_NUMBER = 2;
    private static final int ENCODER_BITMAP = 0;

    private int mWidth;
    private int mHeight;

    private int[] mPixelData;
    private SVEEngine.onSVImageBufferCallBack mCallBack=null;
    private ImageReader mImageReader=null;
    private Surface mInputSurface=null;

    public SVEImageRead(int width, int height,SVEEngine.onSVImageBufferCallBack _CallBack) {
        this.mWidth = width;
        this.mHeight = height;
        this.mCallBack=_CallBack;
        this.mPixelData = new int[width * height];
        this.mImageReader = ImageReader.newInstance(width, height, PixelFormat.RGBA_8888, MAX_IMAGE_NUMBER);

        mInputSurface = mImageReader.getSurface();

        mImageReader.setOnImageAvailableListener(new ImageReader.OnImageAvailableListener() {
            @Override
            public void onImageAvailable(ImageReader reader) {
                Image image = reader.acquireNextImage();//获取下一个
                Image.Plane[] planes = image.getPlanes();
                int width = image.getWidth();//设置的宽
                int height = image.getHeight();//设置的高
                int pixelStride = planes[0].getPixelStride();//内存对齐参数
                int rowStride = planes[0].getRowStride();
                if(mCallBack!=null) {
                    mCallBack.onSVBufferCallBack(image.getPlanes()[0].getBuffer(),width,height,rowStride/pixelStride);
                }
//                int rowPadding = rowStride - pixelStride * width;
////
////                byte[] data = getBuffer(rowStride * height);//获得byte
////                ByteBuffer buffer = planesß[0].getBuffer();//获得buffer
////                buffer.get(data);//将buffer数据写入byte中
                image.close();//用完需要关闭
            }
        }, null);
    }


    /**
     * Returns the encoder's input surface.
     */
    public Surface getInputSurface() {
        return mInputSurface;
    }

    /**
     * Releases encoder resources.
     */
    public void release() {
        if (VERBOSE) Log.d(TAG, "releasing encoder objects");
        if (mImageReader != null) {
            mImageReader.close();
            mImageReader = null;
        }

    }

}
