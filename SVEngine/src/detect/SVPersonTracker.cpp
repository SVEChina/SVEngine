//
// SVPersonTracker.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVPersonTracker.h"
#include "../basesys/SVConfig.h"
#include "../event/SVEvent.h"
#include "../event/SVEventMgr.h"
#include "../app/SVInst.h"

#define STANDARD_EYE_STD 311
#define STANDARD_NOISE_STD 225

SVPersonTracker::SVPersonTracker(SVInst* _app)
:SVGBase(_app) {
    m_img_w = 720;
    m_img_h = 1280;
    m_personID = 0;
    m_yaw = 0.0f;
    m_roll = 0.0f;
    m_pitch = 0.0f;
    m_enableCorrect = false;
}

SVPersonTracker::~SVPersonTracker() {

}

void SVPersonTracker::startCorrectStandardface(){
    m_enableCorrect = true;
}

void SVPersonTracker::stopCorrectStandardface(){
    m_enableCorrect = false;
}

void SVPersonTracker::track_st(void *_data, s32 _ptnum, SVRect &_rect, f32 yaw, f32 pitch, f32 roll, s32 personID) {
    m_pFacedata = (f32 *) _data;
    m_personID = personID;
    m_yaw = yaw;
    m_roll = roll;
    m_pitch = pitch;
    m_img_w = mApp->m_pGlobalParam->m_inner_width;
    m_img_h = mApp->m_pGlobalParam->m_inner_height;
    //面部区域
    m_faceRect = _rect;
    {
        //双眼正中位置
        m_eyecenter.x = m_pFacedata[43 * 2];
        m_eyecenter.y = m_pFacedata[43 * 2 + 1];
        m_eyecenter.z = 0;

        //面部中心位置
        m_facecenter.x = m_pFacedata[45 * 2];
        m_facecenter.y = m_pFacedata[45 * 2 + 1];
        m_facecenter.z = 0;

        //鼻子上
        m_noiseup.x = m_pFacedata[44 * 2];
        m_noiseup.y = m_pFacedata[44 * 2 + 1];
        m_noiseup.z = 0;

        //鼻子中心
        m_noisecenter.x = m_pFacedata[46 * 2];
        m_noisecenter.y = m_pFacedata[46 * 2 + 1];
        m_noisecenter.z = 0;

        //鼻子下
        m_noisedown.x = m_pFacedata[49 * 2];
        m_noisedown.y = m_pFacedata[49 * 2 + 1];
        m_noisedown.z = 0;

        m_jawbottompos.x = m_pFacedata[16 * 2];
        m_jawbottompos.y = m_pFacedata[16 * 2 + 1];
        m_jawbottompos.z = 0;

        m_leftEyePos.x = m_pFacedata[74 * 2];
        m_leftEyePos.y = m_pFacedata[74 * 2 + 1];
        m_leftEyePos.z = 0;
        
        m_rightEyePos.x = m_pFacedata[77 * 2];
        m_rightEyePos.y = m_pFacedata[77 * 2 + 1];
        m_rightEyePos.z = 0;
        
        
        f32 t_yaw_ratio = fabs(cosf(yaw*DEGTORAD));
        if (t_yaw_ratio == 0.0f) {
            t_yaw_ratio = 1.0f;
        }
        
        f32 t_pitch_ratio = fabs(cosf(pitch*DEGTORAD));
        if (t_pitch_ratio == 0.0f) {
            t_pitch_ratio = 1.0f;
        }
        m_eyeDistance = (m_leftEyePos - m_rightEyePos).length() / t_yaw_ratio;
        m_noiseDistance = (m_eyecenter - m_noisedown).length() / t_pitch_ratio;
        //横向缩放
        m_eyestd_scale = m_eyeDistance / STANDARD_EYE_STD;
        //竖向缩放
        m_noisetd_scale = m_noiseDistance / STANDARD_NOISE_STD;
        //
        m_eye_angle = atan2f(m_eyecenter.y - m_noisedown.y, m_eyecenter.x - m_noisedown.x)*RADTODEG - 90;
    }
    if (m_enableCorrect) {
        _correctStandardface_st();
    }
    //精细表情识别
    _trackExpression_st();
}

