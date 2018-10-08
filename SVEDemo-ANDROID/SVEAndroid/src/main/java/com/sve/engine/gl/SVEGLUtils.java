package com.sve.engine.gl;

import android.opengl.GLES11Ext;
import android.opengl.GLES20;

import java.nio.Buffer;
import java.nio.FloatBuffer;

/**
 * Created by ldm on 17/1/22.
 * GL常用代码段
 */

public class SVEGLUtils {

    /**
     * 创建一个空的纹理ID
     * 例：createBlankTextureId(yuvWidth, yuvHeight, GLES20.GL_LUMINANCE, GLES20.GL_UNSIGNED_BYTE);
     *
     * @param width  宽度
     * @param height 高度
     * @param format 格式 取值：GL_ALPHA/GL_RGB/GL_RGBA/GL_LUMINANCE/GL_LUMINANCE_ALPHA...
     * @param type   类型 取值：GL_UNSIGNED_BYTE/GL_SHORT/GL_UNSIGNED_SHORT/GL_FLOAT...
     * @return 纹理ID
     */
    public static int createBlankTexture2DId(int width, int height, int format, int type) {
        return createBlankTextureId(GLES20.GL_TEXTURE_2D, width, height, format, type);
    }

    /**
     * 创建一个空的纹理ID
     * 例：createBlankTextureId(GLES20.GL_TEXTURE_2D, yuvWidth, yuvHeight, GLES20.GL_LUMINANCE, GLES20.GL_UNSIGNED_BYTE);
     *
     * @param target 纹理类型：GL_TEXTURE_2D/GL_TEXTURE_EXTERNAL_OES
     * @param width  宽度
     * @param height 高度
     * @param format 格式 取值：GL_ALPHA/GL_RGB/GL_RGBA/GL_LUMINANCE/GL_LUMINANCE_ALPHA...
     * @param type   类型 取值：GL_UNSIGNED_BYTE/GL_SHORT/GL_UNSIGNED_SHORT/GL_FLOAT...
     * @return 纹理ID
     */
    public static int createBlankTextureId(int target, int width, int height, int format, int type) {
        int[] texturesIds = new int[1];
        GLES20.glGenTextures(1, texturesIds, 0);
        GLES20.glBindTexture(target, texturesIds[0]);
        GLES20.glTexImage2D(target, 0, format, width, height, 0, format, type, null);
        GLES20.glTexParameteri(target, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_LINEAR);
        GLES20.glTexParameteri(target, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_LINEAR);
        GLES20.glTexParameteri(target, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE);
        GLES20.glTexParameteri(target, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE);
        return texturesIds[0];
    }

    /**
     * 删除纹理
     *
     * @param textureId textureIDs
     */
    public static void deleteTexture(int... textureId) {
        if (textureId.length > 0) {
            GLES20.glDeleteTextures(textureId.length, textureId, 0);
        }
    }

    /**
     * 删除FboID
     *
     * @param fboId FrameBufferId
     */
    public static void deleteFrameBuffers(int... fboId) {
        if (fboId.length > 0) {
            GLES20.glDeleteBuffers(fboId.length, fboId, 0);
        }
    }

    /**
     * 向指定的纹理序列中提交Buffer
     *
     * @param textureSerialNumber 纹理序列号，取值：GLES20.GL_TEXTURE0/GLES20.GL_TEXTURE1...
     * @param texId               纹理ID
     * @param texWidth            纹理宽度
     * @param texHeight           纹理高度
     * @param pixels              Buffer
     */
    public static void subTexture2D(int textureSerialNumber, int texId, int texWidth, int texHeight, Buffer pixels) {
        subTextureBuffer(GLES20.GL_TEXTURE_2D, textureSerialNumber, texId, texWidth, texHeight, pixels);
    }

