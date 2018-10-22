//
// SVFaceBeautyShowFilter.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_FACEBEAUTYSHOWFILTER_H
#define SV_FACEBEAUTYSHOWFILTER_H


#include "SVFilterBase.h"
#include "../../mtl/SVMtlDeclare.h"
#include "../../node/SVNodeDeclare.h"

namespace sv {

    namespace logic {
        
        class SVGenFBParam : public SVObject {
            
            public :
            
            SVGenFBParam(){
                reset();
            };
            
            f32 m_smooth;
            f32 m_param01;
            f32 m_param02;
            f32 m_param03;
            f32 m_param04;
            f32 m_param05;
            f32 m_param06;
            
            void reset(){
                f32 m_smooth = 0.0;
                f32 m_param01 = 0.0;
                f32 m_param02 = 0.0;
                f32 m_param03 = 0.0;
                f32 m_param04 = 0.0;
                f32 m_param05 = 0.0;
                f32 m_param06 = 0.0;
            }
            
            void copy(SVGenFBParamPtr _p){
                 m_smooth = _p->m_smooth;
                 m_param01 = _p->m_param01;
                 m_param02 = _p->m_param02;
                 m_param03 = _p->m_param03;
                 m_param04 = _p->m_param04;
                 m_param05 = _p->m_param05;
                 m_param06 = _p->m_param06;
            }
            
        };
        
        class SVFaceBeautyBase  : public SVFilterBase{
            
        public:
            
            SVFaceBeautyBase(SVInst *_app);
            
            ~SVFaceBeautyBase();
            
            virtual bool create();
            
            virtual void destroy();
            
            virtual void update(f32 dt);
            
            virtual void setFilterParam(f32 _smooth,SVFILTERITEMTYPE _type);
            
            virtual f32 getFilterParam(SVFILTERITEMTYPE _type);
            
            virtual void refreshData(SVGenFBParamPtr _param);
            
            void setSmooth(f32 _smooth);
            
            void setParam01(f32 _smooth);
            
            void setParam02(f32 _smooth);
            
            void setParam03(f32 _smooth);
            
            void setParam04(f32 _smooth);
            
            void setParam05(f32 _smooth);
            
            void setParam06(f32 _smooth);
            
        protected:
            
            SVGenFBParamPtr m_Param;
            
        };
        
        class SVFairDataBlur : public SVFaceBeautyBase {
        public:
            SVFairDataBlur(SVInst *_app);
            
            ~SVFairDataBlur();
            
            virtual bool create();
            
            virtual void destroy();
            
            virtual void update(f32 dt);
            
            virtual void setFilterParam(f32 _smooth,SVFILTERITEMTYPE _type);
            
            virtual f32 getFilterParam(SVFILTERITEMTYPE _type);
            
            SVMtlCorePtr m_mtl_a;
            SVMtlCorePtr m_mtl_b;
            SVMtlCorePtr m_mtl_c;
            SVMtlBlurFairPtr m_mtl_d;
            SVMtlCorePtr m_mtl_e;
            SVMtlCorePtr m_mtl_f;
            SVMtlCorePtr m_mtl_back;
            
        };
        
        class SVFairLtraLow : public SVFilterBase {
        public:
            SVFairLtraLow(SVInst *_app);
            
            ~SVFairLtraLow();
            
            virtual  bool create();
            
            virtual void destroy();
            
            virtual  void update(f32 dt);
            
            virtual void setFilterParam(f32 _smooth,SVFILTERITEMTYPE _type);
            
            virtual f32 getFilterParam(SVFILTERITEMTYPE _type);
            
            SVMtlCorePtr m_mtl_a;
            SVMtlCorePtr m_mtl_back;

            
            inline void setSmooth(f32 _smooth) {
                m_smooth = _smooth;
            }
            
            inline f32 getSmooth() {
               return m_smooth;
            }
            
        protected:
            f32 m_smooth;
            
        };
    }
}

#endif /* SV_FACEBEAUTYSHOWFILTER_H */
