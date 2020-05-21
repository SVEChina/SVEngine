//
// SVRenderMgr.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RENDERMGR_H
#define SV_RENDERMGR_H

#include "SVRenderDef.h"
#include "../base/SVGBase.h"
#include "../base/SVMap.h"
#include "../base/SVPreDeclare.h"
#include "../base/SVMat4d.h"
#include "../base/SVStack.h"
namespace sv {
    
    class SVRenderMgr : public SVGBase {
    public:
        SVRenderMgr(SVInst *_app);
        
        ~SVRenderMgr();
        
        void init();
        
        void destroy();
        
        void clear();
        
        void render();
        
        void clearScreen();
        
        void swapData();
        
        void recycleRes();  //回收GL资源
        
        SVRenderMeshPtr createMeshRObj();
        
        void setRenderer(SVRendererPtr _renderer);  //设置渲染器
        
        SVRendererPtr getRenderer();    //获取渲染器
        
        void setRenderScene(SVRenderScenePtr _rs);  //设置渲染场景
        
        SVRenderScenePtr getRenderScene();
        
        void pushRCmdCreate(SVRObjBasePtr _robj);
        
        void setRenderTarget(cptr8 _name,SVRenderTargetPtr _rt);
        
        SVRenderTargetPtr getRenderTarget(cptr8 _name);
    
    protected:
        void _adapt();
        s32 m_adaptMode;
        //渲染场景
        SVRenderScenePtr m_pRenderScene;
        //渲染器
        SVRendererPtr m_pRenderer;
        //渲染流(缓存流)
        SVRenderStreamPtr m_RStreamCache;
        //
        SVLockPtr m_renderLock;
        SVLockPtr m_logicLock;
        
    };//!namespace logic

}//!namespace sv



#endif //SV_RENDERMGR_H
