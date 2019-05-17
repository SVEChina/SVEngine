//
// SVPenPackData.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PENPACKDATA_H
#define SV_PENPACKDATA_H

#include "../../base/SVGBase.h"
namespace sv{
    
    class SVPenPackData : public SVGBase {
    public:
        SVPenPackData(SVInst *_app);
        
        ~SVPenPackData();
        
        void reset();
        
        bool writePenData(SVDataSwapPtr _data, cptr8 _path, bool _clearData = true);
        
        bool loadPenData(SVDataSwapPtr _data, cptr8 _path, s32 _offset, s32 _length);
        
        u64 checkPenStrokeDataLength(cptr8 _path);
        
    };
    
}//!namespace sv

#endif //SV_PENPACKDATA_H
