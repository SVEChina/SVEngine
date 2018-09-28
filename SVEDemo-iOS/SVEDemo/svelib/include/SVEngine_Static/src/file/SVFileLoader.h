//
// SVFileLoader.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_FILELOADER_H
#define SV_FILELOADER_H

#include "../base/SVObject.h"
#include "../base/SVGBase.h"
#include "../mtl/SVMtlDeclare.h"

namespace sv {
    
    class SVFileLoader : public SVGBase {
    public:
        SVFileLoader(SVInst *_app);
        
        ~SVFileLoader();
        
        virtual SVTexture *load(cptr8 _filename);
    };

    
}//!namespace sv



#endif //SV_FILELOADER_H
