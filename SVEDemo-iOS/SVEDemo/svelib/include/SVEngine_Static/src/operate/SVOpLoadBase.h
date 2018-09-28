//
// SVOpLoadBase.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_OPLOADBASE_H
#define SV_OPLOADBASE_H


#include "SVOpBase.h"

namespace sv {
    
    //
    class SVOpLoadBase : public SVOpBase {
    public:
        SVOpLoadBase(SVInst *_app);
        ~SVOpLoadBase();
    };
    
    //回收节点
    class SVOpRecycleNode : public SVOpBase {
    public:
        SVOpRecycleNode(SVInst *_app, SVNodePtr _node);
        
    protected:
        void _process(f32 dt);
        
        SVNodePtr m_node;
    };
    
    //拾取包
    class SVOpPick : public SVOpBase {
    public:
        SVOpPick(SVInst *_app, s32 _x,s32 _y);
        
        ~SVOpPick();
        
    protected:
        void _process(f32 dt);
        s32 m_ui_x;
        s32 m_ui_y;
    };
    
}//!namespace sv



#endif //SV_OPLOADBASE_H
