//
// SVPictureProcess.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PICPROC_H
#define SV_PICPROC_H

#include "../base/SVGBase.h"
#include "filter/SVFilterDef.h"
#include "../mtl/SVMtlDeclare.h"

namespace sv {

    namespace logic {
        
        //图片处理
        class SVPictureProcess : public SVGBase {
        public:
            SVPictureProcess(SVInst *_app);
            
            ~SVPictureProcess();
            //
            void init();
            //
            void destroy();
            //更新
            void update(f32 _dt);
            //higher
            void openFair();
            //meidump (u)
            void openFairlow();
            //meidump (u)
            void openFairBlur();
            //lowerer (mg)
            void openFairultraLow();
            //关闭美颜
            void clearFair();
            //曲线
            void openToneCurverRGB();
            //
            void clearToneCurverRGB();
            //滤镜
            void openBasedOnFilter();
            //
            void clearBasedOnFilter();
            //设置滤镜参数
            void setFilterParam(f32 _smooth,SVFILTERITEMTYPE _type);
            //获取滤镜参数
            f32 getFilterParam(SVFILTERITEMTYPE _type);
            //设置滤镜数据
            void setFilterBSplineData(ptru8 _data);
            //
        protected:
            void _addNodeMainTex(SVMultPassNodePtr _toneCurverNode,SVTexturePtr _tex);
            
            SVFairDataPtr m_fair1;

            SVFairDataLowsPtr m_fairLow;
            
            SVFairDataBlurPtr m_fairBlur;
            
            SVFairLtraLowPtr  m_fairLtraLow;

            SVBasedonFilterPtr m_baseonfilter;
            
            SVRGBToneCurveFilterPtr m_toneCurverFilter;
        };
        
    }//!namespace logic

}//!namespace sv



#endif //SV_PICPROC_H
