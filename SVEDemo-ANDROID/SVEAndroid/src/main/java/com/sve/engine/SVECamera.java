package com.sve.engine;

import android.app.Activity;
import android.graphics.ImageFormat;
import android.graphics.Point;
import android.graphics.Rect;
import android.graphics.SurfaceTexture;
import android.hardware.Camera;
import android.opengl.GLES20;
import android.os.Build;
import android.view.Surface;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Iterator;
import java.util.List;

/**
 * Created by menghai on 2017/3/1.
 */

public class SVECamera {

    /**
     * 最大宽高比差
     */
    private static final double MAX_ASPECT_DISTORTION = 0.15;

    /**
     * 最小预览界面的分辨率
     */
    private static final int MIN_PREVIEW_PIXELS = 480 * 320;
    private static final int DEFAULT_CAMERA_WIDTH = 1280;
    private static final int DEFAULT_CAMERA_HEIGHT = 720;

    private Camera camera;
    private SurfaceTexture cameraSurfaceTexture;
    int[] textures = null;

    private int format = ImageFormat.NV21;
    private int cameraDisplayOrientation = 90;
    private final int[] previewSize = new int[]{DEFAULT_CAMERA_WIDTH, DEFAULT_CAMERA_HEIGHT};

    private boolean isPreviewing = false;
    private TakePictureInterface mTakePictureInterface;
    private Activity activity;

    public SVECamera(Activity activity) {
        this.activity = activity;
    }

    public void open() {
        int cameraId = getFrontCamera();
        camera = Camera.open(cameraId);
        cameraDisplayOrientation = getCameraDisplayOrientation(cameraId);
    }

    public void open(int format, boolean isfront) {
        this.format = format;
        int cameraId;
        if (isfront) {
            cameraId = getFrontCamera();
            camera = Camera.open(cameraId);
        } else {
            cameraId = getBackCamera();
            camera = Camera.open(cameraId);
        }
        cameraDisplayOrientation = getCameraDisplayOrientation(cameraId);
       // doAutoFocus();
    }

    public int getCameraWidth() {
        return previewSize[0];
    }

    public int getCameraHeight() {
        return previewSize[1];
    }

    public int getCameraFormat() {
        return format;
    }

    public int[] getPreviewSize() {
        return previewSize;
    }

    public int getCameraDisplayOrientation() {
        return cameraDisplayOrientation;
    }

    private int getBackCamera() {
        Camera.CameraInfo cameraInfo = new Camera.CameraInfo();
        final int numberOfCameras = Camera.getNumberOfCameras();

        for (int i = 0; i < numberOfCameras; ++i) {
            Camera.getCameraInfo(i, cameraInfo);
            if (cameraInfo.facing == Camera.CameraInfo.CAMERA_FACING_BACK) {
                return i;
            }
        }

        return Camera.CameraInfo.CAMERA_FACING_BACK;
    }

    private int getFrontCamera() {
        Camera.CameraInfo cameraInfo = new Camera.CameraInfo();
        final int numberOfCameras = Camera.getNumberOfCameras();
        for (int i = 0; i < numberOfCameras; ++i) {
            Camera.getCameraInfo(i, cameraInfo);
            if (cameraInfo.facing == Camera.CameraInfo.CAMERA_FACING_FRONT) {
                return i;
            }
        }
        return Camera.CameraInfo.CAMERA_FACING_FRONT;
    }


    /**
     * 设置相机输出尺寸
     * setPreviewSize(1280, 720)
     *
     * @param width  宽度
     * @param height 高度
     */
    public void setPreviewSize(int width, int height) {
        this.previewSize[0] = width;
        this.previewSize[1] = height;
    }

    public void setExposureCompensation(int _value){
//        Camera.Parameters parameters = camera.getParameters();
//        try {
//            parameters.setExposureCompensation(_value);
//        } catch (Exception e) {
//            // TODO: handle exception
//            e.printStackTrace();
//        }
//        camera.setParameters(parameters);
    }

