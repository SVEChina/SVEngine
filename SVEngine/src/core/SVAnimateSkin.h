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
#include "../base/SVVec4.h"
#include "../base/SVMat4.h"
#include "../base/svstr.h"
#include "../base/SVMap.h"

namespace sv {
    
    namespace util{
        
        //骨头
        class SVBone :public SVObject {
        public:
            SVBone();
            
            SVString m_name;
            s32 m_id;
            FVec3 m_tran;
            FVec3 m_scale;
            FVec4 m_rot;
            SVBonePtr m_pParent;
            //
            typedef SVArray<SVBonePtr> BONEPOOL;
            BONEPOOL m_children;
            //
        public:
            void clear();
            
            void update();
        };
        
        
        //骨架
        class SVSkeleton :public SVObject {
        public:
            SVSkeleton();
            
            void addBone(SVBonePtr _bone); //需要单独对SVLoaderGLTF开放接口 **fuyizhou
            
            SVString m_name;
            
            SVBonePtr m_root;
            
        protected:
            typedef SVArray<SVBonePtr> BONEPOOL;
            BONEPOOL m_boneArray;
        
        public:
            void destroy();
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
            //基础矩阵
           FMat4 m_inverseBindMatrices;
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
