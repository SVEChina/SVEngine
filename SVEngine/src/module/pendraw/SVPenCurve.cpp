//
// SVPenCurve.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVPenCurve.h"
SVPenCurve::SVPenCurve(SVInst *_app)
:SVGBase(_app) {
    m_bPushSamePoint = false;
}

SVPenCurve::~SVPenCurve() {
    m_ptPool.clear();
}

void SVPenCurve::reset(){
    m_ptPool.clear();
    m_bPushSamePoint = false;
}

bool SVPenCurve::addPoint(f32 x, f32 y, f32 _width, f32 _density, ADDPOINTACTION _action, SVArray<FVec2> &_outPtPool){
    //如果插入点跟上一个点不同或者没有上一个点，推入
    FVec2 t_prePoint = FVec2(-10000, -10000);
    s32 t_ptSize = m_ptPool.size();
    if (t_ptSize > 0)
        t_prePoint = m_ptPool[m_ptPool.size() - 1];
    
    if ((t_prePoint.x == x) && (t_prePoint.y == y)) {
        m_bPushSamePoint = true;
    } else {
        m_ptPool.append(FVec2(x, y));
        t_ptSize++;
        m_bPushSamePoint = false;
    }
    if (!m_bPushSamePoint) {
        if (t_ptSize == 1) {
            //什么都不做
        } else if (t_ptSize == 2) {
            if (_action == SV_ADD_DRAWEND) {
                _lineLerp(_width, _density, _outPtPool);
            }
        } else if (t_ptSize == 3) {
            if (_action == SV_ADD_DRAWING) {
                _lerpNor(_width, _density, _outPtPool, true);
            }
            if (_action == SV_ADD_DRAWEND) {
                _lerpNor(_width, _density, _outPtPool, true);
            }
        } else if (t_ptSize == 4) {
            if (_action == SV_ADD_DRAWING) {
                
            }
            if (_action == SV_ADD_DRAWEND) {
                _lerpNor(_width, _density, _outPtPool, true);
            }
        } else if (t_ptSize > 4) {
            if (_action == SV_ADD_DRAWING) {
                _lerpNor(_width, _density, _outPtPool, false);
            }
            if (_action == SV_ADD_DRAWEND) {
                _lerpNor(_width, _density, _outPtPool, false);
            }
        }
    } else {//如果插入点跟上一个点相同，不推入：
        if (t_ptSize == 1) {
            if (_action == SV_ADD_DRAWEND) {
                _onePt(_width, _density, _outPtPool);//（就是点击一下的情况，画圆点）
            }
        } else if (t_ptSize == 2) {
            if (_action == SV_ADD_DRAWEND) {
                _lineLerp(_width, _density, _outPtPool);//线性插值,画笔尖
            }
        } else if (t_ptSize == 3) {
            
        } else if (t_ptSize == 4) {
            if (_action == SV_ADD_DRAWEND) {
                _lerpNor(_width, _density, _outPtPool, true);
            }
        } else if (t_ptSize > 4) {
            if (_action == SV_ADD_DRAWEND) {
                _lerpNor(_width, _density, _outPtPool, true);
            }
        }
    }
    return true;
}

void
SVPenCurve::_lerpNor(f32 _width, f32 _density, SVArray<FVec2> &_outPtPool, bool _useLast3){
    s32 size = (s32) m_ptPool.size();
    if (size < 3)
        return;
    //只有3个点才能插值
    FVec2 P2;
    FVec2 P1;
    FVec2 P0;
    
    if (_useLast3) {
        P2 = m_ptPool[size - 1];
        P1 = m_ptPool[size - 2];
        P0 = m_ptPool[size - 3];
    } else {
        P2 = m_ptPool[size - 2];
        P1 = m_ptPool[size - 3];
        P0 = m_ptPool[size - 4];
    }
    
    double t, delta = 1 / 100.0;
    for (t = 0; t < 1.0; t += delta) {
        /*** core code ***/
        double g0 = 0.5 * (t - 1) * (t - 1);
        double g1 = 0.5 * (-2 * t * t + 2 * t + 1);
        double g2 = 0.5 * t * t;
        float _x = (float) (P0.x * g0 + P1.x * g1 + P2.x * g2);
        float _y = (float) (P0.y * g0 + P1.y * g1 + P2.y * g2);
        /*****************/
        FVec2 tempP = FVec2(_x, _y);
        _outPtPool.append(tempP);//插值
    }
}

void
SVPenCurve::_lineLerp(f32 _width, f32 _density, SVArray<FVec2> &_outPtPool){
    FVec2 tempP1 = m_ptPool[0];
    FVec2 tempP2 = m_ptPool[1];
    FVec2 t_dir = tempP2 - tempP1;
    //
    f32 t_len = t_dir.length();
    if (t_len == 0) {
        //就是一个点 返回去 就好
        _outPtPool.append(tempP1);
        return;
    }
    t_dir.normalize();
    f32 t_step = _width * _density; //步长 0.2w代表重叠80%
    f32 t_acc_len = 0.0f;
    while (t_acc_len < t_len) {
        FVec2 tempP = tempP1 + t_dir * t_acc_len;
        _outPtPool.append(tempP);
        t_acc_len += t_step;
        if (t_acc_len >= t_len) {
            _outPtPool.append(tempP2);
            break;  //推送最后一个点
        }
    }
}

void SVPenCurve::_onePt(f32 _width, f32 _density, SVArray<FVec2> &_outPtPool){
    FVec2 tempP = m_ptPool[0];
    _outPtPool.append(tempP);
}
