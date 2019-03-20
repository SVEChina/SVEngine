//
//  SVRGBGradient.cpp
//  SVEngine
//
//  Created by 徐子昱 on 2019/3/16.
//  Copyright © 2019 李晓帆. All rights reserved.
//

#include "SVRGBGradient.h"



//void createRgbGradient(SVArray<FVec3> _colors , ptr8 outRgba){
//    s32 t_size = 256/(_colors.size()-1);
//    s32 t_index=0;
//    FVec3 t_startColor;
//    FVec3 t_endColor;
//    FVec3 t_colorin;
//    FVec3 t_rgb;
//    
//    for(s32 i = 0; i < _colors.size()-1; i++){
//        t_startColor =_colors[i]
//        t_endColor =_colors[i+1];
//        t_colorin = t_startColor - t_endColor;
//        for(s32 j = 0 ; j < t_size; j++ ){
//            t_rgb = t_endColor + (s32)(t_colorin * ((f32)j/t_size) + 0.5f);
//            outRgba[t_index*4]=(u8)t_rgb.x;
//            outRgba[t_index*4+1]=(u8)t_rgb.y;
//            outRgba[t_index*4+2]=(u8)t_rgb.z;
//            outRgba[t_index*4+3]=255;
//            t_index++;
//        }
//    }
//}
