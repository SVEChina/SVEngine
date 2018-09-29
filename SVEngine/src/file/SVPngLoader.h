//
// SVPngLoader.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PNGLOADER_H
#define SV_PNGLOADER_H

#include "SVFileLoader.h"
#include <png.h>

namespace sv {
    
    typedef struct {
        const u8 *data;
        ssize_t size;
        s32 offset;
    } tImageSource;
    
    class SVPngLoader : public SVFileLoader {
    public:
        friend class SVOpTexLoad;
        
        SVPngLoader(SVInst *_app);
        
        ~SVPngLoader();
        
        void loadData(cptr8 name, u8 **data);
        
    private:
        void _readPngFromStream(void *stream, s32 dataLen, cptr8 fname, u8 **data);
        
        void _premultipliedAlpha(u8* _data, s32 _width, s32 _height);
        
        u32 m_iWidth;
        u32 m_iHeight;
        u32 m_iDataLength;
        s32 m_iDataType;
        s32 mRenderFormat;
        
    };
    
}//!namespace sv




#endif //SV_PNGLOADER_H
