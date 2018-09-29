//
// SVTextureIOS.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_TEXTURE_IOS_H
#define SV_TEXTURE_IOS_H

#include "SVTexture.h"

#ifdef SV_IOS

#include "VideoToolBox/VideoToolbox.h"
#include <CoreVideo/CVOpenGLESTextureCache.h>

#endif

namespace sv {
    
    namespace mtl {
        
        class SVTextureIOS : public SVTexture {
        public:
            SVTextureIOS(SVInst *_app);
            
            ~SVTextureIOS();
            
            virtual void init(cptr8 _name, s32 _type, s32 _width, s32 _height, s32 _informate, s32 _dateformate);
            
            virtual void create(SVRendererBasePtr _renderer);
            
            virtual void destroy(SVRendererBasePtr _renderer);
            
            virtual void pushData(u8* _srcPtr,s32 _w,s32 _h,s32 _pixelformate);
            
            virtual void fetchData(u8* _dstPtr,s32 _w,s32 _h);
        protected:
            void _updateData();
        };
    }
}



#endif //SV_TEXTURE_IOS_H
