//
// SVPenStroke.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PENSTROKE_H
#define SV_PENSTROKE_H

#include "../SVGameBase.h"
#include "../../base/SVVec3.h"

namespace sv{
    
    class SVPenStroke : public SVGameBase {
    public:
        SVPenStroke(SVInst* _app);
        
        ~SVPenStroke();
        
        void update(f32 _dt);
        
        void begin(f32 _px,f32 _py,f32 _pz);
        
        void end(f32 _px,f32 _py,f32 _pz);
        
        void draw(f32 _px,f32 _py,f32 _pz);
        
    protected:
        //生成面片
        void _genMesh();
        
        void _drawMesh();
        //
        typedef SVArray<FVec3> PTPOOL;
        PTPOOL m_ptPool;
        //数据域
        SVDataSwapPtr m_dataswap;
//        //
//        static PTPOOL m_ptCache;
    };
    
}//!namespace sv





#endif //SV_PENSTROKE_H
