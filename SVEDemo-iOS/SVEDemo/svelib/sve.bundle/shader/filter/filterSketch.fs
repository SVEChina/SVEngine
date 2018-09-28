precision highp float;

varying vec2        v_texcoord0;
varying vec2        v_texcoord1;
uniform sampler2D   aTexture0;
uniform sampler2D   aTexture1;

uniform vec2        samplerSteps;
uniform float       thresholdUpper; //Range: [1.0, 1000.0]
uniform float       thresholdDown; //Range: [-1.0, 1.0]
uniform float       thresholEdge; //Range: [-1.0, 1.0]




void main()
{
    vec4 color = texture2D(aTexture0,v_texcoord0);
    vec4 color2 = texture2D(aTexture1,v_texcoord0);
    float gray = color.r*0.299 + color.g*0.587 + color.b*0.114;
    float Edge = 0.0;
    int Line_Width = 1;

    vec2 SetpLeft = vec2(-Line_Width,0.0);
    vec2 SetpUp = vec2(0.0,-Line_Width);
    
    vec2 SetpRight = vec2(1.0,0.0);
    vec2 SetpDown = vec2(0.0,1.0);
    
    int End = Line_Width*2;

    vec2 t_texcoord = v_texcoord0;
    vec4 colorTemp = texture2D(aTexture0,t_texcoord+vec2(0,samplerSteps.y));
    float grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(samplerSteps.x,0));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(0,-samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(-samplerSteps.x,0));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(samplerSteps.x,samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(-samplerSteps.x,-samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(samplerSteps.x,-samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(-samplerSteps.x,samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    t_texcoord = t_texcoord + vec2(0,samplerSteps.y);
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(0,samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(samplerSteps.x,0));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(0,-samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(-samplerSteps.x,0));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(samplerSteps.x,samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(-samplerSteps.x,-samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(samplerSteps.x,-samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(-samplerSteps.x,samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    t_texcoord = t_texcoord + vec2(0,-samplerSteps.y);
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(0,samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(samplerSteps.x,0));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(0,-samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(-samplerSteps.x,0));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(samplerSteps.x,samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(-samplerSteps.x,-samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(samplerSteps.x,-samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(-samplerSteps.x,samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    t_texcoord = t_texcoord + vec2(0,2.0*samplerSteps.y);
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(0,samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(samplerSteps.x,0));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(0,-samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(-samplerSteps.x,0));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(samplerSteps.x,samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(-samplerSteps.x,-samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(samplerSteps.x,-samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(-samplerSteps.x,samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    
    t_texcoord = t_texcoord + vec2(0,-2.0*samplerSteps.y);
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(0,samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(samplerSteps.x,0));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(0,-samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(-samplerSteps.x,0));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(samplerSteps.x,samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(-samplerSteps.x,-samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(samplerSteps.x,-samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(-samplerSteps.x,samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
  
    t_texcoord = t_texcoord + vec2(samplerSteps.x,0);
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(0,samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(samplerSteps.x,0));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(0,-samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(-samplerSteps.x,0));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(samplerSteps.x,samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(-samplerSteps.x,-samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(samplerSteps.x,-samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(-samplerSteps.x,samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    t_texcoord = t_texcoord + vec2(-samplerSteps.x,0);
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(0,samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(samplerSteps.x,0));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(0,-samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(-samplerSteps.x,0));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(samplerSteps.x,samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(-samplerSteps.x,-samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(samplerSteps.x,-samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(-samplerSteps.x,samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    t_texcoord = t_texcoord + vec2(2.0*samplerSteps.x,0);
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(0,samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(samplerSteps.x,0));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(0,-samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(-samplerSteps.x,0));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(samplerSteps.x,samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(-samplerSteps.x,-samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(samplerSteps.x,-samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(-samplerSteps.x,samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    
    t_texcoord = t_texcoord + vec2(-2.0*samplerSteps.x,0);
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(0,samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(samplerSteps.x,0));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(0,-samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(-samplerSteps.x,0));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(samplerSteps.x,samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(-samplerSteps.x,-samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(samplerSteps.x,-samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;
    
    colorTemp = texture2D(aTexture0,t_texcoord+vec2(-samplerSteps.x,samplerSteps.y));
    grayTemp = colorTemp.r*0.299 + colorTemp.g*0.587 + colorTemp.b*0.114;
    Edge = Edge + gray - grayTemp;

    Edge = 1.0 - Edge;
    gl_FragColor =  vec4(Edge,Edge,Edge,1.0);

    
    if (Edge<thresholEdge) {
        
        gl_FragColor = vec4(0.0,0.0,0.0,1.0);
    }
    else
    {
        if (gray>thresholdDown)
        {
            if(gray<thresholdUpper)
            {
                gl_FragColor = texture2D(aTexture1, v_texcoord0);
                
                
            }
            else
            {
                gl_FragColor = vec4(1.0,1.0,1.0,1.0);
            }
            
        }
        else
        {
            
            gl_FragColor = vec4(0.0,0.0,0.0,1.0);
        }
    }
   
}
