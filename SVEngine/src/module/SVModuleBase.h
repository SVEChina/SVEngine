//
// SVModuleBase.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MODULEBASE_H
#define SV_MODULEBASE_H

#include "SVModuleDef.h"
#include "../event/SVEventProc.h"
#include "../base/SVDef.h"
#include "../node/SVNodeDeclare.h"

namespace sv{
    
    class SVModuleBase : public SVEventProc {
    public:
        SVModuleBase(SVInst* _app);
        
        virtual ~SVModuleBase();
        
        virtual void init();
        
        virtual void destroy();
        
        virtual void update(f32 _dt);
        
        virtual void open();
        
        virtual void close();
        
        virtual bool isOpen();
        
        virtual void setOpCallBack(cb_func_op _cb, void *_obj);
        
        void setModuleName(cptr8 _name){ m_module_name = _name;}
        
        cptr8 getModuleName() { return m_module_name.c_str() ;}
        
    protected:
        bool m_isOpen;
        
        cb_func_op m_cb;
        
        void *m_obj;
        
        SVString m_module_name;
    };
    
}//!namespace sv





#endif //SV_MODULESYS_H
