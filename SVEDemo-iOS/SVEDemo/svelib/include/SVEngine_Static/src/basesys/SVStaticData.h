//
// SVStaticData.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_STATICDATA_H
#define SV_STATICDATA_H

//基础渲染数据接口 包含很多 基本的数据

#include "../rendercore/SVRenderDef.h"
#include "../base/SVGBase.h"
#include "../core/SVVertDef.h"
#include "../base/SVPreDeclare.h"

namespace sv {

    class SVStaticData : public SVGBase {
    public:
        SVStaticData(SVInst* _app);
        
        ~SVStaticData();
        
        SVRenderMeshPtr m_screenMesh;
        SVRenderMeshPtr m_screenMeshRot90;
        SVRenderMeshPtr m_screenMeshRot180;
        SVRenderMeshPtr m_screenMeshRot270;
        SVRenderMeshPtr m_screenLTMesh;
        SVRenderMeshPtr m_screenLBMesh;
        SVRenderMeshPtr m_screenRTMesh;
        SVRenderMeshPtr m_screenRBMesh;
        //
        static V3_C_T0 m_baseRect[6];
        
        SVMeshDataPtr getMeshData();
        
    protected:
        
        void init();
        
        void destroy();
        
        void _initBaseRect();
        
        SVMeshDataPtr m_meshDataPtr;
        
    };
    
}//!namespace




#endif //SV_RENDERDATA_H
