//
// SVEffectPackage.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_EFFECTPACKAGE_H
#define SV_EFFECTPACKAGE_H


#include "SVModuleBase.h"
namespace sv{
    
    class SVEffectPackage : public SVModuleBase {
        
    public:
        SVEffectPackage(SVInst* _app);
        
        ~SVEffectPackage();
        
        virtual void init();
        
        virtual  void destroy();
        
        virtual void update(f32 _dt);
        
        virtual bool procEvent(SVEventPtr _event);
        
        void addNode(SVNodePtr _nodePtr);
        
        bool hasNode(SVNodePtr _nodePtr);
        
        bool rmoveNode(SVNodePtr _nodePtr);
        
        bool removeNode(cptr8 _name);
        
        SVNodePtr getNode(cptr8 _name);
        
    protected:
        typedef SVArray<SVNodePtr> SVNODEPOOL;
        SVNODEPOOL m_nodePool;
        
        SVLockPtr m_lock;
    };
    
    
}//!namespace sv

#endif /* SV_EFFECTPACKAGE_H */
