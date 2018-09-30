#ifdef GL_ES
precision mediump float;
#endif

//白平衡
varying vec2 v_texcoord0;
uniform sampler2D aTexture0;

uniform vec4 color0;
uniform vec4 color1;
uniform vec4 color2;
uniform vec4 color3;
uniform vec4 color4;
uniform vec4 color5;
uniform vec4 color6;
uniform vec4 color7;
uniform vec4 color8;

vec4 samplecolor(int row,float tex_x){
    if(tex_x>=0.0 && tex_x<1.0){
        if(row == 0){
            return vec4(1.0,0.0,0.0,1.0);
        }
        else if(row == 1){
            return vec4(0.0,1.0,0.0,1.0);
        }
        else if(row == 2){
            return vec4(0.0,0.0,1.0,1.0);
        }
    }
    else if(tex_x>=1.0 && tex_x<2.0){
        if(row == 0){
            return vec4(1.0,1.0,0.0,1.0);
        }
        else if(row == 1){
            return vec4(0.0,1.0,1.0,1.0);
        }
        else if(row == 2){
            return vec4(1.0,0.0,1.0,1.0);
        }
    }
    else if(tex_x>=2.0 && tex_x<=3.0){
        if(row == 0){
            return vec4(0.5,0.5,0.5,1.0);
        }
        else if(row == 1){
            return vec4(0.8,0.5,0.2,1.0);
        }
        else if(row == 2){
            return vec4(0.2,0.5,0.8,1.0);
        }
    }
    return vec4(1.0,1.0,1.0,1.0);
}

void main()
{
    vec2 newTexcoord = v_texcoord0*vec2(3.0,3.0);
    vec4 rgba = texture2D(aTexture0,fract(newTexcoord));
    if(newTexcoord.y>=0.0 && newTexcoord.y<1.0){
        gl_FragColor = samplecolor(0,newTexcoord.x)*rgba;
    }
    else if(newTexcoord.y>=1.0 && newTexcoord.x<2.0){
        gl_FragColor = samplecolor(1,newTexcoord.x)*rgba;
    }
    else if(newTexcoord.y>=2.0 && newTexcoord.x<=3.0){
        gl_FragColor = samplecolor(2,newTexcoord.x)*rgba;
    }
}
