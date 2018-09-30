attribute vec2 aPosition;
attribute vec2 aTexcoord0;
varying vec2 v_texcoord1;
varying vec2 v_texcoord0;
uniform vec2 texcoordClip;
uniform vec2 outlinePoints[106];

#define RESHAPE_NUM 14

#define RESHAPE_INDEX_CHIN          0       //下巴
#define RESHAPE_INDEX_EYE           1       //大眼
#define RESHAPE_INDEX_NOSE          2       //瘦鼻
#define RESHAPE_INDEX_FACE          3       //瘦脸
#define RESHAPE_INDEX_FOREHEAD      4       //额头
#define RESHAPE_INDEX_SMALL_FACE    5       //小脸
#define RESHAPE_INDEX_CUT_FACE      6       //削脸

#define RESHAPE_INDEX_MOV_NOSE      7       //长鼻
#define RESHAPE_INDEX_ZOOM_MOUTH    8       //嘴型
#define RESHAPE_INDEX_CORNER_EYE    9       //开眼角

#define RESHAPE_INDEX_MOUTH_CORNER  10      //微笑嘴角

#define RESHAPE_INDEX_MOV_EYE       11      //移动眼睛 没有滑杆
#define RESHAPE_INDEX_ZOOM_EYE      12      //放大眼睛 没有滑杆
#define RESHAPE_INDEX_MOV_MOUTH     13      //移动嘴巴 没有滑杆

uniform float g_intensity[RESHAPE_NUM];
//uniform int vSurfaceWidth;
//uniform int vSurfaceHeight;
uniform float surfaceWidth;
uniform float surfaceHeight;
//#define x_y vec2(720.0,1280.0)
#define x_y vec2(surfaceWidth,surfaceHeight)
vec2 x_y_proportion;
vec2 curCoord;
#define MAX_STEP 40     //最大步数
uniform vec2 startPoint[MAX_STEP];
uniform vec2 endPoint[MAX_STEP];
uniform int actionType[MAX_STEP];
uniform float intensity[MAX_STEP];
uniform float radius[MAX_STEP];
uniform int RealStep[1];   //记录形变实际使用的步数


vec2 stretchFun(vec2 textureCoord, vec2 originPosition, vec2 targetPosition, float radius)
{
    vec2 offset = vec2(0.0);
    vec2 result = vec2(0.0);
    
    vec2 direction = targetPosition - originPosition;
    float lengthA = length(direction);
    //if(lengthA<0.0001)   return (textureCoord-direction);
    
    float infect = distance(textureCoord, originPosition)/radius;
    
    infect = 1.0-infect;
    infect = clamp(infect,0.0,1.0);
    offset = direction * infect;
    
    result = textureCoord - offset;
    
    return result;
}

vec2 stretchFun2(vec2 textureCoord, vec2 originPosition, vec2 targetPosition, float radius,float intensity)
{
    vec2 offset = vec2(0.0);
    vec2 result = vec2(0.0);
    
    vec2 direction = targetPosition - originPosition;
    float lengthA = length(direction);
    //if(lengthA<0.0001)   return (textureCoord-direction);
    
    float infect = distance(textureCoord, originPosition)/radius;
    
    infect = 1.0-infect;
    infect = clamp(infect,0.0,1.0);
    offset = direction * infect*intensity;
    
    result = textureCoord - offset;
    
    return result;
}

vec2 stretchFun3(vec2 textureCoord, vec2 originPosition, vec2 targetPosition, float radius)
{
    vec2 offset = vec2(0.0);
    vec2 result = vec2(0.0);
    
    vec2 direction = targetPosition - originPosition;
    float lengthA = length(direction);
    //if(lengthA<0.0001)   return (textureCoord-direction);
    
    float infect = distance(textureCoord, originPosition)/radius;
    
    infect = 1.0-infect;
    infect = smoothstep(0.0,1.0,infect);
    infect = sqrt(infect);
    offset = direction * infect;
    
    result = textureCoord - offset;
    
    return result;
}

vec2 enlargeFun(vec2 curCoord,vec2 circleCenter,float radius,float intensity)
{
    float currentDistance = distance(curCoord,circleCenter);
    //if (currentDistance<=radius)
    {
        float weight = currentDistance/radius;
        weight = 1.0-intensity*(1.0-weight*weight);
        weight = clamp(weight,0.0,1.0);
        curCoord = circleCenter+(curCoord-circleCenter)*weight;
    }
    return curCoord;
}

vec2 narrowFun(vec2 curCoord,vec2 circleCenter,float radius,float intensity)
{
    float currentDistance = distance(curCoord,circleCenter);
    //if (currentDistance<=radius)
    {
        float weight = currentDistance/radius;
        weight = 1.0-intensity*(1.0-weight*weight);
        weight = clamp(weight,0.0001,1.0);
        curCoord = circleCenter+(curCoord-circleCenter)/weight;
    }
    return curCoord;
}

void main()
{
    gl_Position = vec4(aPosition,0.0,1.0);
    v_texcoord0 = (aTexcoord0 - 0.5)*texcoordClip + 0.5;
    curCoord=v_texcoord0*x_y;
    vec2 srcPoint               = vec2(0.0);
    vec2 dstPoint               = vec2(0.0);
    float face_scale = 0.5 / 0.8;
    //大眼幅度
    float eye_scale=g_intensity[RESHAPE_INDEX_EYE]*0.5 + 0.5;
    eye_scale = eye_scale * 2.0;
    {
        for(int i=0;i<RealStep[0];i++)
        {
            srcPoint        = startPoint[i];
            dstPoint        = endPoint[i];
            float scaleForEyeOrFace=face_scale;
            if(i >= 16 && i <= 19)
                scaleForEyeOrFace=eye_scale;
            if(actionType[i]== 0)
            {
                curCoord  = stretchFun2(curCoord,srcPoint,dstPoint,radius[i],intensity[i]*scaleForEyeOrFace);
            }
            else if(actionType[i] == 1)
            {
                curCoord = enlargeFun(curCoord,dstPoint,radius[i],intensity[i]*scaleForEyeOrFace);
            }
            else if(actionType[i] == 2)
            {
                curCoord = narrowFun(curCoord,dstPoint,radius[i],intensity[i]*scaleForEyeOrFace);
            }
        }
    }
    v_texcoord0 = curCoord/x_y;
//    v_texcoord0.x=v_texcoord0.x;
//    v_texcoord0.y=1.0-v_texcoord0.y;
    v_texcoord1 = v_texcoord0;
}
