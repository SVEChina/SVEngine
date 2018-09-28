//
//  SVPersonExpressionExt.h
//  SVEngine
//
//  Created by xiaofan Li on 2017/8/29.
//  Copyright © 2017年 xiaofan Li. All rights reserved.
//

#ifndef SVPersonExpressionExt_h
#define SVPersonExpressionExt_h

#include "../base/SVObject.h"
#include "../event/SVEventDef.h"
#include "../base/SVGBase.h"
#include "../base/SVRect.h"
#include "../app/SVInst.h"
enum EXPRESSIONTYPE{
    EX_TYPE_NONE = 0,
    EX_TYPE_LEFTEYEOPEN,
    EX_TYPE_RIGHTEYEOPEN,
    EX_TYPE_LEFTEYECLOSE_RIGHTEYEOPEN,
    EX_TYPE_LEFTEYEOPEN_RIGHTEYECLOSE,
    EX_TYPE_LEFTEYECLOSE_RIGHTEYECLOSE,
    EX_TYPE_MOUTHOPEN,
    EX_TYPE_MOUTHCLOSE,
    EX_TYPE_LEFTTURNHEAD_MOUTHOPEN,
    EX_TYPE_RIGHTTURNHEAD_MOUTHOPEN,
    EX_TYPE_LEFTMOUTHRAISECORNER,
    EX_TYPE_RIGHTMOUTHRAISECORNER,
    EX_TYPE_MOUTHRAISECORNER,
    EX_TYPE_LEFTMOUTHBOWCORNER,
    EX_TYPE_RIGHTMOUTHBOWCORNER,
    EX_TYPE_MOUTHBOWCORNER,
    EX_TYPE_LEFTTURNHEAD,
    EX_TYPE_RIGHTTURNHEAD,
    EX_TYPE_RAISEHEAD,
    EX_TYPE_BOWHEAD,
    EX_TYPE_TOUTHMOUTH,
    EX_TYPE_LEFTTURNHEAD_TOUTHMOUTH,
    EX_TYPE_RIGHTTURNHEAD_TOUTHMOUTH
};
struct EXPRESSION{
    EXPRESSIONTYPE expressionType;
    const char*    expressionName;
    bool           trigger;
    f32          confidence;
    f32          expValue;
};
//

//下巴
#define SV_CONTOUR_CHIN 0
//左边轮廓
#define SV_CONTOUR_LEFT1 1
#define SV_CONTOUR_LEFT2 2
#define SV_CONTOUR_LEFT4 3
#define SV_CONTOUR_LEFT6 4
#define SV_CONTOUR_LEFT8 5
//右边轮廓
#define SV_CONTOUR_RIGHT1 6
#define SV_CONTOUR_RIGHT2 7
#define SV_CONTOUR_RIGHT4 8
#define SV_CONTOUR_RIGHT6 9
#define SV_CONTOUR_RIGHT8 10
//左眼
#define SV_LEFT_EYE_BOTTOM 11
#define SV_LEFT_EYE_LEFT_CORNER 12
#define SV_LEFT_EYE_PUPIL 13
#define SV_LEFT_EYE_RIGHT_CORNER 14
#define SV_LEFT_EYE_TOP 15
//左眉
#define SV_LEFT_EYEBROW_LEFT_CORNER 16
#define SV_LEFT_EYEBROW_LOWER_MIDDLE 17
#define SV_LEFT_EYEBROW_RIGHT_CORNER 18
#define SV_LEFT_EYEBROW_UPPER_MIDDLE 19
//嘴
#define SV_MOUTH_LEFT_CORNER 20
#define SV_MOUTH_LOWER_LIP_BOTTOM 21
#define SV_MOUTH_LOWER_LIP_TOP 22
#define SV_MOUTH_RIGHT_CORNER 23
#define SV_MOUTH_UPPER_LIP_BOTTOM 24
#define SV_MOUTH_UPPER_LIP_TOP 25
//鼻子
#define SV_NOSE_CONTOUR_LOWER_MIDDLE 26
#define SV_NOSE_LEFT 27
#define SV_NOSE_RIGHT 28
#define SV_NOSE_TIP 29
//右眼
#define SV_RIGHT_EYE_BOTTOM 30
#define SV_RIGHT_EYE_LEFT_CORNER 31
#define SV_RIGHT_EYE_PUPIL 32
#define SV_RIGHT_EYE_RIGHT_CORNER 33
#define SV_RIGHT_EYE_TOP 34
//右眉
#define SV_RIGHT_EYEBROW_LEFT_CORNER 35
#define SV_RIGHT_EYEBROW_LOWER_MIDDLE 36
#define SV_RIGHT_EYEBROW_RIGHT_CORNER 37
#define SV_RIGHT_EYEBROW_UPPER_MIDDLE 38

