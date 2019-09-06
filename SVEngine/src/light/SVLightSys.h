//
// SVUIMgr.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_LIGHTSYS_H
#define SV_LIGHTSYS_H

#include "../basesys/SVSysBase.h"
#include "../base/SVVec3.h"

namespace sv {
    
    class SVLightSys : public SVSysBase {
    public:
        SVLightSys(SVInst *_app);
        
        ~SVLightSys();
        
        void init();
        
        void destroy();
        
        void update(f32 dt);
        
    public:
        FVec3 m_g_ambient;      //全局环境光
        FVec3 m_g_diffuse;      //全局漫反射
        FVec3 m_g_specular;     //全局高光
        FVec3 m_g_direction;    //光向
        f32 m_g_diffuse_sc;     //满反射颜色缩放值
    };

}//!namespace sv


#endif //SV_LIGHTSYS_H