    /**
     * 向指定的纹理序列中提交Buffer
     * 例：subTextureBuffer(GLES20.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE0, 纹理Id, 宽度, 高度, 纹理buffer);
     *
     * @param target              纹理类型：GL_TEXTURE_2D/GL_TEXTURE_EXTERNAL_OES
     * @param textureSerialNumber 纹理序列号，取值：GLES20.GL_TEXTURE0/GLES20.GL_TEXTURE1...
     * @param texId               纹理ID
     * @param texWidth            纹理宽度
     * @param texHeight           纹理高度
     * @param pixels              Buffer
     */
    public static void subTextureBuffer(int target, int textureSerialNumber, int texId, int texWidth, int texHeight, Buffer pixels) {
        GLES20.glActiveTexture(textureSerialNumber);
        GLES20.glBindTexture(target, texId);
        GLES20.glTexSubImage2D(target, 0, 0, 0, texWidth, texHeight, GLES20.GL_LUMINANCE, GLES20.GL_UNSIGNED_BYTE, pixels);
    }

    /**
     * 提交bufferId到指定的顶点属性中
     *
     * @param attrIndex 绑定到shader属性的id
     * @param size      顶点每组的size
     * @param bufferId  可以使用{@link #createFloatBufferId(int, float[])}返回的bufferId
     */
    public static void subVertexAttribPointer(int attrIndex, int size, int bufferId) {
        GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, bufferId);
        GLES20.glEnableVertexAttribArray(attrIndex);
        GLES20.glVertexAttribPointer(attrIndex, size, GLES20.GL_FLOAT, false, 0, 0);
    }


    /**
     * 提交顶点属性指针
     *
     * @param attrIndex 绑定到shader属性的id
     * @param size      顶点每组的size
     * @param buffer    顶点buffer FloatBuffer
     *                  FloatBuffer buffer = FloatBuffer.wrap(bufferArray);
     *                  buffer.position(0);
     */
    public static void subVertexAttribPointer(int attrIndex, int size, FloatBuffer buffer) {
        GLES20.glEnableVertexAttribArray(attrIndex);
        GLES20.glVertexAttribPointer(attrIndex, size, GLES20.GL_FLOAT, false, 0, buffer);
    }

    /**
     * 根据bufferArray创建并提交bufferArray，成功后返回 bufferId
     * <p>
     * 例：
     * float[] vertexArray = new float[]{-1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f};
     * createFloatBuffer(GLES20.GL_ARRAY_BUFFER, vertexArray);
     * <p>
     *
     * @param arrayBufferType buffer类型， 取值 GLES20.GL_ARRAY_BUFFER/GL_ELEMENT_ARRAY_BUFFER
     * @param bufferArray     数据
     * @return bufferId       bufferId可以提供给{@link #subVertexAttribPointer(int, int, int)}方法使用
     */
    public static int createFloatBufferId(int arrayBufferType, float[] bufferArray) {
        int[] bufferIds = new int[1];
        GLES20.glGenBuffers(1, bufferIds, 0);
        GLES20.glBindBuffer(arrayBufferType, bufferIds[0]);
        GLES20.glBufferData(arrayBufferType, bufferArray.length * 4, FloatBuffer.wrap(bufferArray).position(0), GLES20.GL_STATIC_DRAW);
        return bufferIds[0];
    }

    /**
     * 创建空白相机纹理
     *
     * @return OES 纹理
     */
    public static int createBlankCameraTexture() {
        final int[] tex = new int[1];
        GLES20.glGenTextures(1, tex, 0);
        GLES20.glBindTexture(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, tex[0]);
        GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_WRAP_S, GLES20.GL_CLAMP_TO_EDGE);
        GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_WRAP_T, GLES20.GL_CLAMP_TO_EDGE);
        GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_MIN_FILTER, GLES20.GL_NEAREST);
        GLES20.glTexParameteri(GLES11Ext.GL_TEXTURE_EXTERNAL_OES, GLES20.GL_TEXTURE_MAG_FILTER, GLES20.GL_NEAREST);
        return tex[0];
    }
}
