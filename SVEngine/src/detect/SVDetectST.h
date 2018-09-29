//
// SVDetectST.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_DETECTST_H
#define SV_DETECTST_H

#include "SVDetectBase.h"

#define ST_POINT_NUM 106
namespace sv {
    
    namespace detect{
        struct ST_PT {
            f32 x;
            f32 y;
        };
        struct PERSON_STDATA {
            bool has;
            f32 yaw;
            f32 pitch;
            f32 roll;
            s32 recttop;
            s32 rectbottom;
            s32 rectright;
            s32 rectleft;
            ST_PT pointdata[ST_POINT_NUM];
        };
        
        class SVDetectST : public SVDetectBase {
        public:
            SVDetectST(SVInst *_app);
            
            ~SVDetectST();
            
            virtual s32 transformIndex(s32 index);
           
            void pushData(void *_faceData);
            
            PERSON_STDATA *getData(s32 _personid);
            
            PERSON_STDATA *getDataOriginal(s32 _personid);
            
        public:
             PERSON_STDATA stData[SUPPORT_MAX_PERSON_NUM];
             PERSON_STDATA stDataOriginal[SUPPORT_MAX_PERSON_NUM];
        protected:
            //修正到以屏幕中心为原点的坐标系。
            void _transformToCenter(PERSON_STDATA *_stData);
            void _parseData(void *data);
        };

    }//!namespace detect
    
}//!namespace sv



#endif //SV_DETECTST_H
