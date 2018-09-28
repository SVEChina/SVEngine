//
// SVObjLoader.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_OBJLOADER_H
#define SV_OBJLOADER_H

#include "SVFileLoader.h"

namespace sv {
    
    class SVObjLoader : public SVFileLoader {
    public:
        SVObjLoader(SVInst *_app);
        
        SVModelPtr loadModel(cptr8 _fname);
    };
    
}//!namespace sv



#endif //SV_OBJLOADER_H
