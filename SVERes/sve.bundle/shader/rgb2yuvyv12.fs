#ifdef GL_ES
precision highp float;
#endif
varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform float width;
uniform float height;
void main(void) {
    vec3 offset = vec3(0.0625, 0.5, 0.5);
    vec3 ycoeff = vec3(0.256816, 0.504154, 0.0979137);
    vec3 ucoeff = vec3(-0.148246, -0.29102, 0.439266);
    vec3 vcoeff = vec3(0.439271, -0.367833, -0.071438);
    vec2 nowTxtPos = v_texcoord0;
    vec2 size = vec2(width, height);
    float hexcursion=fract(height/16.0);
    float xexcursion=0.0;
    if(hexcursion!=0.0){
        xexcursion=1.0/height*(1.0-hexcursion);
    }

    vec2 yScale = vec2(4, 4);
    vec2 uvScale = vec2(8, 8);

    if (nowTxtPos.y < 0.25) {
        vec2 basePos = nowTxtPos * yScale * size;
        float addY = float(int((basePos.x / width)));
        basePos.x -= addY * width;
        basePos.y += addY;
        float y1, y2, y3, y4;
        vec2 samplingPos = basePos / size;
        vec4 texel = texture2D(aTexture0, samplingPos);
        y1 = dot(texel.rgb, ycoeff);
        y1 += offset.x;
        basePos.x += 1.0;
        samplingPos = basePos / size;
        texel = texture2D(aTexture0, samplingPos);
        y2 = dot(texel.rgb, ycoeff);
        y2 += offset.x;
        basePos.x += 1.0;
        samplingPos = basePos / size;
        texel = texture2D(aTexture0, samplingPos);
        y3 = dot(texel.rgb, ycoeff);
        y3 += offset.x;
        basePos.x += 1.0;
        samplingPos = basePos / size;
        texel = texture2D(aTexture0, samplingPos);
        y4 = dot(texel.rgb, ycoeff);
        y4 += offset.x;
        gl_FragColor = vec4(y1, y2, y3, y4);
    } else if (nowTxtPos.y <=0.3125&&!((nowTxtPos.y==(0.3125-xexcursion)||nowTxtPos.y==0.3125)&&nowTxtPos.x>=hexcursion)) {
        nowTxtPos.y -= 0.25;
        vec2 basePos = nowTxtPos * uvScale * size;
        float addY = float(int(basePos.x / width));
        basePos.x -= addY * width;
        basePos.y += addY;
        basePos.y *= 2.0;
        basePos -= clamp(uvScale * 0.5, vec2(0.0), uvScale);
        basePos.y -= 2.0;
        vec2 samplingPos = basePos / size;
        vec4 sample = texture2D(aTexture0, samplingPos).rgba;
        float u1 = dot(sample.rgb, vcoeff);
        u1 += offset.y;
        basePos.x += 2.0;
        samplingPos = basePos / size;
        sample = texture2D(aTexture0, samplingPos).rgba;
        float u2 = dot(sample.rgb, vcoeff);
        u2 += offset.y;
        basePos.x += 2.0;
        samplingPos = basePos / size;
        sample = texture2D(aTexture0,samplingPos).rgba;
        float u3 = dot(sample.rgb, vcoeff);
        u3 += offset.y;
        basePos.x += 2.0;
        samplingPos = basePos / size;
        sample = texture2D(aTexture0, samplingPos).rgba;
        float u4 = dot(sample.rgb, vcoeff);
        u4 += offset.y;
        gl_FragColor = vec4(u1, u2, u3, u4);
        } else if(nowTxtPos.y<=0.375){
        nowTxtPos.y -= 0.3125;
        vec2 basePos = nowTxtPos * uvScale * size;
        float addY = float(int(basePos.x / width));
        basePos.x -= addY * width;
        basePos.y += addY;
        basePos.y *= 2.0;
        basePos -= clamp(uvScale * 0.5 - 2.0, vec2(0.0), uvScale);
        basePos.y -= 2.0;
        vec2 samplingPos = basePos / size;
        vec4 sample = texture2D(aTexture0, samplingPos).rgba;
        float v1 = dot(sample.rgb, ucoeff);
        v1 += offset.z;
        basePos.x += 2.0;
        samplingPos = basePos / size;
        sample = texture2D(aTexture0, samplingPos).rgba;
        float v2 = dot(sample.rgb, ucoeff);
        v2 += offset.z;
        basePos.x += 2.0;
        samplingPos = basePos / size;
        sample = texture2D(aTexture0, samplingPos).rgba;
        float v3 = dot(sample.rgb, ucoeff);
        v3 += offset.z;
        basePos.x += 2.0;
        samplingPos = basePos / size;
        sample = texture2D(aTexture0, samplingPos).rgba;
        float v4 = dot(sample.rgb, ucoeff);
        v4 += offset.z;
        gl_FragColor = vec4(v1, v2, v3, v4);
    }
}
