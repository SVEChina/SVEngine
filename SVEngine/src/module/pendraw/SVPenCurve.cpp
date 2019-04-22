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


double L(double t, double A, double B, double C) {
    double ttt = C + t * (B + A * t);
    if(ttt<0.0000000)
        return 1.0;
    if(A<0.0000000)
        return 1.0;
    if(C<0.0000000)
        return 1.0;
    double temp1 = sqrt(ttt);
    //
    double temp2 = (2 * A * t * temp1 + B * (temp1 - sqrt(C)));
    //
    ttt = B + 2 * sqrt(A) * sqrt(C);
    double temp3 = 1.0;
    if(abs(ttt)>=0.0000001){
        temp3 = log(ttt);
    }
    //
    ttt = B + 2 * A * t + 2 * sqrt(A) * temp1;
    double temp4 = 1.0 ;
    if(abs(ttt)>=0.0000001){
        temp4 = log(ttt);
    }
    //
    double temp5 = 2 * sqrt(A) * temp2;
    //
    double temp6 = (B * B - 4 * A * C) * (temp3 - temp4);
    return (temp5 + temp6) / (8 * pow(A, 1.5));
}

double s(double t, double A, double B, double C) {
    double ttt = A * t * t + B * t + C;
    if(ttt<0.0000000)
        return 1.0;
    return sqrt(ttt);
}

double InvertL(double t, double l, double A, double B, double C) {
    double t1 = t;
    double t2 = 0.0;
    int test = 1;
    do {
        test++;
        double ttt = s(t1, A, B, C);
        t2 = t1 - (L(t1, A, B, C) - l) / ttt;
        if (abs(t1 - t2) < 0.000001 || (test > 20) ){//林龙飞：(test > 20)为了防止特殊数字无限循环
            break;
        }
        t1 = t2;
    } while (true);
    return t2;
}

bool SVPenCurve::addPointB(FVec3 &_pt, f32 _width, f32 _density, ADDPOINTACTION _action, SVArray<FVec3> &_outPtPool){
    //如果插入点跟上一个点不同或者没有上一个点，推入
    FVec3 t_prePoint = FVec3(-10000, -10000, -10000);
    s32 t_ptSize = m_ptPool.size();
    if (t_ptSize > 0)
        t_prePoint = m_ptPool[m_ptPool.size() - 1];
    if ((t_prePoint.x == _pt.x) && (t_prePoint.y == _pt.y) && (t_prePoint.z == _pt.z)) {
        m_bPushSamePoint = true;
    } else {
        m_ptPool.append(_pt);
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
                _lerp(_width, _density, _action, _outPtPool, true);
            }
            if (_action == SV_ADD_DRAWEND) {
                _lerp(_width, _density, _action, _outPtPool, true);
            }
        } else if (t_ptSize == 4) {
            if (_action == SV_ADD_DRAWING) {
                
            }
            if (_action == SV_ADD_DRAWEND) {
                _lerp(_width, _density, _action, _outPtPool, true);
            }
        } else if (t_ptSize > 4) {
            if (_action == SV_ADD_DRAWING) {
                _lerp(_width, _density, _action, _outPtPool, false);
            }
            if (_action == SV_ADD_DRAWEND) {
                _lerp(_width, _density, _action, _outPtPool, false);
            }
        }
    } else {//如果插入点跟上一个点相同，不推入：
        if (t_ptSize == 1) {
            if (_action == SV_ADD_DRAWEND) {
                _lineLerp(_width, _density, _outPtPool);
            }
        } else if (t_ptSize == 2) {
            if (_action == SV_ADD_DRAWEND) {
                _lineLerp(_width, _density, _outPtPool);//线性插值,画笔尖
            }
        } else if (t_ptSize == 3) {
            
        } else if (t_ptSize == 4) {
            if (_action == SV_ADD_DRAWEND) {
                _lerp(_width, _density, _action, _outPtPool, true);
            }
        } else if (t_ptSize > 4) {
            if (_action == SV_ADD_DRAWEND) {
                _lerp(_width, _density, _action, _outPtPool, true);
            }
        }
    }
    return true;
}

