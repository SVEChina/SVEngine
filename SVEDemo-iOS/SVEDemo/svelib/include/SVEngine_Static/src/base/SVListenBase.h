//
// Created by yizhou Fu on 2017/3/16.
//

#ifndef SV_LISTENBASE_H
#define SV_LISTENBASE_H

#include "SVGBase.h"

namespace sv {
    
    //监听函数池
    struct LISTENFUNC {
        SVObject *listener;
        dataListenFunc func;
    };
    
    class SVListenBase : public SVGBase {
    public:
        SVListenBase(SVInst* _app);
        
        ~SVListenBase();
        
        //类函数指针监听
        void registListenFunc(SVObject *listener, dataListenFunc func);
        
        void unregistListenFunc(SVObject *listener, dataListenFunc func);
        
        void noticeListener(float dt);
        
    protected:
        virtual void _updateSelf(float dt);
        
        virtual void _notice(float dt);
        
        typedef SVArray<LISTENFUNC> LISTENFUNCPOOL;
        LISTENFUNCPOOL m_listenfunc_pool;
        SVLockPtr m_listenlock;
        bool m_listenDirty;
    };

}//!namespace sv


#endif //SV_LISTENBASE_H