    public void start(SurfaceTexture surface) throws IOException {
        Camera.Parameters parameters = camera.getParameters();
        final List<String> focusModes = parameters.getSupportedFocusModes();
        // 连续聚焦
        if (focusModes.contains(Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO)) {
            parameters.setFocusMode(Camera.Parameters.FOCUS_MODE_CONTINUOUS_VIDEO);
        }

        if (focusModes.contains(Camera.Parameters.FLASH_MODE_AUTO)) {
            parameters.setFocusMode(Camera.Parameters.FLASH_MODE_AUTO);
        }
//        previewRequestBuilder.set(CaptureRequest.CONTROL_AE_MODE,
//                CaptureRequest.CONTROL_AE_MODE_ON_AUTO_FLASH);
        try {
            int value =parameters.getMaxExposureCompensation();
            parameters.setExposureCompensation(0);
        } catch (Exception e) {
            // TODO: handle exception
            e.printStackTrace();
        }

        // 自动聚焦
        if (focusModes.contains(Camera.Parameters.FOCUS_MODE_AUTO)) {
            parameters.setFocusMode(Camera.Parameters.FOCUS_MODE_AUTO);
        }

        if (focusModes.contains(Camera.Parameters.FOCUS_MODE_AUTO)) {
            parameters.setFocusMode(Camera.Parameters.FOCUS_MODE_AUTO);
        }

        parameters.setPreviewFormat(format);

        Camera.Size CameraSize = findBestPreviewResolution(parameters);
        if (CameraSize != null) {
            parameters.setPreviewSize( previewSize[0], previewSize[1]); //预览
          //  parameters.setPictureSize(previewSize[0], previewSize[1]); //拍照
            previewSize[0] = CameraSize.width;
            previewSize[1] = CameraSize.height;
        }

//        if (parameters.getMaxNumMeteringAreas() > 0) {
//            List<Camera.Area> areas = new ArrayList<Camera.Area>();
//
//            DisplayMetrics dm = activity.getResources().getDisplayMetrics();
//            int screenWidth = dm.widthPixels;
//            int screenHeight = dm.heightPixels;
//
//            //xy变换了
//            int rectY = -screenWidth / 2 * 2000 / screenWidth + 1000;
//            int rectX = screenHeight / 2 * 2000 / screenHeight - 1000;
//
//            int left = rectX < -900 ? -1000 : rectX - 100;
//            int top = rectY < -900 ? -1000 : rectY - 100;
//            int right = rectX > 900 ? 1000 : rectX + 100;
//            int bottom = rectY > 900 ? 1000 : rectY + 100;
//            Rect area1 = new Rect(left, top, right, bottom);
//            areas.add(new Camera.Area(area1, 800));
//            parameters.setMeteringAreas(areas);
//        }

        //设置以最大帧率回调
        final List<int[]> supportedFpsRange = parameters.getSupportedPreviewFpsRange();
        if (supportedFpsRange != null && supportedFpsRange.size() > 0) {
            final int[] max_fps = supportedFpsRange.get(supportedFpsRange.size() - 1);
            parameters.setPreviewFpsRange(max_fps[0], max_fps[1]);
        }

        camera.setParameters(parameters);
//        camera.setDisplayOrientation(cameraDisplayOrientation);
        freeTexture();

        isPreviewing = true;
        camera.setPreviewTexture(surface);
        camera.startPreview();
        camera.cancelAutoFocus();
//        doAutoFocus();
//        onFocus(new Point(360, 640), new Camera.AutoFocusCallback() {
//            @Override
//            public void onAutoFocus(boolean success, Camera camera) {
//
//            }
//        });
    }


    public void start() throws IOException {
        textures = new int[1];
        GLES20.glGenTextures(1, textures, 0);
        cameraSurfaceTexture = new SurfaceTexture(textures[0]);
        start(cameraSurfaceTexture);
    }

    private void freeTexture() {
        if (textures != null && textures[0] != 0) {
            GLES20.glDeleteTextures(1, textures, 0);
            textures = null;
        }
    }

