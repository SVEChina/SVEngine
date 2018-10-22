//
// SVSceneMgr.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_SCENEMGR_H
#define SV_SCENEMGR_H

#include "../basesys/SVSysBase.h"

namespace sv {

    class SVSceneMgr : public SVSysBase {
    public:
        SVSceneMgr(SVInst *_app);
        
        ~SVSceneMgr();
        
        void init();
        
        void destroy();
        
        void setScene(SVScenePtr _scene);
        
        SVScenePtr getScene();
        
        //ui变换到scene中
        void uiToScene(f32& _w,f32& _h);
        
        void update(f32 dt);
        
    protected:
        SVScenePtr m_pMainScene;
        
        SVLockPtr m_sceneLock;
    };
    
}//!namespace




#endif //SV_SCENEMGR_H
