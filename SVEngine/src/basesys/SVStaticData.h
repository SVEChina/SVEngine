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
#include "../base/SVMap.h"
namespace sv {

    class SVStaticData : public SVGBase {
    public:
        enum EFFICACYMESHTYPE{
            EFFICACY_MESH_TYPE_NONE = 0,
            EFFICACY_MESH_TYPE_TWODIVISION = 1,//二分
            EFFICACY_MESH_TYPE_FOURDIVISION,   //四分 +字分割
            EFFICACY_MESH_TYPE_FOURDIVISION_X  //四分 X字分割
        };
        SVStaticData(SVInst* _app);
        
        ~SVStaticData();
        
        void init();
        
        void destroy();
        
        SVMeshDataPtr getMeshData();
        
        SVRenderMeshPtr getMeshByType(EFFICACYMESHTYPE _type);
    public:
        SVRenderMeshPtr m_screenMesh;
        SVRenderMeshPtr m_screenMeshRot90;
        SVRenderMeshPtr m_screenMeshRot180;
        SVRenderMeshPtr m_screenMeshRot270;
        SVRenderMeshPtr m_screenLTMesh;
        SVRenderMeshPtr m_screenLBMesh;
        SVRenderMeshPtr m_screenRTMesh;
        SVRenderMeshPtr m_screenRBMesh;
        static V3_C_T0 m_baseRect[6];
    protected:
        void _initBaseRect();
        
        void _initEfficacyMeshs();
        
        void _clearEfficacyMeshs();
        
        void _initTwoDivisionMesh();
        
        void _initFourDivisionMesh();
        
        void _initFourDivisionMesh_X();
    protected:
        SVMeshDataPtr m_meshDataPtr;
        typedef SVMap<EFFICACYMESHTYPE, SVRenderMeshPtr> EFFICACYMESHPOOL;
        EFFICACYMESHPOOL m_efficacyMeshPool;
    };
    
}//!namespace




#endif //SV_RENDERDATA_H