    /**
     * 找出最适合的预览界面分辨率
     */
    private Camera.Size findBestPreviewResolution(Camera.Parameters mParams) {
        if (mParams != null) {
            Camera.Size defaultPreviewResolution = mParams.getPreviewSize();

            List<Camera.Size> rawSupportedSizes = mParams.getSupportedPreviewSizes();
            if (rawSupportedSizes == null) {
                return defaultPreviewResolution;
            }

            //精确查找一次
            List<Camera.Size> supportedPreviewResolutions = new ArrayList<>(rawSupportedSizes);
            final int targetWidth = previewSize[0];
            final int targetHeight = previewSize[1];
            for (Camera.Size resolution : supportedPreviewResolutions) {
                if (targetWidth == resolution.width && targetHeight == resolution.height) {
                    return resolution;
                }
            }

            // 按照分辨率从大到小排序
            Collections.sort(supportedPreviewResolutions, new Comparator<Camera.Size>() {
                @Override
                public int compare(Camera.Size a, Camera.Size b) {
                    int aPixels = a.height * a.width;
                    int bPixels = b.height * b.width;
                    if (bPixels < aPixels) {
                        return -1;
                    }
                    if (bPixels > aPixels) {
                        return 1;
                    }
                    return 0;
                }
            });

            Iterator<Camera.Size> it = supportedPreviewResolutions.iterator();
            // 移除不符合条件的分辨率
            double screenAspectRatio = (double) previewSize[1] / (double) previewSize[0];
            while (it.hasNext()) {
                Camera.Size supportedPreviewResolution = it.next();
                int width = supportedPreviewResolution.width;
                int height = supportedPreviewResolution.height;
                // 移除低于下限的分辨率，尽可能取高分辨率
                if (width * height < MIN_PREVIEW_PIXELS) {
                    it.remove();
                    continue;
                }

                // 在camera分辨率与屏幕分辨率宽高比不相等的情况下，找出差距最小的一组分辨率
                // 由于camera的分辨率是width>height，我们设置的portrait模式中，width<height
                // 因此这里要先交换然preview宽高比后在比较
                boolean isCandidatePortrait = width > height;
                int maybeFlippedWidth = isCandidatePortrait ? height : width;
                int maybeFlippedHeight = isCandidatePortrait ? width : height;
                double aspectRatio = (double) maybeFlippedWidth / (double) maybeFlippedHeight;
                double distortion = Math.abs(aspectRatio - screenAspectRatio);
                if (distortion > MAX_ASPECT_DISTORTION) {
                    it.remove();
                    continue;
                }

                // 找到与屏幕分辨率完全匹配的预览界面分辨率直接返回
                if (maybeFlippedWidth == previewSize[1] && maybeFlippedHeight == previewSize[0]) {
                    return supportedPreviewResolution;
                }
            }
            // 如果没有找到合适的，并且还有候选的像素，则设置其中最大比例的，对于配置比较低的机器不太合适
            if (!supportedPreviewResolutions.isEmpty()) {
                return supportedPreviewResolutions.get(0);
            }

            // 没有找到合适的，就返回默认的
            return defaultPreviewResolution;
        }
        return null;
    }


    public void stop() {
        if (camera != null) {
            isPreviewing = false;
            camera.stopPreview();
        }
    }

    public void updateTexture() {
        if (cameraSurfaceTexture != null) {
            cameraSurfaceTexture.updateTexImage();
        }
    }

