//
// SVGlobalParam.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_GLOBALPARAM_H
#define SV_GLOBALPARAM_H

#include "../base/SVGBase.h"
#include "../base/SVMat4d.h"
#include "../base/SVVec4.h"
#include "../base/SVRandom.h"
#include "../rendercore/SVRenderDeclare.h"
#include "../node/SVNodeDeclare.h"

namespace sv {

    class SVGlobalParam: public SVGBase {
    public:
        SVGlobalParam(SVInst* _app);
        
        ~SVGlobalParam();
        
        void clear();
        
        void setSeed(u32 seed);
        
        u32 getSeed() const;
        
        u32 getRandom();
        
        s32 getRandomInt(s32 from,s32 to);
        
        f32 getRandomFloat(f32 from,f32 to);
        
        f64 getRandomDouble(f64 from,f64 to) ;
        
        //当前场景
        SVScenePtr m_curScene;
        
        //内部宽高
        s32 m_inner_width;
        
        s32 m_inner_height;
        
        //UI矩阵
        FMat4 m_mat_ui;
        //
        SVRandom m_random;
        //获取引擎内核
        SV_ENGCORE getEngCore(){ return m_engCore; };
    
    protected:
        SV_ENGCORE m_engCore;   //引擎内核
        
    public:
        FVec4 diffuse_color;    //漫反射颜色
        f32 diffuse_scale;      //满反射颜色缩放值
    };
}//!namespace sv




#endif //SV_GLOBALPARAM_H
