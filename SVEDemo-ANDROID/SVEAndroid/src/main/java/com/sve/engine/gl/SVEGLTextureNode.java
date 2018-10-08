package com.sve.engine.gl;

import android.opengl.GLES20;

/**
 * Created by lidm on 17/5/23.
 * 绘制RGBA纹
 */

public class SVEGLTextureNode extends SVEGLNode {

    private static final String FRAGMENT_SHADER = "precision mediump float;" +
            "varying vec2 sTextureCoordinate;" +
            "uniform sampler2D inputImageTexture;" +
            "void main() {" +
            "lowp vec4 color = texture2D(inputImageTexture, sTextureCoordinate);" +
            "gl_FragColor = color.rgba;" +
            "}";

    private int vertexBufferId;
    private int textureBufferId;
    private int textureId;

    private SVEGLTextureNode() {

    }

    public static SVEGLTextureNode create() {
        SVEGLTextureNode node = new SVEGLTextureNode();
        if (node.init(DEFAULT_VERTEX_SHADER, FRAGMENT_SHADER)) {
            return node;
        }
        node.release();
        return null;
    }

    @Override
    protected void onBindBuffer() {
        vertexBufferId = createDefaultVertexBuffer();
        textureBufferId = createDefaultTextureBuffer();
    }

    @Override
    protected void onBindTexture() {
        GLES20.glUniform1i(GLES20.glGetUniformLocation(mProgram.getProgramId(), "inputImageTexture"), 0);
    }

    public void setTextureId(int textureId) {
        this.textureId = textureId;
    }

    @Override
    protected void onBindShader() {
        setFlipScale(1, 1);
    }

    public void draw(){
        onDraw();
    }

    @Override
    public void onDraw() {
        if (textureId > 0) {
            mProgram.bind();
            GLES20.glActiveTexture(GLES20.GL_TEXTURE0);
            GLES20.glBindTexture(GLES20.GL_TEXTURE_2D, textureId);

            SVEGLUtils.subVertexAttribPointer(aPosition, 2, vertexBufferId);
            SVEGLUtils.subVertexAttribPointer(aTextureCoordinate, 2, textureBufferId);

            GLES20.glDrawArrays(GLES20.GL_TRIANGLE_FAN, 0, 4);
        }
    }
}
