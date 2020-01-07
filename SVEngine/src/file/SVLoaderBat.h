//
// SVLoaderBat.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_BATLOADER_H
#define SV_BATLOADER_H

#include "SVFileLoader.h"

namespace sv {
    
    //加载普通的明文文件
    class SVLoaderBat : public SVFileLoader {
    public:
        SVLoaderBat(SVInst *_app);
        
        bool loadFromFile(cptr8 _filename);
    };
    
}//!namespace sv



#endif //SV_BATLOADER_H
