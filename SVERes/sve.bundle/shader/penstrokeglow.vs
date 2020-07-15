#ifdef GL_ES
precision mediump float;
#endif
attribute vec3 aPosition;
attribute vec2 aTexcoord0;
uniform mat4 aMatrixM;
uniform mat4 aMatrixVP;
uniform vec2 texcoordClip;
varying vec2 v_texcoord0;
uniform vec3 u_up;
uniform vec3 u_viewPos;
uniform vec3 u_objPos;
void main(){
    v_texcoord0 = (aTexcoord0 - 0.5)*texcoordClip + 0.5;
    vec3 up = u_up;
    vec3 lookat = u_viewPos - u_objPos;
    vec3 right = cross(up,  lookat );
    up = cross( lookat, right );
    lookat = normalize( lookat );
    right = normalize( right );
    up = normalize( up );
    //
    mat4 W = mat4(vec4( right, 0.0 ),
                  vec4( up, 0.0 ),
                  vec4( lookat, 0.0 ),
                  vec4( u_objPos, 1.0 ) );
    gl_Position = aMatrixVP * W * vec4(aPosition,1.0);
}
