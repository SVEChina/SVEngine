#ifdef GL_ES
#extension GL_OES_EGL_image_external : require
precision mediump float;
#endif

varying vec2                v_texcoord0;
uniform samplerExternalOES  aTexture0;

void main() {
    gl_FragColor = texture2D(aTexture0, v_texcoord0);
}
