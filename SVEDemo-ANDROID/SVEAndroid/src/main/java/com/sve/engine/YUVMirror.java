package com.sve.engine;

/**
 * Created by lidm on 17/9/18.
 * YUV镜像
 */

public class YUVMirror {


    //镜像android前置摄像头YUV数据， native速度要快一些
    public static void yuv90MirrorJava(byte[] srcYuv, byte[] destYuv, int w, int h) {

        for (int i = 1; i <=h; i++) {
            System.arraycopy(srcYuv, (h - i) * w, destYuv, (i-1) * w, w);
        }
        int offIndex = w * h;
        int size=offIndex+offIndex/2;
        for (int i = 1; i <= h/2; i++) {
            System.arraycopy(srcYuv, size-i*w, destYuv, offIndex + (i-1)* w, w);
        }
    }

}
