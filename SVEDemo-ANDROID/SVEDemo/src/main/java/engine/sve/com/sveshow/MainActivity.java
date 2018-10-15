package engine.sve.com.sveshow;

import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.ImageFormat;
import android.graphics.SurfaceTexture;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.hardware.Camera;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.TextureView;
import android.widget.ImageView;

import com.sve.engine.SVECamera;
import com.sve.engine.SVEEngine;

import java.io.IOException;
import java.io.InputStream;
import java.nio.ByteBuffer;


public class MainActivity extends AppCompatActivity {
    private SVECamera realCamera; //相机
    private boolean isOpenCamera = false;
    SVEEngine mSVEngine;
    private ImageView mImageView;
    private Bitmap bitmap;
    private int m_width;
    private int m_height;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);


        setContentView(R.layout.activity_main);
        mImageView = findViewById(R.id.svimageId);
        bitmap = Bitmap.createBitmap(768, 1280, Bitmap.Config.ARGB_8888);
        TextureView mTextureView = (TextureView) findViewById(R.id.textureView1);
        mTextureView.setOpaque(false);

        mSVEngine = SVEEngine.getinstance();

        mSVEngine.init(Environment.getExternalStorageDirectory().getPath() + "/sve/res/", 720, 1280, 1280, 720);
        mSVEngine.resume();

        mSVEngine.openFaceBeauty(1);
        mSVEngine.updateFilter(SVEEngine.FILTER_TYPE.SVI_EBEAUTY_FILTER, 95);
        AssetManager asm = getAssets();
        InputStream inputStream = null;
        try {
            inputStream = asm.open("sve-icon.png");
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
        Drawable drawble = Drawable.createFromStream(inputStream, null);
        Bitmap bitmapwatermark = ((BitmapDrawable) drawble).getBitmap();
        ByteBuffer buffer = ByteBuffer.allocateDirect(bitmapwatermark.getWidth() * bitmapwatermark.getHeight() * 4);
        bitmapwatermark.copyPixelsToBuffer(buffer);
        mSVEngine.createWatermark(buffer, bitmapwatermark.getWidth(), bitmapwatermark.getHeight());

//                mSVEngine.updateFilter(SVEEngine.FILTER_TYPE.SVI_EBEAUTY_FILTER,95);
//                mSVEngine.updateFilter(SVEEngine.FILTER_TYPE.SVI_ACUTANCE_FILTER,0);
//                mSVEngine.updateFilter(SVEEngine.FILTER_TYPE.SVI_WHITENING_FILTER,0);
//                mSVEngine.updateFilter(SVEEngine.FILTER_TYPE.SVI_CONTRAST_FILTER,0);


        mTextureView.setSurfaceTextureListener(new TextureView.SurfaceTextureListener() {
            @Override
            public void onSurfaceTextureAvailable(final SurfaceTexture surface, int width, int height) {
                m_width = width;
                m_height = height;
                mSVEngine.setSurfaceTextureDraw(surface);
                mSVEngine.setCallBackImagBuffer(new SVEEngine.onSVImageBufferCallBack() {
                    @Override
                    public void onSVBufferCallBack(ByteBuffer _data, int width, int height, int stride) {
                        bitmap.copyPixelsFromBuffer(_data);
                        mImageView.setImageBitmap(bitmap);
                    }
                });
            }

            @Override
            public void onSurfaceTextureSizeChanged(SurfaceTexture surface, int width, int height) {

            }

            @Override
            public boolean onSurfaceTextureDestroyed(SurfaceTexture surface) {
                return false;
            }

            @Override
            public void onSurfaceTextureUpdated(SurfaceTexture surface) {


            }
        });

    }


    private boolean isFront = true;

    //开启相机
    public void openCamera(boolean isFront) {
        if (isOpenCamera) {
            return;
        }
        if (realCamera == null) {
            try {
                realCamera = new SVECamera(this);
                realCamera.setPreviewSize(1280, 720);
                this.isFront = isFront;
                realCamera.open(ImageFormat.NV21, isFront);

                realCamera.start();
                isOpenCamera = true;
                realCamera.registerCameraPreviewListener(mPreviewCallback);
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    public void closeCamera() {
        isOpenCamera = false;
        if (realCamera != null) {
            realCamera.unRegisterCameraPreviewListener();
            realCamera.release();
            realCamera = null;
        }
    }

    private int mCameraWidth, mCameraHeight;

    private boolean flagEx = false;
    private Camera.PreviewCallback mPreviewCallback = new Camera.PreviewCallback() {
        @Override
        public void onPreviewFrame(byte[] data, Camera camera) {
            Camera.Parameters param = camera.getParameters();
            Camera.Size tSize = param.getPreviewSize();
            mCameraHeight = tSize.height;
            mCameraWidth = tSize.width;
            if (mSVEngine != null) {
                mSVEngine.pushSteam(data, 0, mCameraWidth, mCameraHeight, isFront, 90);
                mSVEngine.drawTex(0, -(m_width * mCameraWidth / mCameraHeight - m_height) / 2, m_width, m_width * mCameraWidth / mCameraHeight);
            }
            int linght = 0;
            for (int i = 0; i < 50; i++) {
                for (int j = 0; j < 50; j++) {
                    int index = (mCameraWidth * (mCameraHeight / 2 - 25 + i) - mCameraWidth / 2 - 25 + j);
                    linght = linght + (data[index] + 128);
                }
            }
            linght /= 2500;


            System.out.println("light " + linght);
            if (!flagEx) {
                if (linght < 120) {
                    realCamera.setExposureCompensation(-3);
                } else if (linght > 150) {
                    realCamera.setExposureCompensation(3);
                }
                flagEx = true;
            }
            camera.addCallbackBuffer(data);
        }
    };

    @Override
    protected void onResume() {
        super.onResume();
        if (mSVEngine != null) {
            mSVEngine.resume();
        }
        openCamera(isFront);//开启前置摄像头

    }

    @Override
    protected void onPause() {
        super.onPause();
        mSVEngine.suspend();
        closeCamera();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mSVEngine.destorySuface();
        mSVEngine.suspend();
        closeCamera();
        mPreviewCallback = null;
    }
}