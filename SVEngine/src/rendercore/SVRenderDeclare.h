//
// SVRenderDeclare.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RENDECLARE_H
#define SV_RENDECLARE_H

#include "../base/SVDef.h"
#include "../base/SVPreDeclare.h"

namespace sv {
    
    namespace render {

        //渲染资源
        DECLARE_SHAREPTR(SVRRes);
        DECLARE_SHAREPTR(SVResTex);
        DECLARE_SHAREPTR(SVResFBO);
        DECLARE_SHAREPTR(SVResVBO);
        DECLARE_SHAREPTR(SVResShader);

        //渲染器
        DECLARE_SHAREPTR(SVRenderer);
        DECLARE_SHAREPTR(SVRenderState);
        DECLARE_SHAREPTR(SVRenderTexture);
        
        //gl
        DECLARE_SHAREPTR(SVRendererGL);
        DECLARE_SHAREPTR(SVRenderStateGL);
        DECLARE_SHAREPTR(SVRResGLTex);
        DECLARE_SHAREPTR(SVRResGLTexWithTexID);
        DECLARE_SHAREPTR(SVRResGLTexPlist);
        DECLARE_SHAREPTR(SVRResGLTexSet);
        DECLARE_SHAREPTR(SVRResGLTexiOS);
        DECLARE_SHAREPTR(SVRResGLShader);
        DECLARE_SHAREPTR(SVRResGLFBO);
        DECLARE_SHAREPTR(SVRResGLOutFBO);
        DECLARE_SHAREPTR(SVResGLRenderTarget);
        DECLARE_SHAREPTR(SVResGLRenderTargetOut);
        DECLARE_SHAREPTR(SVResGLRenderTexture);
        DECLARE_SHAREPTR(SVResGLVBO);
        DECLARE_SHAREPTR(SVResGLRenderMesh);
        DECLARE_SHAREPTR(SVResGLRenderMeshDvid);
        DECLARE_SHAREPTR(SVResGLRenderMeshData);
        
        //metal
        #if defined(SV_IOS) || defined(SV_OSX)
        DECLARE_SHAREPTR(SVRendererMetal);
        DECLARE_SHAREPTR(SVRResMetalTex);
        DECLARE_SHAREPTR(SVRResMetalShader);
        DECLARE_SHAREPTR(SVResMetalRenderMesh);
        #endif
        //vulkan
        DECLARE_SHAREPTR(SVRendererVK);
        
        //渲染框架
        DECLARE_SHAREPTR(SVFboObject);
        DECLARE_SHAREPTR(SVRenderTarget);
        DECLARE_SHAREPTR(SVRenderTargetDefault);
        DECLARE_SHAREPTR(SVRenderTargetOut);
        DECLARE_SHAREPTR(SVRenderTargetOutTexture);
        DECLARE_SHAREPTR(SVRenderScene);
        DECLARE_SHAREPTR(SVRenderPipline);
        DECLARE_SHAREPTR(SVRenderStream);
        DECLARE_SHAREPTR(SVRenderMesh);
        DECLARE_SHAREPTR(SVRenderMeshDvid);
        
        //rendercmd
        DECLARE_SHAREPTR(SVRenderCmd);
        DECLARE_SHAREPTR(SVRenderCmdTransGPU);
        DECLARE_SHAREPTR(SVRenderCmdPushVPMat);
        DECLARE_SHAREPTR(SVRenderCmdPopVPMat);
        DECLARE_SHAREPTR(SVRenderCmdPushMat);
        DECLARE_SHAREPTR(SVRenderCmdPopMat);
        DECLARE_SHAREPTR(SVRenderCmdLine);
        DECLARE_SHAREPTR(SVRenderCmdRect);
        DECLARE_SHAREPTR(SVRenderCmdAABB);
        DECLARE_SHAREPTR(SVRenderCmdNor);
        DECLARE_SHAREPTR(SVRenderCmdPass);
        DECLARE_SHAREPTR(SVRenderCmdPassCollection);
        DECLARE_SHAREPTR(SVRenderCmdFboBind);
        DECLARE_SHAREPTR(SVRenderCmdClear);
        DECLARE_SHAREPTR(SVRenderCmdAdapt);
        DECLARE_SHAREPTR(SVRenderCmdFboUnbind);
        DECLARE_SHAREPTR(SVRenderCmdStreamOutNor);
        DECLARE_SHAREPTR(SVRenderCmdStreamOutIOS);
        DECLARE_SHAREPTR(SVRCmdCreate);
        DECLARE_SHAREPTR(SVRenderCmdGeo);
        DECLARE_SHAREPTR(SVRenderCmdLine);
        DECLARE_SHAREPTR(SVRenderCmdRect);
        DECLARE_SHAREPTR(SVRenderCmdCurve);
        DECLARE_SHAREPTR(SVRenderCmdCircle);
        DECLARE_SHAREPTR(SVRenderCmdCircleSolid);
        
        //renderobject
        DECLARE_SHAREPTR(SVRObjBase);
        DECLARE_SHAREPTR(SVRenderObject);
        DECLARE_SHAREPTR(SVMultPassRenderObject);
        DECLARE_SHAREPTR(SVMultMeshRenderObject);
        DECLARE_SHAREPTR(SVMultMeshMtlRenderObject);

    }//!namespace render

}//!namespace sv

#endif //SV_RENDECLARE_H
