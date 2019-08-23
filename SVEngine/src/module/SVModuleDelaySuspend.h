//
// SVModuleDelaySuspend.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MODULEDELAYSUSPEND_H
#define SV_MODULEDELAYSUSPEND_H

#include "SVModuleBase.h"

namespace sv{
    
    class SVModuleDelaySuspend : public SVModuleBase {
    public:
        SVModuleDelaySuspend(SVInst* _app);
        
        virtual ~SVModuleDelaySuspend();
        
        virtual void init();
        
        virtual void destroy();
        
        virtual void update(f32 _dt);
        
        virtual void open();
        
        virtual void close();
        
        virtual bool isOpen();
        
        void setModuleName(cptr8 _name){ m_module_name = _name;}
        
        cptr8 getModuleName() { return m_module_name.c_str() ;}
        
        void setAccTime(f32 _time);
        
    protected:
        f32 m_accTime;
        
        bool m_isOpen;
        
        cb_func_op m_cb;
        
        SVString m_module_name;
    };
    
}//!SV_MODULEDELAYSUSPEND_H sv





#endif //SV_MODULESYS_H
