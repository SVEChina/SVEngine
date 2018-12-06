//
//  SWPreDef.h
//  SVEVideo
//
//  Created by 李晓帆 on 2018/7/6.
//  Copyright © 2018年 付一洲. All rights reserved.
//

#ifndef SWPreDef_h
#define SWPreDef_h
#define SCENENAME @"showtool"
typedef enum DETECTIONTYPE{
    SWDETECTSENSETIME = 1,
    SWDETECTFACEPP
} SWDETECTION;

typedef enum STREAMTYPE{
    SWSTREAMSAMPLE = 1,
    SWSTREAMBUFFER,
} SWSTREAMTYPE;

//数据源类型
typedef enum DATASOURCE{
    DATASRCNONE = 0,
    DATASRCCAMERA = 1,
    DATASRCARCAMERA,
    DATASRCPIC,
    DATASRCVIDEO,
    DATASRCNET
} SWDATASRCTYPE;
#endif /* SWPreDef_h */
