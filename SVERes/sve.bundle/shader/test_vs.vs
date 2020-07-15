#ifdef GL_ES
precision highp float;
#endif

attribute vec3 aPosition;
varying vec4 v_pos;

uniform mat4 aMatrixM;
uniform mat4 aMatrixV;
uniform mat4 aMatrixP;
uniform mat4 aMatrixVP;

void main(){    
    vec4 t_pos = aMatrixVP*aMatrixM*vec4(aPosition.xyz,1.0);
    v_pos = aMatrixV*vec4(aPosition.xyz,1.0);
    v_pos = vec4(aPosition.xyz,1.0);
    gl_Position = t_pos;
}
