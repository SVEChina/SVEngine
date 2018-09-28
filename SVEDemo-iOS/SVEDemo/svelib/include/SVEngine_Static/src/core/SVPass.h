//
// SVPass.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PASS_H
#define SV_PASS_H

#include "../base/SVObject.h"
#include "../rendercore/SVRenderDeclare.h"
#include "../mtl/SVMtlDeclare.h"

namespace sv {
    
    namespace util{
        
        class SVPass :public SVObject {
        public:
            SVPass();

            ~SVPass();

            bool setInTex(s32 _index,SVTexturePtr _tex);

            bool setOutTex(SVTexturePtr _tex);

            void setMtl(SVMtlCorePtr _mtl);
            
            void setMesh( SVRenderMeshPtr _pMesh);

            SVMtlCorePtr m_pMtl;

            SVTexturePtr m_outTex;
            
            SVRenderMeshPtr m_pMesh;
        };
    };
};

#endif /* SV_PASS_H */
