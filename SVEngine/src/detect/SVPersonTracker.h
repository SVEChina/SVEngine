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
            
            void track_st(void *_data, s32 _ptnum, SVRect &_rect, f32 yaw, f32 pitch, f32 roll, s32 personID);
            
            void setEnableExpression(bool _enable);
            
            f32 getEyeDis();
            
        public:
            FVec3 m_facecenter;
            FVec3 m_noisecenter;
            FVec3 m_eyecenter;
            FVec3 m_jawbottompos;
            FVec3 m_noiseup;
            FVec3 m_noisedown;
            FVec3 m_leftEyePos;
            FVec3 m_rightEyePos;
            SVRect m_faceRect;
            f32 *m_pFacedata;
            s32 m_personID;
            f32 m_img_w;
            f32 m_img_h;
            f32 m_yaw;
            f32 m_roll;
            f32 m_pitch;
            f32 m_eyeDistance;      //瞳距
            f32 m_noiseDistance;    //鼻子长度
            f32 m_eyestd_scale;     //瞳距相对于标准脸的缩放
            f32 m_noisetd_scale;
            f32 m_eye_angle;
            
        protected:
            void _sendExpressionEvent(SV_EVENT_TYPE _type);
            //精细表情识别
            void _trackExpression_st();//表情识别
            void _correctStandardface_st();//矫正标准脸
            void _trackExpression_as();
            void _correctStandardface_as();
            void _trackExpression_fp();
            void _correctStandardface_fp();
            f32 m_standard_lefteye_high;
            f32 m_standard_righteye_high;
            f32 m_standard_mouth_high;
            f32 m_standard_mouth_width;
            f32 m_standard_mouth_angle_leftcorner_upperliptop_rightcorner;
            f32 m_standard_mouth_angle_leftcorner_lowerlipbottom_rightcorner;
        protected:
            bool m_enableExpression;
        };
        
    }//!namespace detect
    
}//!namespace sv

#endif //SV_PERSONTRACKER_H
