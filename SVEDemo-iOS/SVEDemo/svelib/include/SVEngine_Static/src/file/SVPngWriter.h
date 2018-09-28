//
// SVPngWriter.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PNGWRITER_H
#define SV_PNGWRITER_H


#include "SVFileWriter.h"

namespace sv {
    
    class SVPngWriter: public SVFileWriter {
    public:
        SVPngWriter(SVInst* _app);
        bool write(cptr8 fname,s32 width,s32 height,void* data);
    };

    
}//!namespace sv



#endif //SV_PNGWRITER_H
