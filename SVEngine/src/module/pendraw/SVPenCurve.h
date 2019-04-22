//
// SVPenCurve.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PENCURVE_H
#define SV_PENCURVE_H

#include "../../base/SVGBase.h"
#include "../../base/SVVec2.h"
#include "../../base/SVVec3.h"
#include "../../base/SVArray.h"
namespace sv{
    
    //插值方法
    enum LERPMETHOD {
        SV_LERP_BALANCE= 0,
        SV_LERP_NOTBALANCE
    };
    
    enum ADDPOINTACTION {
        SV_ADD_DRAWBEGIN = 0,
        SV_ADD_DRAWING,
        SV_ADD_DRAWEND
    };
    
    class SVPenCurve : public SVGBase {
    public:
        SVPenCurve(SVInst *_app);
        
        ~SVPenCurve();
        
        void reset();
        
        bool addPoint(FVec3 &_pt, f32 _width, f32 _density, ADDPOINTACTION _action, SVArray<FVec3> &_outPtPool);
        
        bool addPointB(FVec3 &_pt, f32 _width, f32 _density, ADDPOINTACTION _action, SVArray<FVec3> &_outPtPool);
        
    protected:
         void
        _lerpNor(f32 _width, f32 _density, SVArray<FVec3> &_outPtPool, bool _useLast3);
        
        void
        _lineLerp(f32 _width, f32 _density, SVArray<FVec3> &_outPtPool);
        
        void _lineForLerp(FVec3 _p1, FVec3 _p2, f32 _width, f32 _density, ADDPOINTACTION _action,
                                      SVArray<FVec3> &_outPtPool);
        
        void
        _lerp(f32 _width, f32 _density, ADDPOINTACTION _action, SVArray<FVec3> &_outPtPool, bool _useLast3);
        
        void _onePt(f32 _width, f32 _density, SVArray<FVec3> &_outPtPool);
        
        typedef SVArray<FVec3> PTPOOL;
        PTPOOL m_ptPool;
        bool m_bPushSamePoint;
    };
    
}//!namespace sv

#endif //SV_PENCURVE_H
