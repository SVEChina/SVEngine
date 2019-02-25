#ifdef GL_ES
precision highp float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;

uniform float m_saturationRed;
uniform float m_HueRed;
uniform float m_LightnessRed;

uniform float m_saturationYellow;
uniform float m_HueYellow;
uniform float m_LightnessYellow;

uniform float m_saturationGreen;
uniform float m_HueGreen;
uniform float m_LightnessGreen;

uniform float m_saturationMagenta;
uniform float m_HueMagenta;
uniform float m_LightnessMagenta;

uniform float m_saturationBlue;
uniform float m_HueBlue;
uniform float m_LightnessBlue;

uniform float m_saturationCyan;
uniform float m_HueCyan;
uniform float m_LightnessCyan;
const float eps = 0.0000001;

vec3 RGBToHSL(vec3 color){
    vec3 hsl; // init to 0 to avoid warnings ? (and reverse if + remove first part)
    float fmin = min(min(color.r, color.g), color.b);    //Min. value of RGB
    float fmax = max(max(color.r, color.g), color.b);    //Max. value of RGB
    float delta = fmax - fmin;             //Delta RGB value
    hsl.z = (fmax + fmin) / 2.0; // Luminance
    if (delta == 0.0){       //This is a gray, no chroma...
        hsl.x = 0.0;    // Hue
        hsl.y = 0.0;    // Saturation
    }else{                                    //Chromatic data...
        if (hsl.z < 0.5)
            hsl.y = delta / (fmax + fmin); // Saturation
        else
            hsl.y = delta / (2.0 - fmax - fmin); // Saturation
        float deltaR = (((fmax - color.r) / 6.0) + (delta / 2.0)) / delta;
        float deltaG = (((fmax - color.g) / 6.0) + (delta / 2.0)) / delta;
        float deltaB = (((fmax - color.b) / 6.0) + (delta / 2.0)) / delta;
        if (color.r == fmax )
            hsl.x = deltaB - deltaG; // Hue
        else if (color.g == fmax)
            hsl.x = (1.0 / 3.0) + deltaR - deltaB; // Hue
        else if (color.b == fmax)
            hsl.x = (2.0 / 3.0) + deltaG - deltaR; // Hue
        
        if (hsl.x < 0.0)
            hsl.x += 1.0; // Hue
        else if (hsl.x > 1.0)
            hsl.x -= 1.0; // Hue
    }
    return hsl;
}

float HueToRGB(float f1, float f2, float hue){
    if (hue < 0.0)
        hue += 1.0;
    else if (hue > 1.0)
        hue -= 1.0;
    float res;
    if ((6.0 * hue) < 1.0)
        res = f1 + (f2 - f1) * 6.0 * hue;
    else if ((2.0 * hue) < 1.0)
        res = f2;
    else if ((3.0 * hue) < 2.0)
        res = f1 + (f2 - f1) * ((2.0 / 3.0) - hue) * 6.0;
    else
        res = f1;
    return res;
}

vec3 HSLToRGB(vec3 hsl){
    vec3 rgb;
    if (hsl.y == 0.0)
        rgb = vec3(hsl.z); // Luminance
    else{
        float f2;
        if (hsl.z < 0.5)
            f2 = hsl.z * (1.0 + hsl.y);
        else
            f2 = (hsl.z + hsl.y) - (hsl.y * hsl.z);
        float f1 = 2.0 * hsl.z - f2;
        rgb.r = HueToRGB(f1, f2, hsl.x + (1.0/3.0));
        rgb.g = HueToRGB(f1, f2, hsl.x);
        rgb.b= HueToRGB(f1, f2, hsl.x - (1.0/3.0));
    }
    return rgb;
}
//========================================================================

void main(){
    vec4 color = texture2D(aTexture0,v_texcoord0);
    vec4 hls=vec4(RGBToHSL(color.rgb),color.a);
    
    if((hls.x>=0.925&&hls.x<=1.0)||(hls.x>=0.0&&hls.x<=0.065)){
        hls.x=hls.x+m_HueRed*0.05;
        hls.y *=m_saturationRed;  //调整饱和度
        hls.z *=m_LightnessRed;
    }
   
    if(hls.x>=0.0&&hls.x<=0.125){
        hls.x=hls.x+m_HueYellow*0.05;
        hls.y *=m_saturationYellow;  //调整饱和度
        hls.z *=m_LightnessYellow;
    }
   
    if(hls.x>0.125&&hls.x<=0.5){
        hls.x=hls.x+m_HueGreen*0.075;
        hls.y *=m_saturationGreen;  //调整饱和度
        hls.z *=m_LightnessGreen;
    }
    
    if(hls.x>0.825&&hls.x<0.925){
        hls.x=hls.x+m_HueMagenta*0.05;
        hls.y *=m_saturationMagenta;  //调整饱和度
        hls.z *=m_LightnessMagenta;
    }
    
    if(hls.x>0.75&&hls.x<0.825){
        hls.x=hls.x+m_HueBlue*0.05;
        hls.y *=m_saturationBlue;  //调整饱和度
        hls.z *=m_LightnessBlue;
    }
    
    if(hls.x>0.5&&hls.x<0.75){
        hls.x=hls.x+m_HueCyan*0.05;
        hls.y *=m_saturationCyan;  //调整饱和度
        hls.z *=m_LightnessCyan;
    }
    
    hls.rgb=HSLToRGB(hls.xyz);
    gl_FragColor=hls;
    
   
}