#define SV_EYEBROW_BETWEEN 39
//

namespace sv {
    
    namespace detect{
        
        class SVPersonExpressionExt : public SVGBase{
        public:
            
            SVPersonExpressionExt(SVInst *_app);
            
            ~SVPersonExpressionExt();
            
            void trackExpression(s32 _pt[], s32 _ptnum, SVRect*_rect, f32 _roll, f32 _yaw, f32 _pitch, s32 _personID);
            
            void saveStandardPt(f32 _pt[], s32 _ptnum, SVRect*_rect, f32 _roll, f32 _yaw, f32 _pitch);
            
            void resetStandarPt();
            
            
            void _setLookUpAngle(f32 _angle);
            f32 _getLookUpAngle();
            void _setLookDownAngle(f32 _angle);
            f32 _getLookDownAngle();
            void _setLookLeftAngle(f32 _angle);
            f32 _getLookLeftAngle();
            void _setLookRightAngle(f32 _angle);
            f32 _getLookRightAngle();
            void _setEyeCloseAdjust(f32 _adjust);
            void _setMouthOpenAdjust(f32 _adjust);
            f32 _getMouthOpenAdjust();
            void _setPoutAdjust(f32 _adjust);
            void _setLipsAdjust(f32 _adjust);
            void setDetectExtRate(s32 _rate);
            s32 getDetectExtRate();
            //获取人脸基本表情状态
            bool getLeftEyeOpenState();
            bool getRightEyeOpenState();
            bool getMouthOpenState();
            bool getLeftMouthCornerRaiseState();
            bool getRightMouthCornerRaiseState();
            bool getLeftMouthCornerBowState();
            bool getRightMouthCornerBowState();
            bool getLeftTurnHeadState();
            bool getRightTurnHeadState();
            bool getRaiseHeadState();
            bool getBowHeadState();
            bool getTouthMouthState();
            //
            
