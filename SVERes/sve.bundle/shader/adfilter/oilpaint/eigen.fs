#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;


void main(){
    vec2 uv = v_texcoord0;
    vec3 structureTensor = texture2D(aTexture0, uv).xyz;
    vec2 majorEigenvector,minorEigenvector;
    float majorEigenValue,minorEigenValue;
    
    float t1 = structureTensor.x + structureTensor.z;
    float t2 = sqrt(pow(structureTensor.x -  structureTensor.z,2.0) + 4.0*pow(structureTensor.y,2.0));
    majorEigenValue = (t1 + t2)/2.0;
    minorEigenValue = (t1 - t2)/2.0;
    majorEigenvector = vec2(structureTensor.y,majorEigenValue-structureTensor.x);
    minorEigenvector = vec2(majorEigenValue-structureTensor.x,-structureTensor.y);
    
    //float integrationScale = (majorEigenValue - minorEigenValue) /(majorEigenValue + minorEigenValue);
    float edge = structureTensor.x + structureTensor.z;
    gl_FragColor = vec4(normalize(minorEigenvector),minorEigenValue,edge);
}