///
void SVPersonTracker::_trackExpression_st(){
    if (m_pFacedata == nullptr) {
        return;
    }
    //上嘴唇顶部
    s32 t_index_mouth_upperlip_top = 87;
    FVec2 t_pt_mouth_upperlip_top = FVec2(m_pFacedata[2 * t_index_mouth_upperlip_top], m_pFacedata[2 * t_index_mouth_upperlip_top + 1]);
    //上嘴唇下部
    s32 t_index_mouth_upperlip_bottom = 98;
    FVec2 t_pt_mouth_upperlip_bottom = FVec2(m_pFacedata[2 * t_index_mouth_upperlip_bottom], m_pFacedata[2 * t_index_mouth_upperlip_bottom + 1]);
    //下嘴唇高度
    f32 t_len_mouth_upperlip_high = (t_pt_mouth_upperlip_top - t_pt_mouth_upperlip_bottom).length();
    //下嘴唇上部
    s32 t_index_mouth_lowerlip_top = 102;
    FVec2 t_pt_mouth_lowerlip_top = FVec2(m_pFacedata[2 * t_index_mouth_lowerlip_top], m_pFacedata[2 * t_index_mouth_lowerlip_top + 1]);
    //下嘴唇下部
    s32 t_index_mouth_lowerlip_bottom = 93;
    FVec2 t_pt_mouth_lowerlip_bottom = FVec2(m_pFacedata[2 * t_index_mouth_lowerlip_bottom], m_pFacedata[2 * t_index_mouth_lowerlip_bottom + 1]);
    //下嘴唇高度
    f32 t_len_mouth_lowerlip_high = (t_pt_mouth_lowerlip_top - t_pt_mouth_lowerlip_bottom).length();
    //张嘴高度
    f32 t_mouthHigh = (t_pt_mouth_upperlip_bottom - t_pt_mouth_lowerlip_top).length();
    //左眼上部
    s32 t_index_lefteye_top = 72;
    FVec2 t_pt_lefteye_top = FVec2( m_pFacedata[2 * t_index_lefteye_top], m_pFacedata[2 * t_index_lefteye_top + 1]);
    //左眼下部
    s32 t_index_lefteye_bottom = 73;
    FVec2 t_pt_lefteye_bottom = FVec2(m_pFacedata[2 * t_index_lefteye_bottom], m_pFacedata[2 * t_index_lefteye_bottom + 1]);
    //左眼高度
    f32 t_leftEyeHigh = (t_pt_lefteye_top - t_pt_lefteye_bottom).length();
    //左眼左眼角
    s32 t_index_lefteye_leftcorner = 52;
    FVec2 t_pt_lefteye_leftcorner = FVec2(m_pFacedata[2 * t_index_lefteye_leftcorner], m_pFacedata[2 * t_index_lefteye_leftcorner + 1]);
    //左眼右眼角
    s32 t_index_lefteye_rightcorner = 55;
    FVec2 t_pt_lefteye_rightcorner = FVec2(m_pFacedata[2 * t_index_lefteye_rightcorner], m_pFacedata[2 * t_index_lefteye_rightcorner + 1]);
    //左眼宽度
    f32 t_leftEyeWidth = (t_pt_lefteye_leftcorner - t_pt_lefteye_rightcorner).length();
    //右眼上部
    s32 t_index_righteye_top = 75;
    FVec2 t_pt_righteye_top = FVec2(m_pFacedata[2 * t_index_righteye_top], m_pFacedata[2 * t_index_righteye_top + 1]);
    //右眼下部
    s32 t_index_righteye_bottom = 76;
    FVec2 t_pt_righteye_bottom = FVec2(m_pFacedata[2 * t_index_righteye_bottom], m_pFacedata[2 * t_index_righteye_bottom + 1]);
    //右眼高度
    f32 t_rightEyeHigh = (t_pt_righteye_top - t_pt_righteye_bottom).length();
    //右眼左眼角
    s32 t_index_righteye_leftcorner = 58;
    FVec2 t_pt_righteye_leftcorner = FVec2(m_pFacedata[2 * t_index_righteye_leftcorner], m_pFacedata[2 * t_index_righteye_leftcorner + 1]);
    //右眼右眼角
    s32 t_index_righteye_rightcorner = 61;
    FVec2 t_pt_righteye_rightcorner = FVec2(m_pFacedata[2 * t_index_righteye_rightcorner], m_pFacedata[2 * t_index_righteye_rightcorner + 1]);
    //右眼宽度
    f32 t_rightEyeWidth = (t_pt_righteye_leftcorner - t_pt_righteye_rightcorner).length();
    
    //左嘴角
    s32 t_index_mouth_leftcorner = 84;
    FVec2 t_pt_mouth_leftcorner = FVec2(m_pFacedata[2 * t_index_mouth_leftcorner], m_pFacedata[2 * t_index_mouth_leftcorner + 1]);
    //右嘴角
    s32 t_index_mouth_rightcorner = 90;
    FVec2 t_pt_mouth_rightcorner = FVec2(m_pFacedata[2 * t_index_mouth_rightcorner], m_pFacedata[2 * t_index_mouth_rightcorner + 1]);
    //嘴宽度
    f32 t_mouthWidth = (t_pt_mouth_leftcorner - t_pt_mouth_rightcorner).length();
    
    
//    
//    //////////////////////嘟嘴
//    float t_vec_mouth_leftcorner_upperliptop_x = t_pt_mouth_leftcorner_x - t_pt_mouth_upperlip_top_x;
//    float t_vec_mouth_leftcorner_upperliptop_y = t_pt_mouth_leftcorner_y - t_pt_mouth_upperlip_top_y;
//    float t_vec_mouth_rightcorner_upperliptop_x = t_pt_mouth_rightcorner_x - t_pt_mouth_upperlip_top_x;
//    float t_vec_mouth_rightcorner_upperliptop_y = t_pt_mouth_rightcorner_y - t_pt_mouth_upperlip_top_y;
//    float t_vec_mouth_upperlipbottom_upperliptop_x = t_pt_mouth_upperlip_bottom_x - t_pt_mouth_upperlip_top_x;
//    float t_vec_mouth_upperlipbottom_upperliptop_y = t_pt_mouth_upperlip_bottom_y - t_pt_mouth_upperlip_top_y;
//    float t_vec_mouth_up = (t_vec_mouth_leftcorner_upperliptop_x * t_vec_mouth_rightcorner_upperliptop_x +
//                            t_vec_mouth_leftcorner_upperliptop_y * t_vec_mouth_rightcorner_upperliptop_y);
//    float t_vec_left_mouth_up = (t_vec_mouth_leftcorner_upperliptop_x * t_vec_mouth_upperlipbottom_upperliptop_x +
//                                 t_vec_mouth_leftcorner_upperliptop_y * t_vec_mouth_upperlipbottom_upperliptop_y);
//    float t_vec_right_mouth_up = (t_vec_mouth_rightcorner_upperliptop_x * t_vec_mouth_upperlipbottom_upperliptop_x +
//                                  t_vec_mouth_rightcorner_upperliptop_y * t_vec_mouth_upperlipbottom_upperliptop_y);
//    float t_len_mouth_leftcorner_upperliptop = MaMathHelper::vec2Length(t_pt_mouth_leftcorner_x, t_pt_mouth_leftcorner_y,
//                                                                        t_pt_mouth_upperlip_top_x, t_pt_mouth_upperlip_top_y);
//    float t_len_rightcorner_upperliptop = MaMathHelper::vec2Length(t_pt_mouth_rightcorner_x, t_pt_mouth_rightcorner_y,
//                                                                   t_pt_mouth_upperlip_top_x, t_pt_mouth_upperlip_top_y);
//    //
//    float t_angle_mouth_leftcorner_upperliptop_rightcorner =
//    acosf(t_vec_mouth_up / (t_len_mouth_leftcorner_upperliptop * t_len_rightcorner_upperliptop)) * TOANGLE;
//    float t_angle_mouth_leftcorner_upperliptop_upperlipbottom =
//    acosf(t_vec_left_mouth_up / (t_len_mouth_leftcorner_upperliptop * t_len_mouth_upperlip_high)) * TOANGLE;
//    float t_angle_mouth_rightcorner_upperliptop_upperlipbottom =
//    acosf(t_vec_right_mouth_up / (t_len_rightcorner_upperliptop * t_len_mouth_upperlip_high)) * TOANGLE;
//    //
//    float t_vec_mouth_leftcorner_lowerlipbottom_x = t_pt_mouth_leftcorner_x - t_pt_mouth_lowerlip_bottom_x;
//    float t_vec_mouth_leftcorner_lowerlipbottom_y = t_pt_mouth_leftcorner_y - t_pt_mouth_lowerlip_bottom_y;
//    float t_vec_mouth_rightcorner_lowerlipbottom_x = t_pt_mouth_rightcorner_x - t_pt_mouth_lowerlip_bottom_x;
//    float t_vec_mouth_rightcorner_lowerlipbottom_y = t_pt_mouth_rightcorner_y - t_pt_mouth_lowerlip_bottom_y;
//    float t_vec_mouth_bottom = (t_vec_mouth_leftcorner_lowerlipbottom_x * t_vec_mouth_rightcorner_lowerlipbottom_x +
//                                t_vec_mouth_leftcorner_lowerlipbottom_y * t_vec_mouth_rightcorner_lowerlipbottom_y);
//    float t_len_mouth_leftcorner_lowerlipbottom = MaMathHelper::vec2Length(t_pt_mouth_leftcorner_x, t_pt_mouth_leftcorner_y,
//                                                                           t_pt_mouth_lowerlip_bottom_x,
//                                                                           t_pt_mouth_lowerlip_bottom_y);
//    float t_len_mouth_rightcorner_lowerlipbottom = MaMathHelper::vec2Length(t_pt_mouth_rightcorner_x, t_pt_mouth_rightcorner_y,
//                                                                            t_pt_mouth_lowerlip_bottom_x,
//                                                                            t_pt_mouth_lowerlip_bottom_y);
//    float t_angle_mouth_leftcorner_lowerlipbottom_rightcorner =
//    acosf(t_vec_mouth_bottom / (t_len_mouth_leftcorner_lowerlipbottom * t_len_mouth_rightcorner_lowerlipbottom)) *
//    TOANGLE;
//    
//    //设置默认标准标记参数
//    if (!m_haveStanderFace) {
//        m_leftStandardEyeHigh = max_(t_leftEyeWidth * 0.35, 24.0);
//        m_rightStandardEyeHigh = max_(t_rightEyeWidth * 0.35, 24.0);
//        m_standard_angle_leftcorner_upperliptop_rightcorner = 155;
//        m_standard_angle_leftcorner_lowerlipbottom_rightcorner = 130;
//        
//    }
//    m_mouthStandardWidth = t_mouthWidth;
//    //
//#ifdef _MK_ANDROID_
//    float t_eyeLeftCloseValue;
//    float t_eyeRightCloseValue;
//    m_ex_lastLeftEyeOpen = m_ex_leftEyeOpen;
//    m_ex_lastRightEyeOpen = m_ex_rightEyeOpen;
//    if (!mApp->mGMgr->maConfig->cameraMirror) {
//        float t_eyeLeftCloseValue = min_(t_leftEyeWidth * m_fEyeCloseAdjust, 24.0f);
//        float t_leftEyeDifference = m_leftStandardEyeHigh - t_leftEyeHigh;
//        if (t_leftEyeDifference > t_eyeLeftCloseValue) {
//            m_ex_leftEyeOpen.trigger = false;
//            float t_t_leftEyeWidth = (t_leftEyeWidth*0.5);
//            float t_confidence = 1.0 - t_leftEyeHigh/t_t_leftEyeWidth;
//            float confidence = min_(fabsf(t_confidence), 1.0f);
//            m_ex_leftEyeOpen.confidence = confidence;
//        } else {
//            m_ex_leftEyeOpen.trigger = true;
//            float t_t_leftEyeWidth = (t_leftEyeWidth*0.5);
//            float confidence = min_(fabsf(t_leftEyeHigh/t_t_leftEyeWidth), 1.0f);
//            m_ex_rightEyeOpen.confidence = confidence;
//        }
//        
//        float t_eyeRightCloseValue = min_(t_rightEyeWidth * m_fEyeCloseAdjust, 24.0f);
//        float t_rightEyeDifference = m_rightStandardEyeHigh - t_rightEyeHigh;
//        if (t_rightEyeDifference > t_eyeRightCloseValue) {
//            m_ex_rightEyeOpen.trigger = false;
//            float t_t_rightEyeWidth = (t_rightEyeWidth*0.5);
//            float t_confidence = 1.0 - t_rightEyeHigh/t_t_rightEyeWidth;
//            float confidence = min_(fabsf(t_confidence), 1.0f);
//            m_ex_rightEyeOpen.confidence = confidence;
//        } else {
//            m_ex_rightEyeOpen.trigger = true;
//            float t_t_rightEyeWidth = (t_rightEyeWidth*0.5);
//            float confidence = min_(fabsf(t_rightEyeHigh/t_t_rightEyeWidth), 1.0f);
//            m_ex_rightEyeOpen.confidence = confidence;
//        }
//    } else {
//        float t_eyeLeftCloseValue = min_(t_leftEyeWidth * m_fEyeCloseAdjust, 24.0f);
//        float t_leftEyeDifference = m_leftStandardEyeHigh - t_leftEyeHigh;
//        if (t_leftEyeDifference > t_eyeLeftCloseValue) {
//            m_ex_leftEyeOpen.trigger = false;
//            float t_t_leftEyeWidth = (t_leftEyeWidth*0.5);
//            float t_confidence = 1.0 - t_leftEyeHigh/t_t_leftEyeWidth;
//            float confidence = min_(fabsf(t_confidence), 1.0f);
//            m_ex_leftEyeOpen.confidence = confidence;
//        } else {
//            m_ex_leftEyeOpen.trigger = true;
//            float t_t_leftEyeWidth = (t_leftEyeWidth*0.5);
//            float confidence = min_(fabsf(t_leftEyeHigh/t_t_leftEyeWidth), 1.0f);
//            m_ex_rightEyeOpen.confidence = confidence;
//        }
//        
//        float t_eyeRightCloseValue = min_(t_rightEyeWidth * m_fEyeCloseAdjust, 24.0f);
//        float t_rightEyeDifference = m_rightStandardEyeHigh - t_rightEyeHigh;
//        if (t_rightEyeDifference > t_eyeRightCloseValue) {
//            m_ex_rightEyeOpen.trigger = false;
//            float t_t_rightEyeWidth = (t_rightEyeWidth*0.5);
//            float t_confidence = 1.0 - t_rightEyeHigh/t_t_rightEyeWidth;
//            float confidence = min_(fabsf(t_confidence), 1.0f);
//            m_ex_rightEyeOpen.confidence = confidence;
//        } else {
//            m_ex_rightEyeOpen.trigger = true;
//            float t_t_rightEyeWidth = (t_rightEyeWidth*0.5);
//            float confidence = min_(fabsf(t_rightEyeHigh/t_t_rightEyeWidth), 1.0f);
//            m_ex_rightEyeOpen.confidence = confidence;
//        }
//    }
//#else
//    m_ex_lastLeftEyeOpen = m_ex_leftEyeOpen;
//    float t_eyeLeftCloseValue = min_(t_leftEyeWidth * m_fEyeCloseAdjust, 24.0f);
//    float t_leftEyeDifference = m_leftStandardEyeHigh - t_leftEyeHigh;
//    if (t_leftEyeDifference > t_eyeLeftCloseValue) {
//        m_ex_leftEyeOpen.trigger = false;
//        float t_t_leftEyeWidth = (t_leftEyeWidth*0.5);
//        float t_confidence = 1.0 - t_leftEyeHigh/t_t_leftEyeWidth;
//        float confidence = min_(fabsf(t_confidence), 1.0f);
//        m_ex_leftEyeOpen.confidence = confidence;
//    } else {
//        m_ex_leftEyeOpen.trigger = true;
//        float t_t_leftEyeWidth = (t_leftEyeWidth*0.5);
//        float confidence = min_(fabsf(t_leftEyeHigh/t_t_leftEyeWidth), 1.0f);
//        m_ex_rightEyeOpen.confidence = confidence;
//    }
//    
//    m_ex_lastRightEyeOpen = m_ex_rightEyeOpen;
//    float t_eyeRightCloseValue = min_(t_rightEyeWidth * m_fEyeCloseAdjust, 24.0f);
//    float t_rightEyeDifference = m_rightStandardEyeHigh - t_rightEyeHigh;
//    if (t_rightEyeDifference > t_eyeRightCloseValue) {
//        m_ex_rightEyeOpen.trigger = false;
//        float t_t_rightEyeWidth = (t_rightEyeWidth*0.5);
//        float t_confidence = 1.0 - t_rightEyeHigh/t_t_rightEyeWidth;
//        float confidence = min_(fabsf(t_confidence), 1.0f);
//        m_ex_rightEyeOpen.confidence = confidence;
//    } else {
//        m_ex_rightEyeOpen.trigger = true;
//        float t_t_rightEyeWidth = (t_rightEyeWidth*0.5);
//        float confidence = min_(fabsf(t_rightEyeHigh/t_t_rightEyeWidth), 1.0f);
//        m_ex_rightEyeOpen.confidence = confidence;
//    }
//#endif
//    //张嘴
//    if (t_mouthHigh - m_mouthStandardWidth * m_fMouseOpenAdjust > 0) {
//        float confidence = min_(fabsf(t_mouthHigh/m_mouthStandardWidth), 1.0f);
//        m_ex_mouthOpen.confidence = confidence;
//        m_ex_mouthOpen.expValue = t_mouthHigh;
//        m_ex_mouthOpen.trigger = true;
//    } else {
//        m_ex_mouthOpen.trigger = false;
//        m_ex_mouthOpen.confidence = 1.0f;
//    }
//    
//    //闭嘴
//    if ((m_ex_mouthOpen.trigger == false) && (t_mouthHigh - m_mouthStandardWidth * m_fMouseOpenAdjust * 0.5 < 0)) {
//        m_ex_mouthClose.confidence = 1.0f;
//        m_ex_mouthClose.trigger = true;
//    } else {
//        m_ex_mouthClose.trigger = false;
//        m_ex_mouthClose.confidence = 1.0f;
//    }
//    
//    //嘴角
//    float t_mouthCornerValue = 0;
//    if (m_personPitch > -15 && m_personPitch < -5) {
//        t_mouthCornerValue = 0;
//    } else if (m_personPitch <= -15) {
//        t_mouthCornerValue = 20 * sinf(-m_personPitch * TORADIAN);
//    } else if (m_personPitch >= -5) {
//        t_mouthCornerValue = 100 * sinf(-m_personPitch * TORADIAN);
//    }
//    
//    float t_value1 = t_angle_mouth_leftcorner_upperliptop_upperlipbottom - 90 * m_fLipsAdjust;
//    if (t_value1 > t_mouthCornerValue) {
//        float confidence = min_(fabsf(t_value1/10.0f), 1.0f);
//        m_ex_leftMouthRaiseCorner.confidence = confidence;
//        m_ex_leftMouthRaiseCorner.trigger = true;
//    } else {
//        m_ex_leftMouthRaiseCorner.confidence = 1.0f;
//        m_ex_leftMouthRaiseCorner.trigger = false;
//    }
//    float t_value2 = t_angle_mouth_rightcorner_upperliptop_upperlipbottom - 90 * m_fLipsAdjust;
//    if (t_value2 > t_mouthCornerValue) {
//        float confidence = min_(fabsf(t_value2/10.0f), 1.0f);
//        m_ex_rightMouthRaiseCorner.confidence = confidence;
//        m_ex_rightMouthRaiseCorner.trigger = true;
//    } else {
//        m_ex_rightMouthRaiseCorner.confidence = 1.0f;
//        m_ex_rightMouthRaiseCorner.trigger = false;
//    }
//    
//    //扭头
//    m_ex_leftTurnHead.trigger = false;
//    m_ex_rightTurnHead.trigger = false;
//    float t_personYaw = fabsf(m_personYaw);
//    if (t_personYaw > m_fLookLeftAngle && (m_personYaw < 0)) {
//        m_ex_leftTurnHead.trigger = true;
//        float confidence = min_(t_personYaw/45.0f, 1.0f);
//        m_ex_leftTurnHead.confidence = confidence;
//    } else if (t_personYaw > m_fLookRightAngle && (m_personYaw > 0)) {
//        float confidence = min_(t_personYaw/45.0f, 1.0f);
//        m_ex_rightTurnHead.confidence = confidence;
//        m_ex_rightTurnHead.trigger = true;
//    }
//    
//    //
//    //抬头
//    m_ex_raiseHead.trigger = false;
//    m_ex_bowHead.trigger = false;
//    float t_personPitch = fabsf(m_personPitch);
//    if (t_personPitch > m_fLookUpAnlge && (m_personPitch > 0)) {
//        m_ex_raiseHead.trigger = true;
//        float confidence = min_(t_personPitch/45.0f, 1.0f);
//        m_ex_raiseHead.confidence = confidence;
//        m_ex_raiseHead.expValue = m_personPitch;
//    } else if (t_personPitch > m_fLookDownAngle && (m_personPitch < 0)) {
//        m_ex_bowHead.trigger = true;
//        float confidence = min_(t_personPitch/45.0f, 1.0f);
//        m_ex_bowHead.confidence = confidence;
//        m_ex_bowHead.expValue = m_personPitch;
//    }
//    //
//    //嘟嘴
//    m_ex_touthMouth.trigger = false;
//    m_ex_touthMouth.confidence = 1.0f;
//    float t_standard_angle_leftcorner_upperliptop_rightcorner =
//    atanf((tanf(m_standard_angle_leftcorner_upperliptop_rightcorner * 0.5 * TORADIAN) *
//           (sqrtf(1.0 - sinf(fabsf(m_personYaw) * TORADIAN))))) *
//    TOANGLE * 2;
//    float t_standard_angle_leftcorner_lowerlipbottom_rightcorner =
//    atanf((tanf(m_standard_angle_leftcorner_lowerlipbottom_rightcorner * 0.5 * TORADIAN) *
//           (sqrtf(1.0 - sinf(fabsf(m_personYaw) * TORADIAN))))) *
//    TOANGLE * 2;
//    if ((m_ex_mouthOpen.trigger == false) && (t_mouthWidth <= m_mouthStandardWidth) && ((t_mouthHigh - t_len_mouth_lowerlip_high) < 0)) {
//        if ((t_standard_angle_leftcorner_lowerlipbottom_rightcorner - t_angle_mouth_leftcorner_lowerlipbottom_rightcorner >
//             10 * m_fPoutAdjust) &&
//            ((t_standard_angle_leftcorner_upperliptop_rightcorner - t_angle_mouth_leftcorner_upperliptop_rightcorner) >
//             10 * m_fPoutAdjust)) {
//                m_ex_touthMouth.trigger = true;
//                float t_lower_confidence = min_((1.0f - fabsf(t_angle_mouth_leftcorner_lowerlipbottom_rightcorner/t_standard_angle_leftcorner_lowerlipbottom_rightcorner))*3.3f, 1.0f);
//                float t_upper_confidence = min_((1.0f - fabsf(t_angle_mouth_leftcorner_upperliptop_rightcorner/t_standard_angle_leftcorner_upperliptop_rightcorner))*3.3f, 1.0f);
//                float t_confidence = (t_lower_confidence+t_upper_confidence)*0.5;
//                m_ex_touthMouth.confidence = t_confidence;
//            }
//    }
//    
//    //组合表情
//    m_ex_lefteyeopen_righteyeclose.trigger = false;
//    m_ex_lefteyeopen_righteyeclose.confidence = 1.0f;
//    if ((m_ex_leftEyeOpen.trigger == true) && (m_ex_rightEyeOpen.trigger == false)) {
//        //左睁眼，右闭眼
//        m_ex_lefteyeopen_righteyeclose.confidence = (m_ex_leftEyeOpen.confidence + m_ex_rightEyeOpen.confidence)*0.5;
//        m_ex_lefteyeopen_righteyeclose.trigger = true;
//    }
//    
//    m_ex_lefteyeclose_righteyeopen.trigger = false;
//    m_ex_lefteyeclose_righteyeopen.confidence = 1.0f;
//    if ((m_ex_leftEyeOpen.trigger == false) && (m_ex_rightEyeOpen.trigger == true)) {
//        //左闭眼，右睁眼
//        m_ex_lefteyeclose_righteyeopen.confidence = (m_ex_leftEyeOpen.confidence + m_ex_rightEyeOpen.confidence)*0.5;
//        m_ex_lefteyeclose_righteyeopen.trigger = true;
//    }
//    
//    m_ex_lefteyeclose_righteyeclose.trigger = false;
//    m_ex_lefteyeclose_righteyeclose.confidence = 1.0f;
//    if ((m_ex_leftEyeOpen.trigger == false) && (m_ex_rightEyeOpen.trigger == false)) {
//        //左闭眼，右闭眼
//        m_ex_lefteyeclose_righteyeclose.confidence = (m_ex_leftEyeOpen.confidence + m_ex_rightEyeOpen.confidence)*0.5;
//        m_ex_lefteyeclose_righteyeclose.trigger = true;
//    }
//    
//    m_ex_mouthopen_headturnleft.trigger = false;
//    m_ex_mouthopen_headturnleft.confidence = 1.0f;
//    if (m_ex_mouthOpen.trigger == true && m_ex_leftTurnHead.trigger == true) {
//        //左扭头张嘴
//        m_ex_mouthopen_headturnleft.confidence = (m_ex_mouthOpen.confidence + m_ex_leftTurnHead.confidence)*0.5;
//        m_ex_mouthopen_headturnleft.trigger = true;
//    }
//    
//    m_ex_mouthopen_headturnright.trigger = false;
//    m_ex_mouthopen_headturnright.confidence = 1.0f;
//    if (m_ex_mouthOpen.trigger == true && m_ex_rightTurnHead.trigger == true) {
//        //右扭头张嘴
//        m_ex_mouthopen_headturnright.confidence = (m_ex_mouthOpen.confidence + m_ex_rightTurnHead.confidence)*0.5;
//        m_ex_mouthopen_headturnright.trigger = true;
//    }
//    
//    m_ex_mouthRaiseCorner.trigger = false;
//    if (m_ex_leftMouthRaiseCorner.trigger == true && m_ex_rightMouthRaiseCorner.trigger == true) {
//        m_ex_mouthRaiseCorner.confidence = (m_ex_leftMouthRaiseCorner.confidence + m_ex_rightMouthRaiseCorner.confidence)*0.5;
//        m_ex_mouthRaiseCorner.trigger = true;
//        //        printf("m_ex_mouthRaiseCorner.confidence:%f\n",m_ex_mouthRaiseCorner.confidence);
//    }
//    
//    m_ex_mouthtouth_headturnleft.trigger = false;
//    m_ex_mouthtouth_headturnleft.confidence = 1.0f;
//    if (m_ex_touthMouth.trigger == true && m_ex_leftTurnHead.trigger == true) {
//        //左嘟嘴
//        m_ex_mouthtouth_headturnleft.confidence = (m_ex_touthMouth.confidence + m_ex_leftTurnHead.confidence)*0.5;
//        m_ex_mouthtouth_headturnleft.trigger = true;
//    }
//    
//    m_ex_mouthtouth_headturnright.trigger = false;
//    m_ex_mouthtouth_headturnright.confidence = 1.0f;
//    if (m_ex_touthMouth.trigger == true && m_ex_rightTurnHead.trigger == true) {
//        //右嘟嘴
//        m_ex_mouthtouth_headturnright.confidence = (m_ex_touthMouth.confidence + m_ex_rightTurnHead.confidence)*0.5;
//        m_ex_mouthtouth_headturnright.trigger = true;
//    }
//}
//
//void MaPersonExpressionExt::saveStandardPt(float *_pt, int _ptnum, MaRect *_rc, float _roll, float _yaw, float _pitch) {
//    m_haveStanderFace = true;
//    memset(m_standardPt, 0, sizeof(m_standardPt));
//    memcpy(m_standardPt, _pt, _ptnum * sizeof(float));
//    m_standardRect.top = _rc->top;
//    m_standardRect.left = _rc->left;
//    m_standardRect.bottom = _rc->bottom;
//    m_standardRect.right = _rc->right;
//    m_standardRoll = _roll;
//    m_standardYaw = _yaw;
//    m_standardPitch = _pitch;
//    //FM_LEFT_EYE_TOP
//    int t_index_lefteye_top = _getRealIndex(FM_LEFT_EYE_TOP);
//    float t_pt_lefteye_top_x = m_standardPt[2 * t_index_lefteye_top];
//    float t_pt_lefteye_top_y = m_standardPt[2 * t_index_lefteye_top + 1];
//    //FM_LEFT_EYE_BOTTOM
//    int t_index_lefteye_bottom = _getRealIndex(FM_LEFT_EYE_BOTTOM);
//    float t_pt_lefteye_bottom_x = m_standardPt[2 * t_index_lefteye_bottom];
//    float t_pt_lefteye_bottom_y = m_standardPt[2 * t_index_lefteye_bottom + 1];
//    m_leftStandardEyeHigh = MaMathHelper::vec2Length(t_pt_lefteye_top_x, t_pt_lefteye_top_y, t_pt_lefteye_bottom_x,
//                                                     t_pt_lefteye_bottom_y);
//    //FM_RIGHT_EYE_TOP
//    int t_index_righteye_top = _getRealIndex(FM_RIGHT_EYE_TOP);
//    float t_pt_righteye_top_x = m_standardPt[2 * t_index_righteye_top];
//    float t_pt_righteye_top_y = m_standardPt[2 * t_index_righteye_top + 1];
//    //FM_RIGHT_EYE_BOTTOM
//    int t_index_righteye_bottom = _getRealIndex(FM_RIGHT_EYE_BOTTOM);
//    float t_pt_righteye_bottom_x = m_standardPt[2 * t_index_righteye_bottom];
//    float t_pt_righteye_bottom_y = m_standardPt[2 * t_index_righteye_bottom + 1];
//    m_rightStandardEyeHigh = MaMathHelper::vec2Length(t_pt_righteye_top_x, t_pt_righteye_top_y, t_pt_righteye_bottom_x,
//                                                      t_pt_righteye_bottom_y);
//    //FM_MOUTH_UPPER_LIP_BOTTOM
//    int t_index_mouth_upperlip_bottom = _getRealIndex(FM_MOUTH_UPPER_LIP_BOTTOM);
//    float t_pt_mouth_upperlip_bottom_x = m_standardPt[2 * t_index_mouth_upperlip_bottom];
//    float t_pt_mouth_upperlip_bottom_y = m_standardPt[2 * t_index_mouth_upperlip_bottom + 1];
//    //FM_MOUTH_LOWER_LIP_TOP
//    int t_index_mouth_lowerlip_top = _getRealIndex(FM_MOUTH_LOWER_LIP_TOP);
//    float t_pt_mouth_lowerlip_top_x = m_standardPt[2 * t_index_mouth_lowerlip_top];
//    float t_pt_mouth_lowerlip_top_y = m_standardPt[2 * t_index_mouth_lowerlip_top + 1];
//    m_mouthStandardHigh = MaMathHelper::vec2Length(t_pt_mouth_upperlip_bottom_x, t_pt_mouth_upperlip_bottom_y,
//                                                   t_pt_mouth_lowerlip_top_x, t_pt_mouth_lowerlip_top_y);
//    //FM_MOUTH_LEFT_CORNER
//    int t_index_mouth_leftcorner = _getRealIndex(FM_MOUTH_LEFT_CORNER);
//    float t_pt_mouth_leftcorner_x = m_standardPt[2 * t_index_mouth_leftcorner];
//    float t_pt_mouth_leftcorner_y = m_standardPt[2 * t_index_mouth_leftcorner + 1];
//    //FM_MOUTH_RIGHT_CORNER
//    int t_index_mouth_rightcorner = _getRealIndex(FM_MOUTH_RIGHT_CORNER);
//    float t_pt_mouth_rightcorner_x = m_standardPt[2 * t_index_mouth_rightcorner];
//    float t_pt_mouth_rightcorner_y = m_standardPt[2 * t_index_mouth_rightcorner + 1];
//    m_mouthStandardWidth = MaMathHelper::vec2Length(t_pt_mouth_leftcorner_x, t_pt_mouth_leftcorner_y, t_pt_mouth_rightcorner_x,
//                                                    t_pt_mouth_rightcorner_y);
//    //FM_MOUTH_UPPER_LIP_TOP
//    int t_index_mouth_upperlip_top = _getRealIndex(FM_MOUTH_UPPER_LIP_TOP);
//    float t_pt_mouth_upperlip_top_x = m_standardPt[2 * t_index_mouth_upperlip_top];
//    float t_pt_mouth_upperlip_top_y = m_standardPt[2 * t_index_mouth_upperlip_top + 1];
//    m_upMouthTipStandardHigh = MaMathHelper::vec2Length(t_pt_mouth_upperlip_top_x, t_pt_mouth_upperlip_top_y,
//                                                        t_pt_mouth_upperlip_bottom_x, t_pt_mouth_upperlip_bottom_y);
//    //FM_MOUTH_LOWER_LIP_BOTTOM
//    int t_index_mouth_lowerlip_bottom = _getRealIndex(FM_MOUTH_LOWER_LIP_BOTTOM);
//    float t_pt_mouth_lowerlip_bottom_x = m_standardPt[2 * t_index_mouth_lowerlip_bottom];
//    float t_pt_mouth_lowerlip_bottom_y = m_standardPt[2 * t_index_mouth_lowerlip_bottom + 1];
//    m_bottomMouthTipStandardHigh = MaMathHelper::vec2Length(t_pt_mouth_lowerlip_bottom_x, t_pt_mouth_lowerlip_bottom_y,
//                                                            t_pt_mouth_lowerlip_top_x, t_pt_mouth_lowerlip_top_y);
//    
//    float t_vec_mouth_leftcorner_upperliptop_x = t_pt_mouth_leftcorner_x - t_pt_mouth_upperlip_top_x;
//    float t_vec_mouth_leftcorner_upperliptop_y = t_pt_mouth_leftcorner_y - t_pt_mouth_upperlip_top_y;
//    float t_vec_mouth_rightcorner_upperliptop_x = t_pt_mouth_rightcorner_x - t_pt_mouth_upperlip_top_x;
//    float t_vec_mouth_rightcorner_upperliptop_y = t_pt_mouth_rightcorner_y - t_pt_mouth_upperlip_top_y;
//    float t_vec_mouth_up = (t_vec_mouth_leftcorner_upperliptop_x * t_vec_mouth_rightcorner_upperliptop_x +
//                            t_vec_mouth_leftcorner_upperliptop_y * t_vec_mouth_rightcorner_upperliptop_y);
//    float t_len_mouth_leftcorner_upperliptop = MaMathHelper::vec2Length(t_pt_mouth_leftcorner_x, t_pt_mouth_leftcorner_y,
//                                                                        t_pt_mouth_upperlip_top_x, t_pt_mouth_upperlip_top_y);
//    float t_len_mouth_rightcorner_upperliptop = MaMathHelper::vec2Length(t_pt_mouth_rightcorner_x, t_pt_mouth_rightcorner_y,
//                                                                         t_pt_mouth_upperlip_top_x, t_pt_mouth_upperlip_top_y);
//    m_standard_angle_leftcorner_upperliptop_rightcorner =
//    acosf(t_vec_mouth_up / (t_len_mouth_leftcorner_upperliptop * t_len_mouth_rightcorner_upperliptop)) * TOANGLE;
//    
//    float t_vec_mouth_leftcorner_lowerlipbottom_x = t_pt_mouth_leftcorner_x - t_pt_mouth_lowerlip_bottom_x;
//    float t_vec_mouth_leftcorner_lowerlipbottom_y = t_pt_mouth_leftcorner_y - t_pt_mouth_lowerlip_bottom_y;
//    float t_vec_mouth_rightcorner_lowerlipbottom_x = t_pt_mouth_rightcorner_x - t_pt_mouth_lowerlip_bottom_x;
//    float t_vec_mouth_rightcorner_lowerlipbottom_y = t_pt_mouth_rightcorner_y - t_pt_mouth_lowerlip_bottom_y;
//    float t_vec_mouth_bottom = (t_vec_mouth_leftcorner_lowerlipbottom_x * t_vec_mouth_rightcorner_lowerlipbottom_x +
//                                t_vec_mouth_leftcorner_lowerlipbottom_y * t_vec_mouth_rightcorner_lowerlipbottom_y);
//    float t_len_leftcorner_lowerlipbottom = MaMathHelper::vec2Length(t_pt_mouth_leftcorner_x, t_pt_mouth_leftcorner_y,
//                                                                     t_pt_mouth_lowerlip_bottom_x, t_pt_mouth_lowerlip_bottom_y);
//    float t_len_rightcorner_lowerlipbottom = MaMathHelper::vec2Length(t_pt_mouth_rightcorner_x, t_pt_mouth_rightcorner_y,
//                                                                      t_pt_mouth_lowerlip_bottom_x, t_pt_mouth_lowerlip_bottom_y);
//    m_standard_angle_leftcorner_lowerlipbottom_rightcorner =
//    acosf(t_vec_mouth_bottom / (t_len_leftcorner_lowerlipbottom * t_len_rightcorner_lowerlipbottom)) * TOANGLE;
//    
//    
//}
//
//void MaPersonExpressionExt::resetStandarPt() {
//    m_haveStanderFace = false;
//    memset(m_standardPt, 0, sizeof(m_standardPt));
//    m_standardRect.top = 0;
//    m_standardRect.left = 0;
//    m_standardRect.bottom = 0;
//    m_standardRect.right = 0;
//    m_standardRoll = 0;
//    m_standardYaw = 0;
//    m_standardPitch = 0;
//    m_isFirstTime = false;
//}
//
//void MaPersonExpressionExt::_sendFaceExpressionEvents() {
//    //////////////////////
//    //发送消息
//    if (m_ex_leftEyeOpen.trigger) {
//        sendEvent("lefteyeopen", EVN_T_LEYE_OPEN, m_ex_leftEyeOpen.confidence);
//        //        printf("m_ex_leftEyeOpen.confidence:%f\n",m_ex_leftEyeOpen.confidence);
//    } else {
//        sendEvent("lefteyeclose", EVN_T_LEYE_CLOSE, m_ex_leftEyeOpen.confidence);
//        //        printf("m_ex_leftEyeOpen.confidence:%f\n",m_ex_leftEyeOpen.confidence);
//    }
//    //
//    if (m_ex_rightEyeOpen.trigger) {
//        sendEvent("righteyeopen", EVN_T_REYE_OPEN, m_ex_rightEyeOpen.confidence);
//        //        printf("m_ex_rightEyeOpen.confidence:%f\n",m_ex_rightEyeOpen.confidence);
//    } else {
//        sendEvent("righteyeclose", EVN_T_REYE_CLOSE, m_ex_rightEyeOpen.confidence);
//        //        printf("m_ex_rightEyeOpen.confidence:%f\n",m_ex_rightEyeOpen.confidence);
//    }
//    
//    if (m_ex_lastLeftEyeOpen.trigger != m_ex_leftEyeOpen.trigger) {
//        this->sendEvent("eyeblink", EVN_T_LEYE_BLINK, (m_ex_lastLeftEyeOpen.confidence + m_ex_leftEyeOpen.confidence)*0.5);
//    }
//    if (m_ex_lastRightEyeOpen.trigger != m_ex_rightEyeOpen.trigger) {
//        this->sendEvent("eyeblink", EVN_T_REYE_BLINK, (m_ex_lastRightEyeOpen.confidence + m_ex_rightEyeOpen.confidence)*0.5);
//    }
//    
//    if (m_ex_mouthOpen.trigger) {
//        sendEvent("mouthopen", EVN_T_MOUSE_OPEN, m_ex_mouthOpen.confidence,m_ex_mouthOpen.expValue);
//    }
//    if (m_ex_mouthClose.trigger) {
//        //        printf("闭嘴\n");
//        sendEvent("mouthclose", EVN_T_MOUSE_CLOSE, m_ex_mouthClose.confidence);
//    }
//    
//    if (m_ex_mouthRaiseCorner.trigger) {
//        //        printf("m_ex_mouthRaiseCorner.confidence:%f\n",m_ex_mouthRaiseCorner.confidence);
//        sendEvent("raisemouthcorner", EVN_T_MOUSE_RAISE_CORNER, m_ex_mouthRaiseCorner.confidence);
//    }
//    
//    //暂时没有，无法判别
//    //    if (m_rightMouthCorner) {
//    //        sendEvent("rightMouthCorner", EVN_T_MOUSE_RIGHT_CORNER);
//    //    }
//    
//    if (m_ex_leftTurnHead.trigger) {
//        //        printf("m_ex_leftTurnHead.confidence:%f\n",m_ex_leftTurnHead.confidence);
//        sendEvent("headturnleft", EVN_T_LEFT_TUREN_HEAD, m_ex_leftTurnHead.confidence);
//    }
//    if (m_ex_rightTurnHead.trigger) {
//        //        printf("m_ex_rightTurnHead.confidence:%f\n",m_ex_rightTurnHead.confidence);
//        sendEvent("headturnright", EVN_T_RIGHT_TUREN_HEAD, m_ex_rightTurnHead.confidence);
//    }
//    if (m_ex_leftTurnHead.trigger == false && m_ex_rightTurnHead.trigger == false) {
//        sendEvent("headnoneturn", EVN_T_NONE_TUREN_HEAD, 1.0);
//    }
//    if (m_ex_raiseHead.trigger) {
//        //        printf("m_ex_raiseHead.confidence:%f\n",m_ex_raiseHead.confidence);
//        sendEvent("headraise", EVN_T_RAISE_HEAD, m_ex_raiseHead.confidence,m_ex_raiseHead.expValue);
//    }
//    if (m_ex_bowHead.trigger) {
//        sendEvent("headbow", EVN_T_BOW_HEAD, m_ex_bowHead.confidence , m_ex_bowHead.expValue);
//    }
//    if (m_ex_raiseHead.trigger == false && m_ex_bowHead.trigger == false) {
//        sendEvent("headnoneraisebow", EVN_T_NONE_RAISE_BOW_HEAD, 1.0);
//    }
//    if (m_ex_touthMouth.trigger) {
//        //        printf("m_ex_touthMouth.confidence:%f\n",m_ex_touthMouth.confidence);
//        sendEvent("mouthtouth", EVN_T_MOUSE_TOUTH, m_ex_touthMouth.confidence);
//    }
//    //组合表情
//    if (m_ex_lefteyeopen_righteyeclose.trigger) {
//        //左睁眼，右闭眼
//        //        printf("m_ex_lefteyeopen_righteyeclose.confidence:%f\n",m_ex_lefteyeopen_righteyeclose.confidence);
//        sendEvent("lefteyeopen_righteyeclose", EVN_T_C_LEFTEYEOPEN_RIGHTEYECLOSE, m_ex_lefteyeopen_righteyeclose.confidence);
//    }
//    if (m_ex_lefteyeclose_righteyeopen.trigger) {
//        //左闭眼，右睁眼
//        //        printf("m_ex_lefteyeclose_righteyeopen.confidence:%f\n",m_ex_lefteyeclose_righteyeopen.confidence);
//        sendEvent("lefteyeclose_righteyeopen", EVN_T_C_LEFTEYECLOSE_RIGHTEYEOPEN, m_ex_lefteyeclose_righteyeopen.confidence);
//    }
//    if (m_ex_lefteyeclose_righteyeclose.trigger) {
//        //左闭眼，右闭眼
//        sendEvent("lefteyeclose_righteyeclose", EVN_T_C_LEFTEYECLOSE_RIGHTEYECLOSE, m_ex_lefteyeclose_righteyeclose.confidence);
//    }
//    if (m_ex_mouthopen_headturnleft.trigger) {
//        //左扭头张嘴
//        //        printf("m_ex_mouthopen_headturnleft.confidence:%f\n",m_ex_mouthopen_headturnleft.confidence);
//        sendEvent("mouthopen_headturnleft", EVN_T_C_MOUTHOPEN_TURNLEFT, m_ex_mouthopen_headturnleft.confidence);
//    }
//    if (m_ex_mouthopen_headturnright.trigger) {
//        //右扭头张嘴
//        //        printf("m_ex_mouthopen_headturnleft.confidence:%f\n",m_ex_mouthopen_headturnleft.confidence);
//        sendEvent("mouthopen_headturnright", EVN_T_C_MOUTHOPEN_TURNRIGHT, m_ex_mouthopen_headturnright.confidence);
//    }
//    if (m_ex_mouthtouth_headturnleft.trigger) {
//        //左嘟嘴
//        //        printf("m_ex_mouthtouth_headturnleft.confidence:%f\n",m_ex_mouthtouth_headturnleft.confidence);
//        sendEvent("mouthtouth_headturnleft", EVN_T_C_MOUTHTOUTH_TURNLEFT, m_ex_mouthtouth_headturnleft.confidence);
//    }
//    if (m_ex_mouthtouth_headturnright.trigger) {
//        //右嘟嘴
//        //        printf("m_ex_mouthtouth_headturnright.confidence:%f\n",m_ex_mouthtouth_headturnright.confidence);
//        sendEvent("mouthtouth_headturnright", EVN_T_C_MOUTHTOUTH_TURNRIGHT, m_ex_mouthtouth_headturnright.confidence);
//    }
}

void SVPersonTracker::_correctStandardface_st(){
    
}
///
void SVPersonTracker::_trackExpression_as(){
    
}

void SVPersonTracker::_correctStandardface_as(){
    
}
///
void SVPersonTracker::_trackExpression_fp(){
    
}

void SVPersonTracker::_correctStandardface_fp(){
    
}
///