    public void release() {
        if (cameraSurfaceTexture != null) {
            cameraSurfaceTexture.release();
            cameraSurfaceTexture = null;
        }

        if (camera != null) {
            isPreviewing = false;
            try {
                camera.stopPreview();
                camera.release();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
        freeTexture();
        mJpegPictureCallback = null;
        activity = null;
    }

    public void registerCameraPreviewListener(Camera.PreviewCallback listener) {
        if (camera != null && listener != null) {
            Camera.Size size = camera.getParameters().getPreviewSize();
            camera.setPreviewCallbackWithBuffer(listener);
//            camera.setPreviewCallback(listener);
            camera.addCallbackBuffer(new byte[size.width * size.height * ImageFormat.getBitsPerPixel(camera.getParameters().getPreviewFormat()) / 8]);
            camera.startPreview();
        }
    }

    public void unRegisterCameraPreviewListener() {
        if (camera != null) {
            camera.setPreviewCallback(null);
            camera.addCallbackBuffer(null);
        }
    }

    public void setTakePicture(TakePictureInterface takePicture) {
        this.mTakePictureInterface = takePicture;
    }

    public void unTakePicture() {
        this.mTakePictureInterface = null;
    }

    public void takePicture() {
        if (isPreviewing && (camera != null)) {
            camera.takePicture(null, null, mJpegPictureCallback);
        }
    }

    private int getCameraDisplayOrientation(int cameraId) {
        android.hardware.Camera.CameraInfo info =
                new android.hardware.Camera.CameraInfo();
        android.hardware.Camera.getCameraInfo(cameraId, info);
        int rotation = activity.getWindowManager().getDefaultDisplay()
                .getRotation();
        int degrees = 0;
        switch (rotation) {
            case Surface.ROTATION_0:
                degrees = 0;
                break;
            case Surface.ROTATION_90:
                degrees = 90;
                break;
            case Surface.ROTATION_180:
                degrees = 180;
                break;
            case Surface.ROTATION_270:
                degrees = 270;
                break;
        }

        int result;
        if (info.facing == Camera.CameraInfo.CAMERA_FACING_FRONT) {
            result = (info.orientation + degrees) % 360;
            result = (360 - result) % 360;  // compensate the mirror
        } else {  // back-facing
            result = (info.orientation - degrees + 360) % 360;
        }
        camera.setDisplayOrientation(result);
        return result;
    }

    //对jpeg图像数据的回调,最重要的一个回调
    private Camera.PictureCallback mJpegPictureCallback = new Camera.PictureCallback() {
        public void onPictureTaken(byte[] data, Camera camera) {

            if (mTakePictureInterface != null) {
                mTakePictureInterface.onTakePicture(data);
            }
        }
    };

    public interface TakePictureInterface {
        void onTakePicture(byte[] data);
    }
    Camera.Parameters parameters;
    private void doAutoFocus() {
      parameters = camera.getParameters();
        parameters.setFocusMode(Camera.Parameters.FOCUS_MODE_AUTO);
        camera.setParameters(parameters);
        camera.autoFocus(new Camera.AutoFocusCallback() {
            @Override
            public void onAutoFocus(boolean success, Camera camera) {
                if (success) {
                    camera.cancelAutoFocus();// 只有加上了这一句，才会自动对焦。
                    if (!Build.MODEL.equals("KORIDY H30")) {
                        parameters = camera.getParameters();
                        parameters.setFocusMode(Camera.Parameters.FOCUS_MODE_CONTINUOUS_PICTURE);// 1连续对焦
                        camera.setParameters(parameters);
                    }else{
                        parameters = camera.getParameters();
                        parameters.setFocusMode(Camera.Parameters.FOCUS_MODE_AUTO);
                        camera.setParameters(parameters);
                    }
                }
            }
        });
    }


    protected boolean onFocus(Point point, Camera.AutoFocusCallback callback) {
        if (camera == null) {
            return false;
        }


        Camera.Parameters parameters = null;
        try {
            parameters = camera.getParameters();
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
        //不支持设置自定义聚焦，则使用自动聚焦，返回

        if(Build.VERSION.SDK_INT >= 14) {
            //定点对焦
            List<Camera.Area> areas = new ArrayList<Camera.Area>();
            int left = point.x-30;
            int top = point.y - 30;
            int right = point.x + 30;
            int bottom = point.y + 30;

            areas.add(new Camera.Area(new Rect(left, top, right, bottom), 100));
            parameters.setFocusAreas(areas);
            try {
                parameters.setExposureCompensation(1);
                camera.setParameters(parameters);
            } catch (Exception e) {
                // TODO: handle exception
                e.printStackTrace();
                return false;
            }
        }


        return focus(callback);
    }


    private boolean focus(Camera.AutoFocusCallback callback) {
        try {
          //  camera.autoFocus(callback);
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
        return true;
    }
}