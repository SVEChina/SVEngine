precision highp float;
varying highp vec2 v_texcoord0;
varying highp vec2 v_texcoord1;

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

uniform sampler2D aTexture0;
uniform sampler2D aTexture1;
uniform vec2 outlinePoints[106];
uniform float g_intensity[RESHAPE_NUM];
uniform float surfaceWidth;
uniform float surfaceHeight;
#define x_y vec2(surfaceWidth,surfaceHeight)
vec2 maskImageSize;
vec2 curCoord;
uniform float inv_surfaceWidth;
uniform float inv_surfaceHeight;
#define inv_x_y vec2(inv_surfaceWidth,inv_surfaceHeight)
uniform float eyesDistance;
uniform float inversedEyesDistance;

vec2 leftlargeEye(vec2 textureCoord, float intensity)
{
    vec2 TexCoord;
    float infect = distance(outlinePoints[74], textureCoord*x_y)*inversedEyesDistance*2.0;
    
    infect = 1.0-infect;
    infect = clamp(infect,0.0,1.0);
    
    TexCoord = outlinePoints[74] + (textureCoord*x_y - outlinePoints[74])*(1.0-intensity * infect);
    
    return TexCoord;
}

vec2 rightlargeEye(vec2 textureCoord, float intensity)
{
    vec2 TexCoord;
    float infect = distance(outlinePoints[77], textureCoord*x_y)*inversedEyesDistance*2.0;
    
    infect = 1.0-infect;
    infect = clamp(infect,0.0,1.0);
    TexCoord = outlinePoints[77] + (textureCoord*x_y - outlinePoints[77])*(1.0-intensity * infect);
    return TexCoord;
}

vec2 largeeye(vec2 TexCoord1, float intensity, float MaskColorR)
{
    vec4 rescolor;
    vec2 rescoord = TexCoord1;
    vec2 TexCoordMask = v_texcoord1;
    vec4 textureColor = texture2D(aTexture0,TexCoord1);
    vec2 TexCoord2;
    vec2 centercoord = (outlinePoints[74]+outlinePoints[77])*0.5;
    intensity = intensity*0.5;
    if(MaskColorR > 0.05 && distance(TexCoord1*x_y,outlinePoints[74]) < distance(TexCoord1*x_y,outlinePoints[77]) )
    {
        TexCoord2 = leftlargeEye(TexCoord1, intensity*MaskColorR);
        
        rescoord = TexCoord2*inv_x_y;
    }
    
    if(MaskColorR> 0.05 && distance(TexCoord1*x_y,outlinePoints[77]) < distance(TexCoord1*x_y,outlinePoints[74]))
    {
        TexCoord2 = rightlargeEye(TexCoord1, intensity*MaskColorR);
        
        rescoord = TexCoord2*inv_x_y;
    }
    return rescoord;
}

//调节眼距
vec2 fareye(vec2 TexCoord1, float intensity, float MaskColorR)
{
    vec2 rescoord = TexCoord1;
    vec2 TexCoordMask = v_texcoord1;
    
    vec4 textureColor = texture2D(aTexture0,TexCoord1);
    
    vec2 TexCoord2;
    vec2 centercoord = (outlinePoints[74]+outlinePoints[77])*0.5;
    
    float standdis = eyesDistance*0.8;
    
    float inv_radis = inversedEyesDistance*0.625;
    
    vec2 direction = (outlinePoints[77] - outlinePoints[74]) * 0.5;
    vec2 offset = vec2(0.0);
    intensity = intensity * 0.15;
    if(MaskColorR > 0.0001 && distance(TexCoord1*x_y,outlinePoints[74]) < distance(TexCoord1*x_y,outlinePoints[77]) )
    {
        float infect = distance(outlinePoints[74], TexCoord1*x_y)*inv_radis;
        infect = 1.0-infect;
        infect = clamp(infect,0.0,1.0);
        
        offset = direction *  intensity * MaskColorR * infect;
        
        rescoord = TexCoord1 + offset*inv_x_y;
    }
    
    if(MaskColorR > 0.0001 && distance(TexCoord1*x_y,outlinePoints[77]) < distance(TexCoord1*x_y,outlinePoints[74]))
    {
        float infect = distance(outlinePoints[77], TexCoord1*x_y)*inv_radis;
        infect = 1.0-infect;
        infect = clamp(infect,0.0,1.0);
        
        offset = direction *  intensity * MaskColorR * infect;
        
        rescoord = TexCoord1 - offset*inv_x_y;
    }
    return rescoord;
}