bool SVPenCurve::addPoint(FVec3 &_pt, f32 _width, f32 _density, ADDPOINTACTION _action, SVArray<FVec3> &_outPtPool){
    //如果插入点跟上一个点不同或者没有上一个点，推入
    FVec3 t_prePoint = FVec3(-10000, -10000, -10000);
    s32 t_ptSize = m_ptPool.size();
    if (t_ptSize > 0)
        t_prePoint = m_ptPool[m_ptPool.size() - 1];
    if ((t_prePoint.x == _pt.x) && (t_prePoint.y == _pt.y) && (t_prePoint.z == _pt.z)) {
        m_bPushSamePoint = true;
    } else {
        m_ptPool.append(_pt);
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
SVPenCurve::_lerpNor(f32 _width, f32 _density, SVArray<FVec3> &_outPtPool, bool _useLast3){
    s32 size = (s32) m_ptPool.size();
    if (size < 3)
        return;
    //只有3个点才能插值
    FVec3 P2;
    FVec3 P1;
    FVec3 P0;
    
    if (_useLast3) {
        P2 = m_ptPool[size - 1];
        P1 = m_ptPool[size - 2];
        P0 = m_ptPool[size - 3];
    } else {
        P2 = m_ptPool[size - 2];
        P1 = m_ptPool[size - 3];
        P0 = m_ptPool[size - 4];
    }
    
    double t, delta = 1 / 20.0;
    for (t = 0; t < 1.0; t += delta) {
        /*** core code ***/
        double g0 = 0.5 * (t - 1) * (t - 1);
        double g1 = 0.5 * (-2 * t * t + 2 * t + 1);
        double g2 = 0.5 * t * t;
        float _x = (float) (P0.x * g0 + P1.x * g1 + P2.x * g2);
        float _y = (float) (P0.y * g0 + P1.y * g1 + P2.y * g2);
        float _z = (float) (P0.z * g0 + P1.z * g1 + P2.z * g2);
        /*****************/
        FVec3 tempP = FVec3(_x, _y, _z);
        _outPtPool.append(tempP);//插值
    }
}

void
SVPenCurve::_lineLerp(f32 _width, f32 _density, SVArray<FVec3> &_outPtPool){
    FVec3 tempP1 = m_ptPool[0];
    FVec3 tempP2 = m_ptPool[1];
    FVec3 t_dir = tempP2 - tempP1;
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
        FVec3 tempP = tempP1 + t_dir * t_acc_len;
        _outPtPool.append(tempP);
        t_acc_len += t_step;
        if (t_acc_len >= t_len) {
            _outPtPool.append(tempP2);
            break;  //推送最后一个点
        }
    }
}

void SVPenCurve::_lineForLerp(FVec3 _p1, FVec3 _p2, f32 _width, f32 _density, ADDPOINTACTION _action,
                           SVArray<FVec3> &_outPtPool) {
    FVec3 tempP1 = _p1;
    FVec3 tempP2 = _p2;
    FVec3 t_dir = tempP2 - tempP1;
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
        FVec3 tempP = tempP1 + t_dir * t_acc_len;
        _outPtPool.append(tempP);
        t_acc_len += t_step;
        if (t_acc_len >= t_len) {
            _outPtPool.append(tempP2);
            break;  //推送最后一个点
        }
    }
}

void SVPenCurve::_onePt(f32 _width, f32 _density, SVArray<FVec3> &_outPtPool){
    FVec3 tempP = m_ptPool[0];
    _outPtPool.append(tempP);
}

void
SVPenCurve::_lerp(f32 _width, f32 _density, ADDPOINTACTION _action, SVArray<FVec3> &_outPtPool, bool _useLast3) {
    s32 size = (s32) m_ptPool.size();
    if (size < 3)
        return;
    //只有3个点才能插值
    FVec3 P2;
    FVec3 P1;
    FVec3 P0;
    
    if (_useLast3) {
        P2 = m_ptPool[size - 1];
        P1 = m_ptPool[size - 2];
        P0 = m_ptPool[size - 3];
    } else {
        P2 = m_ptPool[size - 2];
        P1 = m_ptPool[size - 3];
        P0 = m_ptPool[size - 4];
    }
    
    double ax = (P0.x - 2 * P1.x + P2.x) * 0.5;
    double ay = (P0.y - 2 * P1.y + P2.y) * 0.5;
    double bx = P1.x - P0.x;
    double by = P1.y - P0.y;
    double A = 4 * (ax * ax + ay * ay);
    double B = 4 * (ax * bx + ay * by);
    double C = bx * bx + by * by;
    
    double total_length = L(1, A, B, C);
    
    int t_step = total_length / (_width * _density);
    
    if (t_step <= 5) {
        FVec3 t_p1((P0.x + P1.x) / 2.0, (P0.y + P1.y) / 2.0, (P0.z + P1.z) / 2.0);
        FVec3 t_p2((P1.x + P2.x) / 2.0, (P1.y + P2.y) / 2.0, (P1.z + P2.z) / 2.0);
        _lineForLerp(t_p1, t_p2, _width, _density , _action, _outPtPool);
    } else {
        for (int i = 0; i < t_step; i++) {
            double t = (double) i / t_step;
            double l = t * total_length;
            
            t = InvertL(t, l, A, B, C);
            /*** core code ***/
            double g0 = 0.5 * (t - 1) * (t - 1);
            double g1 = 0.5 * (-2 * t * t + 2 * t + 1);
            double g2 = 0.5 * t * t;
            float _x = (float) (P0.x * g0 + P1.x * g1 + P2.x * g2);
            float _y = (float) (P0.y * g0 + P1.y * g1 + P2.y * g2);
            float _z = (float) (P0.z * g0 + P1.z * g1 + P2.z * g2);
            /*****************/
            FVec3 tempP = FVec3(_x, _y, _z);
            _outPtPool.append(tempP);//插值
        }
    }
}

