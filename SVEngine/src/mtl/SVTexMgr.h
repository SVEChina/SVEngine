//
// SVTexMgr.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_TEXMGR_H
#define SV_TEXMGR_H

#include "../base/SVGBase.h"
#include "../base/SVMap.h"
#include "../mtl/SVMtlDeclare.h"

namespace sv {
    
    class SVTexMgr : public SVGBase {
        //friend class mtl::SVTexture;
    public:
        SVTexMgr(SVInst *_app);
        
        ~SVTexMgr();
        
        void init();
        
        void destroy();
        
        void update(f32 _dt);
        
        SVTexturePtr getTexture(cptr8 _name, bool _create = false, bool _enableMipMap = false);
        
        SVTexturePtr getTextureSync(cptr8 _name, bool _create = false, bool _enableMipMap = false);
        
        SVTexturePtr createUnctrlTexture(s32 _w, s32 _h, s32 _informat, s32 _dataformat, bool _enableMipMap = false);
        
        SVTexturePtr createUnctrlTextureWithTexID(s32 _texId, s32 _width, s32 _height, s32 _informat, s32 _dataformat, bool _enableMipMap = false);
        
        SVTexturePtr createUnctrlTextureWithData(s32 _width, s32 _height, s32 _informat, s32 _dataformat, void* _data, bool _enableMipMap = false);
        
        bool hasTexture(SVTexturePtr _tex);
        
        bool hasTexture(cptr8 _name);
        
        void clear();
        
        SVTexturePtr getSVETexture();
        
        SVTexturePtr loadImgPList(cptr8 _plistname, cptr8 _path);
        
    protected:
        SVTexturePtr _createTexture(cptr8 _name, bool _sync = true, bool _enableMipMap = false);
        
        SVTexturePtr _createTextureSet(cptr8 _name, bool _sync = true, bool _enableMipMap = false);
        
        void _loadTextureSync(SVTexturePtr _tex, cptr8 _name, bool _enableMipMap = false);
        
        void _loadTextureAsync(SVTexturePtr _tex, cptr8 _name, bool _enableMipMap = false);
        
        void _createTexturList();
        
        void _removeUnuseTexture();
        
        typedef SVMap<SVString, SVTexturePtr> TEXPOOL;
        TEXPOOL mTexpool;
        SVLockPtr m_texLock;
        SVTexturePtr m_sveTexture;
        bool mAsync;
    };

    
}//!namespace sv



#endif //SV_TEXMGR_H
