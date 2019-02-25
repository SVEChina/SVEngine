#ifdef GL_ES
precision mediump float;
#endif

attribute vec2 aPosition;
uniform mat4 aMatrixM;
uniform mat4 aMatrixVP;



void main(){
    gl_Position = aMatrixVP*aMatrixM*vec4(aPosition.x-540.0/2.0,960.0/2.0-aPosition.y,0.0,1.0);
   
    gl_PointSize = 10.0;
}