vec2 movnose(vec2 TexCoord1, float intensity,float MaskColorG)
{
    vec2 rescoord = TexCoord1;
    vec2 TexCoordMask = v_texcoord1;
    
    vec4 textureColor = texture2D(aTexture0,TexCoord1);
    
    vec2 TexCoord2;
    
    float standdis = eyesDistance*0.2;
    
    vec2 direction1 = (outlinePoints[77]-outlinePoints[74])*0.2;
    vec2 direction = vec2(direction1.y,-direction1.x);
    intensity = intensity*0.3;
    vec2 offset = vec2(0.0);
    if(MaskColorG > 0.001)
    {
        offset = direction *  intensity * MaskColorG;
        
        rescoord = TexCoord1 + offset*inv_x_y;
    }
    return rescoord;
}

vec2 movnose2(vec2 TexCoord1, float intensity,float MaskColorG)
{
    vec2 rescoord = TexCoord1;
    //  vec2 TexCoordMask = v_texcoord1;
    
    //  vec4 textureColor = texture2D(aTexture0,TexCoord1);
    
    //   vec2 TexCoord2;
    
    float standdis = eyesDistance*0.5;
    
    vec2 direction1 = (outlinePoints[77]-outlinePoints[74])*0.2;
    vec2 direction = vec2(direction1.y,-direction1.x);
    intensity = intensity*0.5;
    vec2 offset = vec2(0.0);
    if(MaskColorG > 0.001)
    {
        float infect = distance(outlinePoints[46], TexCoord1*x_y)/standdis;
        
        infect = 1.0-infect;
        infect = clamp(infect,0.0,1.0);
        offset = direction *  intensity * MaskColorG*infect;
        
        rescoord = TexCoord1 + offset*inv_x_y;
    }
    return rescoord;
}

vec2 movmouth2(vec2 TexCoord1, float intensity, float MaskColorB)
{
    vec2 rescoord = TexCoord1;
    
    vec4 textureColor = texture2D(aTexture0,TexCoord1);
    
    //  vec2 TexCoord2;
    
    //float standdis = distance(outlinePoints[74],outlinePoints[77])*0.2;
    vec2 direction1 = (outlinePoints[77]-outlinePoints[74])*0.2;
    vec2 direction = vec2(direction1.y,-direction1.x);
    vec2 offset = vec2(0.0);
    vec2 realMouthCenter = (outlinePoints[84]+outlinePoints[90])*0.5;
    
    float standdis = distance(outlinePoints[84],outlinePoints[90])*1.2;
    intensity = intensity*0.4;
    if(MaskColorB > 0.01)
    {
        float infect = distance(realMouthCenter, TexCoord1*x_y)/standdis;
        
        infect = 1.0-infect;
        infect = clamp(infect,0.0,1.0);
        offset = direction *  intensity * MaskColorB*infect;
        
        rescoord = TexCoord1 + offset/x_y;
    }
    return rescoord;
}
vec2 movmouth(vec2 TexCoord1, float intensity, float MaskColorB)
{
    vec2 rescoord = TexCoord1;
    vec2 TexCoordMask = v_texcoord1;
    
    vec4 textureColor = texture2D(aTexture0,TexCoord1);
    
    vec2 TexCoord2;
    
    float standdis = eyesDistance*0.2;
    vec2 direction1 = (outlinePoints[77]-outlinePoints[74])*0.2;
    vec2 direction = vec2(direction1.y,-direction1.x);
    vec2 offset = vec2(0.0);
    if(MaskColorB> 0.001)
    {
        offset = direction *  intensity * MaskColorB;
        
        rescoord = TexCoord1 + offset*inv_x_y;
        
    }
    return rescoord;
}

