package com.sve.engine.gl;

import android.opengl.GLES20;

/**
 * Created by lidm on 17/8/7.
 * 渲染辅助类， 提供FM2渲染好的TextureId
 */

public class SVERenderHelper {


    private SVEGLTextureNode mGLTextureNode = null;
    private int mViewWidth, mViewHeight;
    private int mInputWidth, mInputHeight;

    public void setInputDataFormat(int width, int height, float rotation) {
        mInputWidth = width;
        mInputHeight = height;
        mGLTextureNode = SVEGLTextureNode.create();
    }

    public void drawTexture(int textureId, int angle,int x,int y,int w,int h) {

        GLES20.glBindFramebuffer(GLES20.GL_FRAMEBUFFER, 0);
        GLES20.glViewport(x, y, w, h);
        GLES20.glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

        GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT | GLES20.GL_DEPTH_BUFFER_BIT | GLES20.GL_STENCIL_BUFFER_BIT);
        GLES20.glEnable(GLES20.GL_BLEND);
        GLES20.glBlendFunc(GLES20.GL_ONE, GLES20.GL_ONE_MINUS_SRC_ALPHA);

        mGLTextureNode.setRotation(angle);
        mGLTextureNode.setFlipScale(1, 1);
        mGLTextureNode.setTextureId(textureId);
        mGLTextureNode.draw();
        GLES20.glDisable(GLES20.GL_BLEND);

    }

    public void destory(){

    }


}
