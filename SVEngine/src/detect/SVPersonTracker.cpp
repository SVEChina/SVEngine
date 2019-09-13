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
    m_enableExpression = true;
}

SVPersonTracker::~SVPersonTracker() {

}

f32 SVPersonTracker::getEyeDis() {
    FVec3 t_dis = m_leftEyePos - m_rightEyePos;
    f32 t_eyedis = t_dis.length();
    //t_eyedis = t_eyedis/cos(m_roll*DEG2RAD);
    t_eyedis = t_eyedis/cos(-m_yaw*DEG2RAD);
    //t_eyedis = t_eyedis/cos(m_pitch*DEG2RAD);
    return t_eyedis;
}

f32 SVPersonTracker::getEyeStdScale(){
    f32 t_adapt_scale = mApp->getConfig()->getDesignAdaptScale();
    f32 t_scale = m_eyeDistance/ t_adapt_scale / STANDARD_EYE_STD;
    return t_scale;
}

f32 SVPersonTracker::getNoiseStdScale(){
    f32 t_adapt_scale = mApp->getConfig()->getDesignAdaptScale();
    f32 t_scale = m_noiseDistance/ t_adapt_scale / STANDARD_NOISE_STD;
    return t_scale;
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
    //精细表情识别
    if (m_enableExpression) {
        _trackExpression_st();
    }
    //更新包围盒
    m_bound.clear();
    for(s32 i=0;i<106;i++) {
        f32 t_x = m_pFacedata[2*i];
        f32 t_y = m_pFacedata[2*i+1];
        m_bound.expand(FVec3(t_x,t_y,0.0f));
    }
}

void SVPersonTracker::setEnableExpression(bool _enable){
    m_enableExpression = _enable;
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

    //嘟嘴
    FVec2 t_mouth_upperliptop2leftcorner = t_pt_mouth_leftcorner - t_pt_mouth_upperlip_top;
    FVec2 t_mouth_upperliptop2rightcorner = t_pt_mouth_rightcorner - t_pt_mouth_upperlip_top;
    FVec2 t_mouth_upperliptop2upperlipbottom = t_pt_mouth_upperlip_bottom - t_pt_mouth_upperlip_top;
 
    f32 t_mouth_angle_leftcorner_upperliptop_rightcorner = acosf((dot(t_mouth_upperliptop2leftcorner,t_mouth_upperliptop2rightcorner) / (t_mouth_upperliptop2leftcorner.length() * t_mouth_upperliptop2rightcorner.length()))) * RADTODEG;
    
    f32 t_mouth_angle_leftcorner_upperliptop_upperlipbottom =
    acosf(dot(t_mouth_upperliptop2leftcorner, t_mouth_upperliptop2upperlipbottom) / (t_mouth_upperliptop2leftcorner.length() * t_mouth_upperliptop2upperlipbottom.length())) * RADTODEG;
    
    f32 t_mouth_angle_rightcorner_upperliptop_upperlipbottom =
    acosf(dot(t_mouth_upperliptop2rightcorner, t_mouth_upperliptop2upperlipbottom) / (t_mouth_upperliptop2rightcorner.length() * t_mouth_upperliptop2upperlipbottom.length())) * RADTODEG;

    //
    FVec2 t_mouth_lowerlipbottom2leftcorner = t_pt_mouth_leftcorner - t_pt_mouth_lowerlip_bottom;
    FVec2 t_mouth_lowerlipbottom2rightcorner = t_pt_mouth_rightcorner - t_pt_mouth_lowerlip_bottom;
    f32 t_mouth_angle_leftcorner_lowerlipbottom_rightcorner = acosf(dot(t_mouth_lowerlipbottom2leftcorner, t_mouth_lowerlipbottom2rightcorner)/(t_mouth_lowerlipbottom2leftcorner.length()*t_mouth_lowerlipbottom2rightcorner.length()))*RADTODEG;

    //设置默认标准标记参数
    if (true) {
        m_standard_lefteye_high = max(t_leftEyeWidth * 0.35, 24.0);
        m_standard_righteye_high = max(t_rightEyeWidth * 0.35, 24.0);
        m_standard_mouth_angle_leftcorner_upperliptop_rightcorner = 155;
        m_standard_mouth_angle_leftcorner_lowerlipbottom_rightcorner = 130;
        
    }
    m_standard_mouth_width = t_mouthWidth;
   
    if (t_leftEyeHigh < t_leftEyeWidth*0.15f) {
       //左眼闭了
        _sendExpressionEvent(EVN_T_EXPRESSION_LEFTEYECLOSE);
    }
    
    if (t_rightEyeHigh < t_rightEyeWidth*0.15f) {
        //右眼闭了
        _sendExpressionEvent(EVN_T_EXPRESSION_RIGHTEYECLOSE);
    }

    
    if (t_mouthHigh - m_standard_mouth_width * 0.3f > 0) {
        //张嘴了
        _sendExpressionEvent(EVN_T_EXPRESSION_MOUTHOPEN);
    }else{
        _sendExpressionEvent(EVN_T_EXPRESSION_MOUTHCLOSE);
    }
    
    //扭头
    f32 t_yaw = fabsf(m_yaw);
    if (t_yaw > 15.0f) {
        if (m_yaw > 0) {
            //向左扭头
            _sendExpressionEvent(EVN_T_EXPRESSION_HEADTURNLEFT);
        }else{
            //向右扭头
            _sendExpressionEvent(EVN_T_EXPRESSION_HEADTURNRIGHT);
        }
    }
    
    //抬头
    float t_pitch = fabsf(m_pitch);
    if (t_pitch > 12.0f) {
        if (m_pitch < 0) {
            //抬头
            _sendExpressionEvent(EVN_T_EXPRESSION_HEADRAISE);
        }else{
            //低头
            _sendExpressionEvent(EVN_T_EXPRESSION_HEADBOW);
        }
    }
    
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

void SVPersonTracker::_sendExpressionEvent(SV_EVENT_TYPE _type){
    SVEventPersonExpressionPtr t_event = MakeSharedPtr<SVEventPersonExpression>();
    t_event->eventType = _type;
    t_event->m_personID = m_personID;
    mApp->getEventMgr()->pushEvent(t_event);
}
///
