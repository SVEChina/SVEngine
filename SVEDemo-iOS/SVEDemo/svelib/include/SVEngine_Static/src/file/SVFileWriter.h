//
// SVFileWriter.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_FILEWRITER_H
#define SV_FILEWRITER_H

#include "../base/SVObject.h"
#include "../base/SVGBase.h"
#include <stdio.h>

namespace sv {
    
    class SVFileWriter: public SVGBase {
    public:
        SVFileWriter(SVInst* _app);
        
        ~SVFileWriter();
        
        void setPath(cptr8 _path);
        
        void* createFileHandle(cptr8 _filename);
        
        void destroyFileHandle();
        
        bool write(void* data,s32 len,s32 count);
        
    protected:
        FILE* m_fp;
        SVString m_path;
    };

    
}//!namespace sv



#endif //SV_FILEWRITER_H
