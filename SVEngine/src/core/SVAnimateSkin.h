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
#include "../base/SVQuat.h"
#include "../base/svstr.h"
#include "../base/SVMap.h"

namespace sv {
    
    namespace util{
        
        enum SVChanType {
            E_CN_T_TRANS = 0,
            E_CN_T_SCALE = 1,
            E_CN_T_ROT = 2,
            E_CN_T_WEIGHT = 3
        };
        
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
            
            SVBonePtr getBone(s32 _index);
            
            SVString m_name;
            
            SVBonePtr m_root;
            
        protected:
            typedef SVArray<SVBonePtr> BONEPOOL;
            BONEPOOL m_boneArray;
        
        public:
            void destroy();
        };
        
        /*
         数据
         */
        
        class SVSkinAniData : public SVObject{
        public:
            SVSkinAniData() {}
            ~SVSkinAniData(){}
            //
            typedef SVArray<f32> DATAVEC;
            DATAVEC m_datavec;
        };
        
        /*
         轨道
         */
        
        class SVChannel : public SVObject{
        public:
            void update(f32 _dt);
            
            s32 m_input;        //输入数据id
            s32 m_output;       //输出数据id
            s32 m_target;       //目标id
            s32 m_type;         //数据类型 scale rotation translation
            s32 m_intertype;    //插值方式 linear
        };
        
        /*
         蒙皮动画 单个动画名称
         */
        class SVAnimateSkin : public SVGBase{
        public:
            SVAnimateSkin(SVInst* _app,cptr8 _name);

            ~SVAnimateSkin();
            
            void update(f32 _dt);
            
            void bind(SVSkeletonPtr _ske);
            
            void unbind();

            cptr8 getName();
            
            void addChannel(SVChannelPtr _chan);
            
            void clearChannel();
            
            void addSkinAniData(s32 _index,SVSkinAniDataPtr _obj);
            
            bool hadSkinAniData(s32 _index);
            
        protected:
            SVString m_name;
            //关联骨架
            SVSkeletonPtr m_pSke;
            //
            f32 m_accTime;
            f32 m_totalTime;
            //动画数据
            typedef SVMap<s32,SVSkinAniDataPtr> DATAPOOL;
            DATAPOOL m_dataPool;
            //数据关系
            typedef SVArray<SVChannelPtr> CHNPOOL;
            CHNPOOL m_chnPool;
            //
        private:
            FVec3 _lerp_trans(s32 _mod,f32 _timepre,f32 _timenxt,f32 _timecur,FVec3 _pos1,FVec3 _pos2);
            FVec3 _lerp_scale(s32 _mod,f32 _timepre,f32 _timenxt,f32 _timecur,FVec3 _scale1,FVec3 _scale2);
            FVec4 _lerp_rot(s32 _mod,f32 _timepre,f32 _timenxt,f32 _timecur,FVec4 _rot1,FVec4 _rot2);
            f32 _lerp_weights();
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
