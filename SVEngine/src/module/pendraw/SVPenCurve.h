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
#include "../../base/SVArray.h"
namespace sv{
    
    class SVPenCurve : public SVGBase {
    public:
        enum ADDPOINTACTION {
            SV_ADD_DRAWBEGIN = 0,
            SV_ADD_DRAWING,
            SV_ADD_DRAWEND
        };
        SVPenCurve(SVInst *_app);
        
        ~SVPenCurve();
        
        void reset();
        
        bool addPoint(f32 x, f32 y, f32 _width, f32 _density, ADDPOINTACTION _action, SVArray<FVec2> &_outPtPool);
        
    protected:
         void
        _lerpNor(f32 _width, f32 _density, SVArray<FVec2> &_outPtPool, bool _useLast3);
        void
        _lineLerp(f32 _width, f32 _density, SVArray<FVec2> &_outPtPool);
        void _onePt(f32 _width, f32 _density, SVArray<FVec2> &_outPtPool);
        typedef SVArray<FVec2> PTPOOL;
        PTPOOL m_ptPool;
        bool m_bPushSamePoint;
    };
    
}//!namespace sv

#endif //SV_PENCURVE_H
