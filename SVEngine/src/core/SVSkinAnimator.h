//
// SVSkinAnimator.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_SKINANIMATOR_H
#define SV_SKINANIMATOR_H

#include "../base/SVObject.h"

namespace sv {
    
    namespace util {
        
        //动画数据
        class SVAniData :public SVObject {
        public:
            
        };
        
        //骨架数据
        class SVSkeData :public SVObject {
        public:
            
        };
        
        //存放动画数据 与 骨骼绑定中
        class SVSkinAnimator :public SVObject {
        public:
            SVSkinAnimator();
            
            ~SVSkinAnimator();
            
            //各个动画通道做差值
            void update(f32 _dt);
            
            //播放指定动画名称
            bool play(cptr8 _name);
            
            //继续播放
            bool play();
            
            //暂停
            bool pause();
            
            //停止动画
            bool stop();
            
            //增加一套动画数据
            void addAnimate(cptr8 _name);
            
            //删除一套动画数据
            void delAnimate(cptr8 _name);
            
            //清理动画
            void clearAnimates();
            
        protected:
            //SVSkeDataPtr m_pSkeData;
            
        };

        //
    }
}

#endif //SV_SKINANIMATOR_H
