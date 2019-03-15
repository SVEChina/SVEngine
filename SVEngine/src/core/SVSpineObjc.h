//
// SVSpineObjc.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_SPINEOBJC_H
#define SV_SPINEOBJC_H

#include "../third/spine/include/spine/Atlas.h"
#include "../base/SVPreDeclare.h"
#include "../mtl/SVMtlDeclare.h"

extern "C" {
    namespace sv{
        namespace mtl{
            class SVSpineObjc{
            public:
                SVSpineObjc();
                ~SVSpineObjc();
                SVTexturePtr m_texture;
            };
        }
        
    }
    
char *_spUtil_readFile(void *_app, const char *path, int *length);
    
void _spAtlasPage_createTexture(void *_app, spAtlasPage *self, const char *path, char enableMipMap);
    
void _spAtlasPage_disposeTexture(spAtlasPage *self);
    
};

#endif //SV_MASPINEOBJC_H