            void setEnableDrawTestPt(bool _enable);
            void setEnableSendEvnet(bool _enable);
        private:
            timeval m_lastT;
            bool   m_isFirstTime;
            f32  m_rate;//多少毫秒进行一次识别操作
            s32     m_personID;
            //标准脸参数
            bool m_haveStanderFace;
            SVRect m_standardRect;
            f32 m_standardRoll;
            f32 m_standardYaw;
            f32 m_standardPitch;
            f32 m_leftStandardEyeHigh;
            f32 m_rightStandardEyeHigh;
            f32 m_mouthStandardHigh;
            f32 m_mouthStandardWidth;
            f32 m_upMouthTipStandardHigh;
            f32 m_bottomMouthTipStandardHigh;
            f32 m_standard_angle_leftcorner_upperliptop_rightcorner;
            f32 m_standard_angle_leftcorner_lowerlipbottom_rightcorner;
            //人脸基本参数
            SVRect m_personRect;
            f32 m_personRoll;
            f32 m_personYaw;
            f32 m_personPitch;
            //人脸基本表情
            EXPRESSION m_ex_leftEyeOpen = {EX_TYPE_LEFTEYEOPEN, "lefteyeopen", true, 1.0};
            EXPRESSION m_ex_rightEyeOpen = {EX_TYPE_RIGHTEYEOPEN, "righteyeopen", true, 1.0};
            EXPRESSION m_ex_lastLeftEyeOpen = {EX_TYPE_LEFTEYEOPEN, "lefteyeopen", true, 1.0};
            EXPRESSION m_ex_lastRightEyeOpen = {EX_TYPE_RIGHTEYEOPEN, "righteyeopen", true, 1.0};
            EXPRESSION m_ex_lefteyeopen_righteyeclose = {EX_TYPE_LEFTEYEOPEN_RIGHTEYECLOSE, "lefteyeopen_righteyeclose", false, 1.0};
            EXPRESSION m_ex_lefteyeclose_righteyeopen = {EX_TYPE_LEFTEYECLOSE_RIGHTEYEOPEN, "lefteyeclose_righteyeopen", false, 1.0};
            EXPRESSION m_ex_lefteyeclose_righteyeclose = {EX_TYPE_LEFTEYECLOSE_RIGHTEYECLOSE, "lefteyeclose_righteyeclose", false, 1.0};
            EXPRESSION m_ex_mouthOpen = {EX_TYPE_MOUTHOPEN, "mouthopen", false, 1.0};
            EXPRESSION m_ex_lastMouthOpen = {EX_TYPE_MOUTHOPEN, "mouthopen", false, 1.0};
            EXPRESSION m_ex_mouthClose = {EX_TYPE_MOUTHCLOSE, "mouthclose", false, 1.0};
            EXPRESSION m_ex_lastMouthClose = {EX_TYPE_MOUTHCLOSE, "mouthclose", false, 1.0};
            EXPRESSION m_ex_mouthopen_headturnleft = {EX_TYPE_LEFTTURNHEAD_MOUTHOPEN, "mouthopen_headturnleft", false, 1.0};
            EXPRESSION m_ex_mouthopen_headturnright = {EX_TYPE_RIGHTTURNHEAD_MOUTHOPEN, "mouthopen_headturnright", false, 1.0};
            EXPRESSION m_ex_leftMouthRaiseCorner = {EX_TYPE_LEFTMOUTHRAISECORNER, "leftraisemouthcorner", false, 1.0};
            EXPRESSION m_ex_rightMouthRaiseCorner = {EX_TYPE_RIGHTMOUTHRAISECORNER, "rightraisemouthcorner", false, 1.0};
            EXPRESSION m_ex_mouthRaiseCorner = {EX_TYPE_RIGHTMOUTHRAISECORNER, "raisemouthcorner", false, 1.0};
            EXPRESSION m_ex_leftMouthBowCorner = {EX_TYPE_LEFTMOUTHBOWCORNER, "leftbowmouthcorner", false, 1.0};
            EXPRESSION m_ex_rightMouthBowCorner = {EX_TYPE_RIGHTMOUTHBOWCORNER, "rightbowmouthcorner", false, 1.0};
            EXPRESSION m_ex_mouthBowCorner = {EX_TYPE_MOUTHBOWCORNER, "bowmouthcorner", false, 1.0};
            EXPRESSION m_ex_leftTurnHead = {EX_TYPE_LEFTTURNHEAD, "headturnleft", false, 1.0};
            EXPRESSION m_ex_rightTurnHead = {EX_TYPE_RIGHTTURNHEAD, "headturnright", false, 1.0};
            EXPRESSION m_ex_raiseHead = {EX_TYPE_RAISEHEAD, "headraise", false, 1.0};
            EXPRESSION m_ex_bowHead = {EX_TYPE_BOWHEAD, "headbow", false, 1.0};
            EXPRESSION m_ex_touthMouth = {EX_TYPE_TOUTHMOUTH, "mouthtouth", false, 1.0};
            EXPRESSION m_ex_mouthtouth_headturnleft = {EX_TYPE_LEFTTURNHEAD_TOUTHMOUTH, "mouthtouth_headturnleft", false, 1.0};
            EXPRESSION m_ex_mouthtouth_headturnright = {EX_TYPE_RIGHTTURNHEAD_TOUTHMOUTH, "mouthtouth_headturnright", false, 1.0};
            
            //外部设置相关参数
            f32  m_fLookUpAnlge;
            f32  m_fLookDownAngle;
            f32  m_fLookLeftAngle;
            f32  m_fLookRightAngle;
            f32  m_fEyeCloseAdjust;
            f32  m_fMouseOpenAdjust;
            f32  m_fPoutAdjust;
            f32  m_fLipsAdjust;
            s32    m_enableTestFacePt;
            s32    m_enableSendEvent;
            s32 m_standardPt[400];
            
            s32 m_personPt[400];
            
            void _analyzeExpression();
            
            void _adaptPersonFaceData(s32 _ptnum);
            
            void _sendFaceExpressionEvents();
            
            void sendEvent(cptr8 eventName, SV_EVENT_TYPE _eventType, f32 _confidence ,f32 _expvalue = 0.0f);
            
            void _loadConfig();
            
            s32 _getRealIndex(s32 _index);
        };
        
    }//!namespace detect
    
}//!namespace sv




#endif /* SVPersonExpressionExt_h */
