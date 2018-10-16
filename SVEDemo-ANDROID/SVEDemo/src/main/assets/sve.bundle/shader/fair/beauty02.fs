#ifdef GL_ES
precision lowp float;
#endif
varying vec2 v_texcoord0;
uniform sampler2D aTexture0;
uniform vec2 tex0size;
uniform float softenStrength;

void main ()
{
    if(softenStrength==0.0){
       vec4 color=texture2D(aTexture0, v_texcoord0);
       gl_FragColor = color;
       return;
    }
    // float softenStrength=0.8;
    //vec2 imageSize=vec2(imageSize.x,imageSize.y);
    lowp vec3 smoothColor_1;
    lowp float gaussianWeight_2;
    lowp float sample_3;
    lowp float sum_4;
    lowp float gaussianWeightTotal_5;
    float mul_y_6;
    float mul_x_7;
    float tmpvar_8;
    tmpvar_8 = (2.0 / 720.0);
    float tmpvar_9;
    tmpvar_9 = (2.0 / 1280.0);
    vec2 tmpvar_10;
    tmpvar_10.x = 0.0;
    tmpvar_10.y = (-10.0 * tmpvar_9);
    vec2 tmpvar_11;
    tmpvar_11.x = (5.0 * tmpvar_8);
    tmpvar_11.y = (-8.0 * tmpvar_9);
    vec2 tmpvar_12;
    tmpvar_12.x = (8.0 * tmpvar_8);
    tmpvar_12.y = (-5.0 * tmpvar_9);
    vec2 tmpvar_13;
    tmpvar_13.x = (10.0 * tmpvar_8);
    tmpvar_13.y = 0.0;
    vec2 tmpvar_14;
    tmpvar_14.x = (8.0 * tmpvar_8);
    tmpvar_14.y = (5.0 * tmpvar_9);
    vec2 tmpvar_15;
    tmpvar_15.x = (5.0 * tmpvar_8);
    tmpvar_15.y = (8.0 * tmpvar_9);
    vec2 tmpvar_16;
    tmpvar_16.x = 0.0;
    tmpvar_16.y = (10.0 * tmpvar_9);
    vec2 tmpvar_17;
    tmpvar_17.x = (-5.0 * tmpvar_8);
    tmpvar_17.y = (8.0 * tmpvar_9);
    vec2 tmpvar_18;
    tmpvar_18.x = (-8.0 * tmpvar_8);
    tmpvar_18.y = (5.0 * tmpvar_9);
    vec2 tmpvar_19;
    tmpvar_19.x = (-10.0 * tmpvar_8);
    tmpvar_19.y = 0.0;
    vec2 tmpvar_20;
    tmpvar_20.x = (-8.0 * tmpvar_8);
    tmpvar_20.y = (-5.0 * tmpvar_9);
    vec2 tmpvar_21;
    tmpvar_21.x = (-5.0 * tmpvar_8);
    tmpvar_21.y = (-8.0 * tmpvar_9);
    mul_x_7 = (1.6 / 720.0);
    mul_y_6 = (1.6 / 1280.0);
    vec2 tmpvar_22;
    tmpvar_22.x = 0.0;
    tmpvar_22.y = (-6.0 * mul_y_6);
    vec2 tmpvar_23;
    tmpvar_23.x = (-4.0 * mul_x_7);
    tmpvar_23.y = (-4.0 * mul_y_6);
    vec2 tmpvar_24;
    tmpvar_24.x = (-6.0 * mul_x_7);
    tmpvar_24.y = 0.0;
    vec2 tmpvar_25;
    tmpvar_25.x = (-4.0 * mul_x_7);
    tmpvar_25.y = (4.0 * mul_y_6);
    vec2 tmpvar_26;
    tmpvar_26.x = 0.0;
    tmpvar_26.y = (6.0 * mul_y_6);
    vec2 tmpvar_27;
    tmpvar_27.x = (4.0 * mul_x_7);
    tmpvar_27.y = (4.0 * mul_y_6);
    vec2 tmpvar_28;
    tmpvar_28.x = (6.0 * mul_x_7);
    tmpvar_28.y = 0.0;
    vec2 tmpvar_29;
    tmpvar_29.x = (4.0 * mul_x_7);
    tmpvar_29.y = (-4.0 * mul_y_6);
    lowp vec4 tmpvar_30;
    tmpvar_30 = texture2D (aTexture0, v_texcoord0);
    sum_4 = (tmpvar_30.y * 0.2);
    lowp vec4 tmpvar_31;
    tmpvar_31 = texture2D (aTexture0, (v_texcoord0 + tmpvar_10));
    gaussianWeight_2 = (0.08 * (1.0 - min (
                                           (abs((tmpvar_30.y - tmpvar_31.y)) * 3.6)
                                           , 1.0)));
    gaussianWeightTotal_5 = (0.2 + gaussianWeight_2);
    sum_4 = (sum_4 + (tmpvar_31.y * gaussianWeight_2));
    lowp vec4 tmpvar_32;
    tmpvar_32 = texture2D (aTexture0, (v_texcoord0 + tmpvar_11));
    gaussianWeight_2 = (0.08 * (1.0 - min (
                                           (abs((tmpvar_30.y - tmpvar_32.y)) * 3.6)
                                           , 1.0)));
    gaussianWeightTotal_5 = (gaussianWeightTotal_5 + gaussianWeight_2);
    sum_4 = (sum_4 + (tmpvar_32.y * gaussianWeight_2));
    lowp vec4 tmpvar_33;
    tmpvar_33 = texture2D (aTexture0, (v_texcoord0 + tmpvar_12));
    gaussianWeight_2 = (0.08 * (1.0 - min (
                                           (abs((tmpvar_30.y - tmpvar_33.y)) * 3.6)
                                           , 1.0)));
    gaussianWeightTotal_5 = (gaussianWeightTotal_5 + gaussianWeight_2);
    sum_4 = (sum_4 + (tmpvar_33.y * gaussianWeight_2));
    lowp vec4 tmpvar_34;
    tmpvar_34 = texture2D (aTexture0, (v_texcoord0 + tmpvar_13));
    gaussianWeight_2 = (0.08 * (1.0 - min (
                                           (abs((tmpvar_30.y - tmpvar_34.y)) * 3.6)
                                           , 1.0)));
    gaussianWeightTotal_5 = (gaussianWeightTotal_5 + gaussianWeight_2);
    sum_4 = (sum_4 + (tmpvar_34.y * gaussianWeight_2));
    lowp vec4 tmpvar_35;
    tmpvar_35 = texture2D (aTexture0, (v_texcoord0 + tmpvar_14));
    gaussianWeight_2 = (0.08 * (1.0 - min (
                                           (abs((tmpvar_30.y - tmpvar_35.y)) * 3.6)
                                           , 1.0)));
    gaussianWeightTotal_5 = (gaussianWeightTotal_5 + gaussianWeight_2);
    sum_4 = (sum_4 + (tmpvar_35.y * gaussianWeight_2));
    lowp vec4 tmpvar_36;
    tmpvar_36 = texture2D (aTexture0, (v_texcoord0 + tmpvar_15));
    gaussianWeight_2 = (0.08 * (1.0 - min (
                                           (abs((tmpvar_30.y - tmpvar_36.y)) * 3.6)
                                           , 1.0)));
    gaussianWeightTotal_5 = (gaussianWeightTotal_5 + gaussianWeight_2);
    sum_4 = (sum_4 + (tmpvar_36.y * gaussianWeight_2));
    lowp vec4 tmpvar_37;
    tmpvar_37 = texture2D (aTexture0, (v_texcoord0 + tmpvar_16));
    gaussianWeight_2 = (0.08 * (1.0 - min (
                                           (abs((tmpvar_30.y - tmpvar_37.y)) * 3.6)
                                           , 1.0)));
    gaussianWeightTotal_5 = (gaussianWeightTotal_5 + gaussianWeight_2);
    sum_4 = (sum_4 + (tmpvar_37.y * gaussianWeight_2));
    lowp vec4 tmpvar_38;
    tmpvar_38 = texture2D (aTexture0, (v_texcoord0 + tmpvar_17));
    gaussianWeight_2 = (0.08 * (1.0 - min (
                                           (abs((tmpvar_30.y - tmpvar_38.y)) * 3.6)
                                           , 1.0)));
    gaussianWeightTotal_5 = (gaussianWeightTotal_5 + gaussianWeight_2);
    sum_4 = (sum_4 + (tmpvar_38.y * gaussianWeight_2));
    lowp vec4 tmpvar_39;
    tmpvar_39 = texture2D (aTexture0, (v_texcoord0 + tmpvar_18));
    gaussianWeight_2 = (0.08 * (1.0 - min (
                                           (abs((tmpvar_30.y - tmpvar_39.y)) * 3.6)
                                           , 1.0)));
    gaussianWeightTotal_5 = (gaussianWeightTotal_5 + gaussianWeight_2);
    sum_4 = (sum_4 + (tmpvar_39.y * gaussianWeight_2));
    lowp vec4 tmpvar_40;
    tmpvar_40 = texture2D (aTexture0, (v_texcoord0 + tmpvar_19));
    gaussianWeight_2 = (0.08 * (1.0 - min (
                                           (abs((tmpvar_30.y - tmpvar_40.y)) * 3.6)
                                           , 1.0)));
    gaussianWeightTotal_5 = (gaussianWeightTotal_5 + gaussianWeight_2);
    sum_4 = (sum_4 + (tmpvar_40.y * gaussianWeight_2));
    lowp vec4 tmpvar_41;
    tmpvar_41 = texture2D (aTexture0, (v_texcoord0 + tmpvar_20));
    gaussianWeight_2 = (0.08 * (1.0 - min (
                                           (abs((tmpvar_30.y - tmpvar_41.y)) * 3.6)
                                           , 1.0)));
    gaussianWeightTotal_5 = (gaussianWeightTotal_5 + gaussianWeight_2);
    sum_4 = (sum_4 + (tmpvar_41.y * gaussianWeight_2));
    lowp vec4 tmpvar_42;
    tmpvar_42 = texture2D (aTexture0, (v_texcoord0 + tmpvar_21));
    gaussianWeight_2 = (0.08 * (1.0 - min (
                                           (abs((tmpvar_30.y - tmpvar_42.y)) * 3.6)
                                           , 1.0)));
    gaussianWeightTotal_5 = (gaussianWeightTotal_5 + gaussianWeight_2);
    sum_4 = (sum_4 + (tmpvar_42.y * gaussianWeight_2));
    lowp vec4 tmpvar_43;
    tmpvar_43 = texture2D (aTexture0, (v_texcoord0 + tmpvar_22));
    gaussianWeight_2 = (0.1 * (1.0 - min (
                                          (abs((tmpvar_30.y - tmpvar_43.y)) * 3.6)
                                          , 1.0)));
    gaussianWeightTotal_5 = (gaussianWeightTotal_5 + gaussianWeight_2);
    sum_4 = (sum_4 + (tmpvar_43.y * gaussianWeight_2));
    lowp vec4 tmpvar_44;
    tmpvar_44 = texture2D (aTexture0, (v_texcoord0 + tmpvar_23));
    gaussianWeight_2 = (0.1 * (1.0 - min (
                                          (abs((tmpvar_30.y - tmpvar_44.y)) * 3.6)
                                          , 1.0)));
    gaussianWeightTotal_5 = (gaussianWeightTotal_5 + gaussianWeight_2);
    sum_4 = (sum_4 + (tmpvar_44.y * gaussianWeight_2));
    lowp vec4 tmpvar_45;
    tmpvar_45 = texture2D (aTexture0, (v_texcoord0 + tmpvar_24));
    gaussianWeight_2 = (0.1 * (1.0 - min (
                                          (abs((tmpvar_30.y - tmpvar_45.y)) * 3.6)
                                          , 1.0)));
    gaussianWeightTotal_5 = (gaussianWeightTotal_5 + gaussianWeight_2);
    sum_4 = (sum_4 + (tmpvar_45.y * gaussianWeight_2));
    lowp vec4 tmpvar_46;
    tmpvar_46 = texture2D (aTexture0, (v_texcoord0 + tmpvar_25));
    gaussianWeight_2 = (0.1 * (1.0 - min (
                                          (abs((tmpvar_30.y - tmpvar_46.y)) * 3.6)
                                          , 1.0)));
    gaussianWeightTotal_5 = (gaussianWeightTotal_5 + gaussianWeight_2);
    sum_4 = (sum_4 + (tmpvar_46.y * gaussianWeight_2));
    lowp vec4 tmpvar_47;
    tmpvar_47 = texture2D (aTexture0, (v_texcoord0 + tmpvar_26));
    gaussianWeight_2 = (0.1 * (1.0 - min (
                                          (abs((tmpvar_30.y - tmpvar_47.y)) * 3.6)
                                          , 1.0)));
    gaussianWeightTotal_5 = (gaussianWeightTotal_5 + gaussianWeight_2);
    sum_4 = (sum_4 + (tmpvar_47.y * gaussianWeight_2));
    lowp vec4 tmpvar_48;
    tmpvar_48 = texture2D (aTexture0, (v_texcoord0 + tmpvar_27));
    gaussianWeight_2 = (0.1 * (1.0 - min (
                                          (abs((tmpvar_30.y - tmpvar_48.y)) * 3.6)
                                          , 1.0)));
    gaussianWeightTotal_5 = (gaussianWeightTotal_5 + gaussianWeight_2);
    sum_4 = (sum_4 + (tmpvar_48.y * gaussianWeight_2));
    lowp vec4 tmpvar_49;
    tmpvar_49 = texture2D (aTexture0, (v_texcoord0 + tmpvar_28));
    gaussianWeight_2 = (0.1 * (1.0 - min (
                                          (abs((tmpvar_30.y - tmpvar_49.y)) * 3.6)
                                          , 1.0)));
    gaussianWeightTotal_5 = (gaussianWeightTotal_5 + gaussianWeight_2);
    sum_4 = (sum_4 + (tmpvar_49.y * gaussianWeight_2));
    lowp vec4 tmpvar_50;
    tmpvar_50 = texture2D (aTexture0, (v_texcoord0 + tmpvar_29));
    gaussianWeight_2 = (0.1 * (1.0 - min (
                                          (abs((tmpvar_30.y - tmpvar_50.y)) * 3.6)
                                          , 1.0)));
    gaussianWeightTotal_5 = (gaussianWeightTotal_5 + gaussianWeight_2);
    sum_4 = (sum_4 + (tmpvar_50.y * gaussianWeight_2));
    sum_4 = (sum_4 / gaussianWeightTotal_5);
    sample_3 = ((tmpvar_30.y - sum_4) + 0.5);
    if ((sample_3 <= 0.5)) {
        sample_3 = ((sample_3 * sample_3) * 2.0);
    } else {
        sample_3 = (1.0 - ((
                            (1.0 - sample_3)
                            *
                            (1.0 - sample_3)
                            ) * 2.0));
    };
    if ((sample_3 <= 0.5)) {
        sample_3 = ((sample_3 * sample_3) * 2.0);
    } else {
        sample_3 = (1.0 - ((
                            (1.0 - sample_3)
                            *
                            (1.0 - sample_3)
                            ) * 2.0));
    };
    if ((sample_3 <= 0.5)) {
        sample_3 = ((sample_3 * sample_3) * 2.0);
    } else {
        sample_3 = (1.0 - ((
                            (1.0 - sample_3)
                            *
                            (1.0 - sample_3)
                            ) * 2.0));
    };
    if ((sample_3 <= 0.5)) {
        sample_3 = ((sample_3 * sample_3) * 2.0);
    } else {
        sample_3 = (1.0 - ((
                            (1.0 - sample_3)
                            *
                            (1.0 - sample_3)
                            ) * 2.0));
    };
    if ((sample_3 <= 0.5)) {
        sample_3 = ((sample_3 * sample_3) * 2.0);
    } else {
        sample_3 = (1.0 - ((
                            (1.0 - sample_3)
                            *
                            (1.0 - sample_3)
                            ) * 2.0));
    };
    lowp float tmpvar_51;
    tmpvar_51 = (1.0 + (pow (sum_4, 0.3) * 0.09));
    lowp vec3 tmpvar_52;
    tmpvar_52 = mix (tmpvar_30.xyz, mix (tmpvar_30.xyz, clamp (
                                                               ((tmpvar_30.xyz * tmpvar_51) - (vec3(sample_3) * (tmpvar_51 - 1.0)))
                                                               , vec3(0.0, 0.0, 0.0), vec3(1.0, 1.0, 1.0)), pow (tmpvar_30.y, 0.33)), pow (tmpvar_30.y, 0.39));
    mediump vec3 tmpvar_53;
    tmpvar_53 = mix (tmpvar_30.xyz, tmpvar_52, softenStrength);
    smoothColor_1 = tmpvar_53;
    lowp vec4 tmpvar_54;
    tmpvar_54.w = 1.0;
    tmpvar_54.xyz = pow (smoothColor_1, vec3(0.96, 0.96, 0.96));
    gl_FragColor = tmpvar_54;
}
