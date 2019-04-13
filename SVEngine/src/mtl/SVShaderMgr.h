//
// SVShaderMgr.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_SHADERMGR_H
#define SV_SHADERMGR_H

#include "../base/SVDef.h"
#include "../base/SVObject.h"
#include "../base/SVMap.h"
#include "../basesys/SVSysBase.h"
#include "../rendercore/SVResShader.h"

namespace sv {
    
    class SVShaderMgr : public SVSysBase {
    public:
        SVShaderMgr(SVInst *_app);
        
        ~SVShaderMgr();
        
        void init();
        
        void destroy();
        
        u32 getProgramme(cptr8 _name);
        
    private:
        void loadAllShader();
        
        void clearAllShader();
        
    protected:
        typedef SVMap<SVString,render::SVResShaderPtr> SHADERPOOL;
        SHADERPOOL ShaderMap;
    };
    
}//!namespace sv


#endif //SV_SHADERMGR_H
