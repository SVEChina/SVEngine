//
// SVEventDef
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_EVENTDEF_H
#define SV_EVENTDEF_H

#include "SVEventDeclare.h"

//
#define EVENT_EYE_LINK      "eyeblink"
#define EVENT_EYE_OPEN      "eyebopen"
#define EVENT_EYE_CLOSE     "eyeclose"

#define EVENT_HAS_WHOLE_FALSE   "haveWholeFace"
#define EVENT_NO_WHOLE_FALSE    "NotWholeFace"

#define EVENT_MOUSE_OPEN    "mouthopen"
#define EVENT_MOUSE_CLOSE   "mouthclose"

#define EVENT_SHAKE_HEAD   "shakehead"

namespace sv {
    
    //
    enum SV_EVENT_TYPE {
        EVN_T_NONE,
        EVN_T_TOUCH_BEGIN,
        EVN_T_TOUCH_MOVE,
        EVN_T_TOUCH_END,
        EVN_T_CAMERA_OREN,
        EVN_T_PICK_COLOR,
        EVN_T_PICK_NODE_CHANGE,
        EVN_T_PICK_GET_NODE,
        EVN_T_PICK_GET_NOTHING,
        EVN_T_ANIMATE,
        EVN_T_LINK,
        EVN_T_FACESHAPE,
        EVN_T_READYENTER_PACKET,
        EVN_T_ENDENTER_PACKET,
        EVN_T_CLOSE_PACKET,
        EVN_T_LEYE_OPEN,
        EVN_T_LEYE_BLINK,
        EVN_T_LEYE_CLOSE,
        EVN_T_REYE_OPEN,
        EVN_T_REYE_BLINK,
        EVN_T_REYE_CLOSE,
        EVN_T_HAS_WHOLE_FACE,
        EVN_T_NO_WHOLE_FACE,
        EVN_T_MOUSE_OPEN,
        EVN_T_MOUSE_CLOSE,
        EVN_T_MOUSE_OPEN_ACTION,//闭嘴动作
        EVN_T_MOUSE_CLOSE_ACTION,//闭嘴动作
        EVN_T_MOUSE_TOUTH,//嘟嘴
        EVN_T_MOUSE_RAISE_CORNER,//嘴角上扬
        EVN_T_MOUSE_BOW_CORNER,//嘴角向下
        EVN_T_SHAKE_HEAD,
        EVN_T_LEFT_TUREN_HEAD,//左扭头
        EVN_T_RIGHT_TUREN_HEAD,//右扭头
        EVN_T_NONE_TUREN_HEAD,//左右居中
        EVN_T_RAISE_HEAD,//抬头
        EVN_T_BOW_HEAD,//低头
        EVN_T_NONE_RAISE_BOW_HEAD,//上下头居中
        EVN_T_C_MOUTHOPEN_TURNLEFT,//组合表情，张嘴左扭头
        EVN_T_C_MOUTHOPEN_TURNRIGHT,//组合表情，张嘴右扭头
        EVN_T_C_LEFTEYEOPEN_RIGHTEYECLOSE,//组合表情，左睁眼，右闭眼
        EVN_T_C_LEFTEYECLOSE_RIGHTEYEOPEN,//组合表情，右睁眼，左闭眼
        EVN_T_C_LEFTEYECLOSE_RIGHTEYECLOSE,//组合表情，左闭眼，右闭眼
        EVN_T_C_MOUTHTOUTH_TURNLEFT,//组合表情，左嘟嘴
        EVN_T_C_MOUTHTOUTH_TURNRIGHT,//组合表情，右嘟
        EVN_T_TIMEOUT,
        EVN_T_ADD_TIME,
        EVN_T_PARSE,
        EVN_T_REACT_EVENT,
        EVN_T_VIDEO_CTRL,
        EVN_T_VIDEO_STATE,
        EVN_T_TEX_LOAD_FINISH,
        EVN_T_SAVE_BEGIN,
        EVN_T_SAVE_END,
        EVN_T_MAX = 100
    };
    
    //
    enum TOUCH_ACTION_TYPE {
        TOUCH_ACTION_DOWN,
        TOUCH_ACTION_MOVE,
        TOUCH_ACTION_UP
    };
    
    //
    enum VIDEOCTRL {
        VIDEO_CTRL_NONE,
        VIDEO_CTRL_PLAY,
        VIDEO_CTRL_PAUSE,
        VIDEO_CTRL_STOP,
        VIDEO_CTRL_RESET,
        VIDEO_CTRL_PREVIEW
    };
    
    //
    enum VIDEOSTATE {
        VIDEO_STATE_NONE,
        VIDEO_STATE_LOADBEIGN,
        VIDEO_STATE_LOADEND,
        VIDEO_STATE_PLAYBEGIN,      //视频渲染开始
        VIDEO_STATE_PLAYEND,        //视频渲染完毕
        VIDEO_STATE_SAVEBEGIN,
        VIDEO_STATE_SAVEEND,
        VIDEO_STATE_TMPLATE_LOADOVER    //模板加载完毕
    };

}

#endif //SV_EVENTDEF_H
