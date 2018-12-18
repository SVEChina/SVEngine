//
// SVPersonTracker.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PERSONTRACKER_H
#define SV_PERSONTRACKER_H

#include "../base/SVGBase.h"
#include "../event/SVEventDef.h"
#include "../base/SVMat4d.h"
#include "../base/SVGBase.h"
#include "../base/SVColor.h"
#include "../base/SVVec3.h"
#include "../base/SVRect.h"

namespace sv {
    
    namespace detect{
        
        class SVPersonTracker : public SVGBase {
        public:
            SVPersonTracker(SVInst* _app);
            
            virtual ~SVPersonTracker();
            
            void track_st(void *_data, s32 _ptnum, SVRect *_rect, f32 yaw, f32 pitch, f32 roll, s32 personID);
            
        public:
            s32 m_personID;
            f32 m_img_w;
            f32 m_img_h;
            FVec3 m_facecenter;
            FVec3 m_noisecenter;
            FVec3 m_eyecenter;
            FVec3 m_jawbottompos;
            FVec3 m_noiseup;
            FVec3 m_noisedown;
            SVRect m_faceRect;
            f32 m_standardEyeDis;
            f32 m_eyeDistance;      //瞳距
            FVec3 m_scale;           //pitch yaw roll
            f32 m_eyestd_scale;     //瞳距相对于标准脸的缩放
        };
        
    }//!namespace detect
    
}//!namespace sv



#endif //SV_PERSONTRACKER_H