vec2 zoommouth(vec2 TexCoord1, float intensity, float MaskColorB)
{
    vec2 rescoord = TexCoord1*x_y;
    
    //    vec2 TexCoordMask = v_texcoord1;
    
    vec4 textureColor = texture2D(aTexture0,TexCoord1);
    
    vec2 TexCoord2;
    // 定义输出颜色值
    //FragColor = vec4(ourColor,1.0);//vec4(1.0, 0.0, 0.0, 1.0);
    //FragColor = vec4((textureColor.rgb+changeColor)*0.5,1.0);
    vec2 realMouthCenter = (outlinePoints[84]+outlinePoints[90])*0.5;
    
    float standdis = distance(outlinePoints[84],outlinePoints[90])*1.2;
    intensity = intensity*0.4;
    if(MaskColorB > 0.01)
    {
        float infect = distance(vec2(realMouthCenter.x,realMouthCenter.y), TexCoord1*x_y)/standdis*1.0;
        
        infect = 1.0-infect;
        infect = clamp(infect,0.0,1.0);
        
        rescoord = vec2(realMouthCenter.x,realMouthCenter.y) + (TexCoord1*x_y - vec2(realMouthCenter.x,realMouthCenter.y))*(1.0-intensity * infect);
        
        //     rescoord = vec2(realMouthCenter.x,1.0*x_y.y-realMouthCenter.y) + (TexCoord1*x_y - vec2(realMouthCenter.x,1.0*x_y.y-realMouthCenter.y))*0.9;
        
    }
    return rescoord/x_y;
}

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
void main()
{
    vec2 TexCoordMask = v_texcoord1;
    
    vec2 TexCoord1 = v_texcoord0;
    
    //process thin face
    float radius = 0.0;
    vec2 srcPoint = vec2(0.0);
    vec2 dstPoint = vec2(0.0);
    
    curCoord = TexCoord1 * x_y;
    float standardLength = distance(outlinePoints[74],outlinePoints[77]);
    vec2 e1 = (outlinePoints[77]-outlinePoints[74])*0.25;
    vec2 e2 = vec2(-e1.y,e1.x);
    
    
    float coeff1a = -0.316901;
    float coeff1b = -0.552054;
    float coeff2a = 0.78962;
    float coeff2b = 0.335958;
    vec2  point = outlinePoints[8];
    float radius_coeff = 0.563382;
    float intensity_new = 0.25;
    
    float level=(g_intensity[RESHAPE_INDEX_FACE] - 0.5)* 2.0 * (1.0 - 0.65 * step(0.0,0.5 - g_intensity[RESHAPE_INDEX_FACE])) * 1.4;
    
    radius = standardLength * radius_coeff;
    srcPoint = point + e1*coeff1a + e2*coeff1b ;
    dstPoint = srcPoint + e1*coeff2a + e2*coeff2b ;
    curCoord = stretchFun2(curCoord,srcPoint,dstPoint,radius,intensity_new * level);
    
    coeff1a = 0.625079;
    coeff1b = 0.0128987;
    coeff2a = -0.789619;
    coeff2b = 0.335958;
    point = outlinePoints[23];
    radius_coeff = 0.563382;
    intensity_new = 0.25;
    
    radius = standardLength * radius_coeff;
    srcPoint = point + e1*coeff1a + e2*coeff1b ;
    dstPoint = srcPoint + e1*coeff2a + e2*coeff2b ;
    curCoord = stretchFun2(curCoord,srcPoint,dstPoint,radius,intensity_new * level);
    
    coeff1a = -0.210348;
    coeff1b = -0.123809;
    coeff2a = 0.506968;
    coeff2b = -0.0294999;
    point = outlinePoints[13];
    radius_coeff = 0.563382;
    intensity_new = 0.25;
    
    radius = standardLength * radius_coeff;
    srcPoint = point + e1*coeff1a + e2*coeff1b ;
    dstPoint = srcPoint + e1*coeff2a + e2*coeff2b ;
    curCoord = stretchFun2(curCoord,srcPoint,dstPoint,radius,intensity_new * level * 1.5);
    
    coeff1a = 0.210348;
    coeff1b = -0.123809;
    coeff2a = -0.506968;
    coeff2b = -0.0294999;
    point = outlinePoints[19];
    radius_coeff = 0.563382;
    intensity_new = 0.25;
    
    radius = standardLength * radius_coeff;
    srcPoint = point + e1*coeff1a + e2*coeff1b ;
    dstPoint = srcPoint + e1*coeff2a + e2*coeff2b ;
    curCoord = stretchFun2(curCoord,srcPoint,dstPoint,radius,intensity_new * level * 1.5);
    
    level = level * 1.2;
    
    coeff1a = -0.0522563;
    coeff1b = -0.262022;
    coeff2a = 0.309933;
    coeff2b = 0.0273557;
    point = outlinePoints[4];
    radius_coeff = 0.492959;
    intensity_new = 0.25;
    
    radius = standardLength * radius_coeff;
    srcPoint = point + e1*coeff1a + e2*coeff1b ;
    dstPoint = srcPoint + e1*coeff2a + e2*coeff2b ;
    curCoord = stretchFun2(curCoord,srcPoint,dstPoint,radius,intensity_new * level);
    
    coeff1a = 0.0522563;
    coeff1b = -0.262022;
    coeff2a = -0.309933;
    coeff2b = 0.0273557;
    point = outlinePoints[28];
    radius_coeff = 0.492959;
    intensity_new = 0.25;
    
    radius = standardLength * radius_coeff;
    srcPoint = point + e1*coeff1a + e2*coeff1b ;
    dstPoint = srcPoint + e1*coeff2a + e2*coeff2b ;
    curCoord = stretchFun2(curCoord,srcPoint,dstPoint,radius,intensity_new * level);
    
    //thin nose
    level=g_intensity[RESHAPE_INDEX_NOSE];
    radius=standardLength*0.25;//0.4
    float factor=level*0.06;//0.08
    srcPoint=outlinePoints[82];
    dstPoint=srcPoint+(outlinePoints[83]- srcPoint)*factor;
    curCoord = stretchFun3(curCoord,srcPoint,dstPoint,radius);
    
    srcPoint=outlinePoints[83];
    dstPoint=srcPoint+(outlinePoints[82]- srcPoint)*factor;
    curCoord = stretchFun3(curCoord,srcPoint,dstPoint,radius);
    
    radius=standardLength*0.35;//0.4
    srcPoint=outlinePoints[80]+ (outlinePoints[78]- outlinePoints[80])*0.5;
    dstPoint=srcPoint+(outlinePoints[81]- srcPoint)*factor;
    curCoord = stretchFun(curCoord,srcPoint,dstPoint,radius);
    
    srcPoint=outlinePoints[81]+(outlinePoints[79]- outlinePoints[81])*0.5;
    dstPoint=srcPoint+(outlinePoints[80]- srcPoint)*factor;
    curCoord = stretchFun(curCoord,srcPoint,dstPoint,radius);
    
    //small face
    factor=0.085*g_intensity[RESHAPE_INDEX_SMALL_FACE];//0.085
    radius=standardLength*1.35;//1.45
    srcPoint=outlinePoints[16];
    dstPoint=srcPoint+ (outlinePoints[46]- srcPoint)*factor;
    curCoord=stretchFun3(curCoord,srcPoint,dstPoint,radius);
    
    factor=0.03*g_intensity[RESHAPE_INDEX_SMALL_FACE];//0.025
    radius=standardLength*0.9;
    srcPoint=outlinePoints[7];
    dstPoint=srcPoint+(outlinePoints[43]- srcPoint)*factor;
    curCoord=stretchFun3(curCoord,srcPoint,dstPoint,radius);
    
    srcPoint=outlinePoints[25];
    dstPoint=srcPoint+(outlinePoints[43]- srcPoint)*factor;
    curCoord=stretchFun3(curCoord,srcPoint,dstPoint,radius);
    
    factor=0.04*g_intensity[RESHAPE_INDEX_SMALL_FACE]; //0.035
    radius=standardLength*0.8;
    srcPoint=outlinePoints[1] + (outlinePoints[1] - outlinePoints[43])*0.38;
    dstPoint=srcPoint+(outlinePoints[43]- curCoord)*factor;
    curCoord=stretchFun3(curCoord,srcPoint,dstPoint,radius);
    
    srcPoint=outlinePoints[31]+ (outlinePoints[31] - outlinePoints[43])*0.38;
    dstPoint=srcPoint+(outlinePoints[43]- curCoord)*factor;
    curCoord=stretchFun3(curCoord,srcPoint,dstPoint,radius);
    
    
    TexCoord1 = curCoord / x_y;
    
    vec4 textureColor = texture2D(aTexture0,TexCoord1);
    vec4 MaskColor = texture2D(aTexture1,TexCoordMask);
    
    vec2 TexCoordres1,TexCoordres2,TexCoordres3,TexCoordres4;
    vec4 textureColor1,textureColor2,textureColor3;
    
    
    level = (g_intensity[RESHAPE_INDEX_MOV_EYE] - 0.5) * 0.2 / 0.5;
    TexCoordres1 = fareye(TexCoord1, level, MaskColor.r);
    
    level = (g_intensity[RESHAPE_INDEX_ZOOM_EYE] - 0.5) * 0.2 / 0.5;
    TexCoordres2 = largeeye(TexCoordres1, level, MaskColor.r);
    
    textureColor1 = mix(textureColor,texture2D(aTexture0,TexCoordres2), MaskColor.r);
    
    level = (g_intensity[RESHAPE_INDEX_MOV_NOSE] - 0.5) * 0.2 / 0.5 * 1.15;
    TexCoordres3 = movnose2(TexCoordres2, level, MaskColor.g);
    
    textureColor2 = mix(textureColor1,texture2D(aTexture0,TexCoordres3),MaskColor.g);
    
    level = (g_intensity[RESHAPE_INDEX_MOV_MOUTH] - 0.5) * 0.2 / 0.5;
    TexCoordres4 = movmouth2(TexCoordres3, level,MaskColor.b);
    
    textureColor3 = mix(textureColor2,texture2D(aTexture0,TexCoordres4),MaskColor.b);
    
    level = (g_intensity[RESHAPE_INDEX_ZOOM_MOUTH] - 0.5) * 0.2 / 0.5;
    TexCoordres4 = zoommouth(TexCoordres4, level, MaskColor.b);
    
    textureColor3 = mix(textureColor3,texture2D(aTexture0,TexCoordres4),MaskColor.b);
    
    gl_FragColor  = textureColor3;
}
