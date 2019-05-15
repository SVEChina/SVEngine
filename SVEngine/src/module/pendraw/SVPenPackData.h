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
        
        bool savePenData(SVDataSwapPtr _data, cptr8 _path);
        
        u32 getDataLength();
    protected:
        u32 m_dataLength;
    };
    
}//!namespace sv

#endif //SV_PENPACKDATA_H
