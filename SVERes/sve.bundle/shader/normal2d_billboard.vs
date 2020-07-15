#ifdef GL_ES
precision mediump float;
#endif
attribute vec2 aPosition;
attribute vec2 aTexcoord0;
uniform mat4 aMatrixM;
uniform mat4 aMatrixVP;
uniform vec2 texcoordClip;
varying vec2 v_texcoord0;
uniform vec3 u_viewPos;
uniform vec3 u_quadPosW;
void main(){
    v_texcoord0 = (aTexcoord0 - 0.5)*texcoordClip + 0.5;
    
    vec3 vAt = u_viewPos - u_quadPosW;
    vec3 vRight = cross( vec3( 0.0, 1.0, 0.0 ),  vAt );
    vec3 vUp = cross( vAt, vRight );
    vAt = normalize( vAt );
    vRight = normalize( vRight );
    vUp = normalize( vUp );
    
    mat4 W = mat4(vec4( vRight, 0.0 ),
                  vec4( vUp, 0.0 ),
                  vec4( vAt, 0.0 ),
                  vec4( u_quadPosW, 1.0 ) );
    
    gl_Position = aMatrixVP * aMatrixM  * vec4( aPosition, 0.0, 1.0 );
}
