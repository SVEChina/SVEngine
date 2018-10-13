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
namespace sv {
    
    class SVRenderMgr : public SVGBase {
    public:
        SVRenderMgr(SVInst *_app);
        
        ~SVRenderMgr();
        
        void init();
        
        void destroy();
        
        void clear();
        
        void render();
        
        void swapData();
        
        void recycleRes();  //回收GL资源
        
        SVRenderMeshPtr createMeshRObj();
        
        void setRenderer(SVRendererBasePtr _renderer);  //设置渲染器
        
        SVRendererBasePtr getRenderer();    //获取渲染器
        
        void setRenderScene(SVRenderScenePtr _rs);  //设置渲染场景
        
        SVRenderScenePtr getRenderScene();
        
        void pushRCmdCreate(SVRObjBasePtr _robj);
        
        void setRenderTarget(cptr8 _name,SVRenderTargetPtr _rt);
        
        SVRenderTargetPtr getRenderTarget(cptr8 _name);
        
        //
        void updateMainMat(FMat4 _projMat, FMat4 _viewMat, FMat4 _vpMat);
        //
        void addToProjStack(FMat4 _mat);
        FMat4 getProjMat();
        bool removeProjMat();
        //
        void addToViewStack(FMat4 _mat);
        FMat4 getViewMat();
        bool removeViewMat();
        //
        void addToVPStack(FMat4 _mat);
        FMat4 getVPMat();
        bool removeVPMat();
    protected:
        //渲染场景
        SVRenderScenePtr m_pRenderScene;
        //渲染器
        SVRendererBasePtr m_pRenderer;
        //渲染流(缓存流)
        SVRenderStreamPtr m_RStreamCache;
        //
        SVLockPtr m_renderLock;
        SVLockPtr m_logicLock;
        //渲染目标Map
        typedef SVMap<SVString,SVRenderTargetPtr> TARGETPOOL;
        TARGETPOOL m_targetPool;
        //
        typedef SVArray<FMat4> MAT4STACK;
        MAT4STACK m_stack_proj;
        MAT4STACK m_stack_view;
        MAT4STACK m_stack_vp;
        
    };//!namespace logic

}//!namespace sv



#endif //SV_RENDERMGR_H
