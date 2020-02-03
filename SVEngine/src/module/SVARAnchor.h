//
// SVARAnchor.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_ARANCHOR_H
#define SV_ARANCHOR_H


#include "SVModuleBase.h"
#include "../base/SVVec3.h"
#include "../base/SVMat4.h"
#include "../rendercore/SVRenderDeclare.h"
#include "../mtl/SVMtlDeclare.h"
namespace sv{

    struct SVAnchorPoint {
        FVec3 point;
        FVec3 normal;
        FVec3 ext0;
        FVec3 ext1;
    };

    class SVARAnchor : public SVModuleBase {
    public:
        SVARAnchor(SVInst* _app);
        
        ~SVARAnchor();
        
        virtual void init();
        
        virtual void destroy();
        
        virtual void update(f32 _dt);
        
        virtual void open();
        
        virtual void close();
        
        virtual bool isOpen();
        
        virtual bool procEvent(SVEventPtr _event);
        
    protected:
        void _addAnchor(f32 _x, f32 _y);
        
        void _generateBox(SVAnchorPoint &_worldPoint);
        
        void _screenPointToWorldAnchorPoint(FVec2 &_point, SVAnchorPoint &_worldPoint);
         
        f32 m_plane_dis;
        SVRenderTexturePtr m_fbo;
        SVTexturePtr m_pTex;
        SVMtlCorePtr m_mtl;
        SVRenderMeshPtr m_pMesh;
        SVRenderObjectPtr m_pRenderObj;
        SV3DBoxPtr t_testBox;
    };
    
    
}//!namespace sv

#endif /* SV_ARANCHOR_H */
