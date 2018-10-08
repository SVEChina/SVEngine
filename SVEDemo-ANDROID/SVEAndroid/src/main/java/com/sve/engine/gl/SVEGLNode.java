package com.sve.engine.gl;

import android.opengl.GLES20;

/**
 * Created by ldm on 17/1/22.
 * 绘制纹理
 */

public abstract class SVEGLNode {

    public static final String DEFAULT_VERTEX_SHADER = "attribute vec2 aPosition;" +
            "attribute vec2 aTextureCoordinate;" +
            "varying vec2 sTextureCoordinate;" +
            "uniform mat2 uRotation;" +
            "uniform vec2 uFlipScale;" +
            "void main() {" +
            "gl_Position = vec4(aPosition, 0.0, 1.0);" +
            "sTextureCoordinate = uFlipScale * (aPosition / 2.0 * uRotation) + 0.5;" +
            "}";


    public static final String DEFAULT_FRAGMENT_SHADER = "precision mediump float;\n" +
            "varying vec2 sTextureCoordinate;\n" +
            "uniform sampler2D inputImageTexture;\n" +
            "void main() {\n" +
            "gl_FragColor = texture2D(inputImageTexture, sTextureCoordinate);\n" +
            "}";

    private int uRotation;
    private int uFlipScale;

    protected SVEGLProgram mProgram;
    protected int aPosition = 0;
    protected int aTextureCoordinate = 1;


    private final float[] defaultVertexArray = new float[]{
            -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f
    };

    private final float[] defaultTextureArray = new float[]{
            0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
    };

    public SVEGLNode() {
    }

    /**
     * 创建node 使用默认的shader
     *
     * @return true/false
     */
    protected boolean init() {
        return init(DEFAULT_VERTEX_SHADER, DEFAULT_FRAGMENT_SHADER);
    }

    protected boolean init(final String pvs, final String pfs) {
        mProgram = new SVEGLProgram(pvs, pfs);
        if (mProgram.getProgramId() == -1) {
            return false;
        }

        bindShader();

        onBindTexture();
        onBindBuffer();
        return true;
    }

    public void bindProgram() {
        mProgram.bind();
    }

    private void bindShader() {
        mProgram.bind();

        uRotation = GLES20.glGetUniformLocation(mProgram.getProgramId(), "uRotation");
        uFlipScale = GLES20.glGetUniformLocation(mProgram.getProgramId(), "uFlipScale");

        GLES20.glBindAttribLocation(mProgram.getProgramId(), aPosition, "aPosition");
        GLES20.glBindAttribLocation(mProgram.getProgramId(), aTextureCoordinate, "aTextureCoordinate");

        setRotation(0.0f);
        setFlipScale(1.0f, 1.0f);

        onBindShader();
    }

    /**
     * 创建或绑定buffer
     */
    protected abstract void onBindBuffer();

    /**
     * 翻转
     *
     * @param x x轴
     * @param y y轴
     */
    public void setFlipScale(float x, float y) {
        GLES20.glUniform2f(uFlipScale, y, x);
    }

    /**
     * 旋转
     *
     * @param r 角度
     */
    public void setRotation(float r) {
        float rad = (float) (r * Math.PI / 180);
        final float cosRad = (float) Math.cos(rad);
        final float sinRad = (float) Math.sin(rad);

        final float[] rotation = new float[]{
                cosRad, sinRad,
                -sinRad, cosRad
        };
        mProgram.bind();
        GLES20.glUniformMatrix2fv(uRotation, 1, false, rotation, 0);
    }

    /**
     * 创建或绑定纹理
     */
    protected abstract void onBindTexture();

    /**
     * 这个方法用来绑定shader中的属性
     * <p>
     * 例：
     * mPosition = glGetAttribLocation(glProgram->getProgramId(), "mPosition");
     * rotation = GLES20.glGetUniformLocation(mProgram.getProgramId(), "rotation");
     */
    protected abstract void onBindShader();

    /**
     * 这个方法用来添加绘制代码
     * 例：
     * mProgram.use();
     * <p>
     * GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
     * GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureId);
     * <p>
     * GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, mPositionVertexBufferId);
     * GLES20.glEnableVertexAttribArray(mPosition);
     * GLES20.glVertexAttribPointer(mPosition, 2, GLES20.GL_FLOAT, GLES20.GL_FALSE, 0, 0);
     * <p>
     * GLES20.glDrawArrays(GLES20.GL_TRIANGLE_FAN, 0, 4);
     */
    public abstract void onDraw();


    /**
     * 创建默认的全屏顶点buffer
     *
     * @return bufferId
     */
    protected int createDefaultVertexBuffer() {
        return SVEGLUtils.createFloatBufferId(GLES20.GL_ARRAY_BUFFER, defaultVertexArray);
    }

    /**
     * 创建默认的全屏纹理buffer
     *
     * @return bufferId
     */
    protected int createDefaultTextureBuffer() {
        return SVEGLUtils.createFloatBufferId(GLES20.GL_ARRAY_BUFFER, defaultTextureArray);
    }

    /**
     * 释放数据
     */
    public void release() {
        if (mProgram != null) {
            mProgram.release();
        }
    }

}
