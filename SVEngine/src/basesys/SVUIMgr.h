//
// SVUIMgr.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_UIMGR_H
#define SV_UIMGR_H

#include "SVSysBase.h"

namespace sv {

    class SVUIMgr : public SVSysBase {
    public:
        SVUIMgr(SVInst *_app);
        
        ~SVUIMgr();
        
        void init();
        
        void destroy();

        void update(f32 dt);
        
        SVNodePtr getRoot();
        
        void pushUINode(SVNodePtr _node);
        
        void removeUINode(SVNodePtr _node);
        
        void clearUINode();
        
    protected:
        SVLockPtr m_uiLock;
        SVNodePtr m_pUIRoot;
    };
    
}//!namespace




#endif //SV_SCENEMGR_H
