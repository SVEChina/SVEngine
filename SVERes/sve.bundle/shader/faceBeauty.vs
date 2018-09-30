#ifdef GL_ES
precision highp float;
#endif

attribute vec2 aPosition;
attribute vec2 aColor0;
attribute vec2 aTexcoord0;

uniform vec2 texcoordClip;

varying vec2 v_texcoord0;


uniform lowp vec2 location0;
uniform lowp vec2 location1;
uniform lowp vec2 location2;
uniform lowp vec2 location3;
uniform lowp vec2 location4;
uniform lowp vec2 location5;
uniform lowp vec2 location6;
uniform lowp vec2 location7;
uniform lowp vec2 location8;
uniform lowp vec2 location9;
uniform lowp vec2 location10;
uniform lowp vec2 location11;
uniform lowp vec2 location12;
uniform lowp vec2 location13;
uniform lowp vec2 location14;
uniform lowp vec2 location15;
uniform lowp vec2 location16;
uniform lowp vec2 location17;
uniform lowp vec2 location18;
uniform lowp vec2 location19;
uniform lowp vec2 location20;
uniform lowp vec2 location21;
uniform lowp vec2 location22;
uniform lowp vec2 location23;

uniform lowp vec2 eyelarge;
uniform lowp vec4 faceslim;


#define p_faceleft location0
#define p_chin location1
#define p_faceright location2
#define p_nose location3
#define p_eyea location4
#define p_eyeb location5
#define p_left location6
#define p_right location7
#define p_chinleft location8
#define p_chinright location9
#define p_chinleft_u location10
#define p_chinright_u location11

#define p_faceleft2 location12
#define p_chin2 location13
#define p_faceright2 location14
#define p_nose2 location15
#define p_eyea2 location16
#define p_eyeb2 location17
#define p_left2 location18
#define p_right2 location19
#define p_chinleft2 location20
#define p_chinright2 location21
#define p_chinleft_u2 location22
#define p_chinright_u2 location23

#define x_a 0.72
#define y_a 1.28

vec2 faceStretch(vec2 textureCoord, vec2 originPosition, vec2 targetPosition, float radius, float curve)
{
    vec2 direction = targetPosition - originPosition;
    float lengthA = length(direction);
    float lengthB = min(lengthA, radius);
    direction *= lengthB / lengthA;
    float infect = distance(textureCoord, originPosition)/radius;
    infect = clamp(1.0-infect,0.0,1.0);
    infect = pow(infect, curve);
    
    return direction * infect;
}

void main()
{
    vec2 newCoord = (aTexcoord0 - 0.5)*texcoordClip + 0.5;
    gl_Position = vec4(aPosition.xy,0.0,1.0);
    gl_PointSize = 10.0;
    
    if(location3.x>0.03 && location3.y>0.03)
    {
        vec2 eyea = vec2(p_eyea.x * x_a, p_eyea.y * y_a);
        vec2 eyeb = vec2(p_eyeb.x * x_a, p_eyeb.y * y_a);
        vec2 left = vec2(p_left.x * x_a, p_left.y * y_a);
        vec2 right = vec2(p_right.x * x_a, p_right.y * y_a);
        
        float aspect = 0.5625;
        float weight = 0.0;
        float face_width = distance(eyea,eyeb);
        
        float eyedis = face_width;
        // eye1
        aspect = 0.5625;
        float eyeRadius = distance(left, right)*0.3;
        float dis_eye1 = distance(vec2(newCoord.x*aspect,newCoord.y), vec2(p_eyea.x*aspect,p_eyea.y));
        if(dis_eye1 <= eyeRadius)
        {
            weight = pow(dis_eye1 / eyeRadius, eyelarge.x);
            newCoord.x = p_eyea.x +(newCoord.x - p_eyea.x) * weight;
            newCoord.y = p_eyea.y +(newCoord.y - p_eyea.y) * weight;
        }
        
        // eye2
        aspect = 0.5625;
        float dis_eye2 = distance(vec2(newCoord.x*aspect,newCoord.y), vec2(p_eyeb.x*aspect,p_eyeb.y));
        if(dis_eye2 <= eyeRadius)
        {
            weight = pow(dis_eye2 / eyeRadius,eyelarge.y);
            newCoord.x = p_eyeb.x +(newCoord.x - p_eyeb.x) * weight;
            newCoord.y = p_eyeb.y +(newCoord.y - p_eyeb.y) * weight;
        }
        vec2 nose = vec2(p_nose.x, p_nose.y);
        vec2 chin = vec2(p_chin.x, p_chin.y);
        float radius = face_width*1.0;
        vec2 leftF = vec2(p_faceleft.x, p_faceleft.y);
        vec2 targetleftF = p_nose + (leftF - p_nose) * faceslim[0];
        vec2 leftFplus = vec2(0.0);
        leftFplus = faceStretch(newCoord, leftF, targetleftF, radius, 1.0);
        newCoord = newCoord - leftFplus;
        
        vec2 rightF = vec2(p_faceright.x, p_faceright.y);
        vec2 targetrightF = p_nose + (rightF - p_nose) * faceslim[0];
        vec2 rightFplus = vec2(0.0);
        rightFplus = faceStretch(newCoord, rightF, targetrightF, radius, 1.0);
        newCoord = newCoord - rightFplus;
        
        radius = face_width*1.5;
        vec2 targetchin = nose + (chin - nose) * faceslim[1];
        vec2 chinplus = vec2(0.0);
        chinplus = faceStretch(newCoord, chin, targetchin, radius, 1.0);
        newCoord = newCoord - chinplus;
        
        vec2 chinCenter = p_nose + (p_chin - p_nose) * 0.7;
        radius = face_width*1.2;
        vec2 leftC = vec2(p_chinleft.x, p_chinleft.y);
        vec2 targetleftC = chinCenter + (leftC - chinCenter) * faceslim[2];
        vec2 leftCplus = vec2(0.0);
        leftCplus = faceStretch(newCoord, leftC, targetleftC, radius, 1.0);
        newCoord = newCoord - leftCplus;
        
        vec2 rightC = vec2(p_chinright.x, p_chinright.y);
        vec2 targetrightC = chinCenter + (rightC - chinCenter) * faceslim[2];
        vec2 rightCplus = vec2(0.0);
        rightCplus = faceStretch(newCoord, rightC, targetrightC, radius, 1.0);
        newCoord = newCoord - rightCplus;
        
        radius = face_width*1.2;
        leftC = vec2(p_chinleft_u.x, p_chinleft_u.y);
        targetleftC = chinCenter + (leftC - chinCenter) * faceslim[3];
        leftCplus = faceStretch(newCoord, leftC, targetleftC, radius, 1.0);
        newCoord = newCoord - leftCplus;
        
        rightC = vec2(p_chinright_u.x, p_chinright_u.y);
        targetrightC = chinCenter + (rightC - chinCenter) * faceslim[3];
        rightCplus = faceStretch(newCoord, rightC, targetrightC, radius, 1.0);
        newCoord = newCoord - rightCplus;
    }
    
    v_texcoord0 = newCoord;
}
