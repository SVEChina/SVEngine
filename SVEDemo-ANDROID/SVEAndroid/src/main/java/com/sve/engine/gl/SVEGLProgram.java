package com.sve.engine.gl;

import android.opengl.GLES20;
import android.util.Log;

/**
 * Created by ldm on 17/1/22.
 * GL MAGLProgram Helper
 */

public class SVEGLProgram {

    private int mProgramId = -1;
    private int mVertexShader = -1;
    private int mFragmentShader = -1;

    public SVEGLProgram(String pvs, String fvs) {
        mProgramId = createProgram(pvs, fvs);
    }

    public int getProgramId() {
        return mProgramId;
    }

    /**
     * 创建Program
     *
     * @param pvs 顶点shader
     * @param fvs 片段shader
     * @return programId
     */
    private int createProgram(String pvs, String fvs) {
        int program = GLES20.glCreateProgram();
        checkGLError("glCreateProgram");
        if (program == 0) {
            Log.e("GLYUV", "Invalid MAGLProgram ID! Check if the context is binded!");
            return -1;
        }

        mVertexShader = loadShader(GLES20.GL_VERTEX_SHADER, pvs);
        mFragmentShader = loadShader(GLES20.GL_FRAGMENT_SHADER, fvs);

        GLES20.glAttachShader(program, mVertexShader);
        checkGLError("glAttachShader");
        GLES20.glAttachShader(program, mFragmentShader);
        checkGLError("glAttachShader");
        GLES20.glLinkProgram(program);
        checkGLError("glLinkProgram");
        return program;
    }


    /**
     * 加载shader
     *
     * @param type       类型
     * @param shaderCode shader脚本
     * @return shaderId
     */
    private int loadShader(int type, String shaderCode) {
        int shader = GLES20.glCreateShader(type);
        checkGLError("glCreateShader");
        GLES20.glShaderSource(shader, shaderCode);
        checkGLError("glShaderSource");
        GLES20.glCompileShader(shader);
        checkGLError("glCompileShader");
        return shader;
    }

    public void bind() {
        GLES20.glUseProgram(mProgramId);
        checkGLError("glUseProgram");
    }


    public void release() {
        if (mProgramId != -1) {
            GLES20.glDeleteProgram(mProgramId);
        }
        if (mVertexShader != -1) {
            GLES20.glDeleteShader(mVertexShader);
        }
        if (mFragmentShader != -1) {
            GLES20.glDeleteShader(mFragmentShader);
        }
    }

    public static void checkGLError(final String tag) {
        int loopCnt = 0;
        for (int err = GLES20.glGetError(); loopCnt < 32 && err != GLES20.GL_FALSE; err = GLES20.glGetError(), ++loopCnt) {
            String msg;
            switch (err) {
                case GLES20.GL_INVALID_ENUM:
                    msg = "invalid enum";
                    break;
                case GLES20.GL_INVALID_FRAMEBUFFER_OPERATION:
                    msg = "invalid framebuffer operation";
                    break;
                case GLES20.GL_INVALID_OPERATION:
                    msg = "invalid operation";
                    break;
                case GLES20.GL_INVALID_VALUE:
                    msg = "invalid value";
                    break;
                case GLES20.GL_OUT_OF_MEMORY:
                    msg = "out of memory";
                    break;
                default:
                    msg = "unknown error";
            }
            Log.e("MAGLProgram", String.format("After tag \"%s\" glGetError %s(0x%x) ", tag, msg, err));
        }
    }
}
