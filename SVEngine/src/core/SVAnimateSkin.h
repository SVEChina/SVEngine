//
// SVAnimateSkin.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_ANIMATE_SKIN_H
#define SV_ANIMATE_SKIN_H

#include "../base/SVGBase.h"
#include "../base/SVVec3.h"
#include "../base/SVMat4.h"
#include "../base/svstr.h"
#include "../base/SVMap.h"

namespace sv {
    
    namespace util{
        
        /*
        骨头
         */
        class SVBone :public SVObject {
        public:
            s32 m_id;
            
            FMat4 m_matrix;
            
            SVBone* m_pParent;
            
            typedef SVArray<SVBone*> BONEPOOL;
            BONEPOOL m_children;
        };

        /*
         蒙皮动画
         */
        class SVAnimateSkin : public SVGBase{
        public:
            SVAnimateSkin(SVInst* _app,cptr8 _name);

            ~SVAnimateSkin();
            
            cptr8 getName();
            
        protected:
            SVString m_name;
            //骨架
            //SVBonePtr m_ske;
            //
            
        };
        
        //骨架池
        class SVSkinMgr : public SVGBase{
        public:
            SVSkinMgr(SVInst* _app);
            
            ~SVSkinMgr();
            
            void addSkin(SVAnimateSkinPtr _skin);
            
            void remvoeSkin(cptr8 _name);
            
        protected:
            typedef SVMap<SVString,SVAnimateSkinPtr> SKINPOOL;
            SKINPOOL m_skinPool;
        };

    }//namespace util
    
}//!namespace sv


#endif //SV_ANIMATE_SKIN_H
