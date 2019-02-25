#ifdef GL_ES
precision mediump float;
#endif

varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform vec3 vColor1;
uniform vec3 vColor2;

vec3 hsvAdjust(vec3 src, vec3 color1, vec3 color2) //color1:red green blue, color2: magenta yellow cyan
{
    float fmax = max(src.r,max(src.g,src.b));
    float fmin = min(src.r,min(src.g,src.b));
    float fdelta = fmax - fmin;
    float fs_off;
    vec3 hsv;
    hsv.z = fmax;
    if(0.0 == fdelta)
    {
        return src;
    }
    //hue calculate
    hsv.y = fdelta/fmax;
    if(fmax == src.r)
    {
        if(src.g >= src.b)
        {
            hsv.x = (src.g - src.b)/fdelta;
            fs_off = (color2.g - color1.r)*hsv.x + color1.r;
            //saturation adjust
            hsv.y = hsv.y*(1.0 + fs_off);
            clamp(hsv.y, 0.0, 1.0);
            //rgb2hsv end
            //hsv2rgb
            src.r = hsv.z;
            src.b = hsv.z*(1.0 - hsv.y);
            src.g = hsv.z*(1.0 - hsv.y + hsv.y*hsv.x);
        }
        else
        {
            hsv.x = (src.r - src.b)/fdelta;
            fs_off = (color1.r - color2.r)*hsv.x + color2.r;
            //saturation adjust
            hsv.y = hsv.y*(1.0 + fs_off);
            clamp(hsv.y, 0.0, 1.0);
            //rgb2hsv end
            //hsv2rgb
            src.r = hsv.z;
            src.g = hsv.z*(1.0 - hsv.y);
            src.b = hsv.z*(1.0 - hsv.y*hsv.x);
        }
    }
    else if(fmax == src.g)
    {
        
        if(src.r > src.b)
        {
            hsv.x = (src.g - src.r)/fdelta;
            fs_off = (color1.g - color2.g)*hsv.x + color2.g;
            //saturation adjust
            hsv.y = hsv.y*(1.0 + fs_off);
            clamp(hsv.y, 0.0, 1.0);
            //rgb2hsv end
            //hsv2rgb
            src.g = hsv.z;
            src.r = hsv.z*(1.0 - hsv.y*hsv.x);
            src.b = hsv.z*(1.0 - hsv.y);
        }
        else
        {
            //2
            hsv.x = (src.b - src.r)/fdelta;
            fs_off = (color2.b - color1.g)*hsv.x + color1.g;
            //saturation adjust
            hsv.y = hsv.y*(1.0 + fs_off);
            clamp(hsv.y, 0.0, 1.0);
            //rgb2hsv end
            //hsv2rgb
            src.g = hsv.z;
            src.r = hsv.z*(1.0 - hsv.y);
            src.b = hsv.z*(1.0 - hsv.y + hsv.y*hsv.x);
        }
    }
    else
    {
        if(src.g > src.r)
        {
            hsv.x = (src.b - src.g)/fdelta;
            fs_off = (color1.b - color2.b)*hsv.x + color2.b;
            //saturation adjust
            hsv.y = hsv.y*(1.0 + fs_off);
            clamp(hsv.y, 0.0, 1.0);
            //rgb2hsv end
            //hsv2rgb
            src.b = hsv.z;
            src.r = hsv.z*(1.0 - hsv.y);
            src.g = hsv.z*(1.0 - hsv.y*hsv.x);
        }
        else
        {
            //4
            hsv.x = (src.r - src.g)/fdelta;
            fs_off = (color2.r - color1.b)*hsv.x + color1.b;
            //saturation adjust
            hsv.y = hsv.y*(1.0 + fs_off);
            clamp(hsv.y, 0.0, 1.0);
            //rgb2hsv end
            //hsv2rgb
            src.b = hsv.z;
            src.r = hsv.z*(1.0 - hsv.y + hsv.y*hsv.x);
            src.g = hsv.z*(1.0 - hsv.y);
        }
    }
    return src;
}

void main()
{
    vec4 src = texture2D(aTexture0, v_texcoord0);
    src.rgb = hsvAdjust(src.rgb, vColor1, vColor2);
    gl_FragColor = src;
}
