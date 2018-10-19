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
        
        class SVFairDataBlur : public SVFilterBase {
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
            
            inline void setSmooth(f32 _smooth) {
                m_smooth = _smooth;
            }
            
            inline f32 getSmooth() {
                return m_smooth;
            }
            
        protected:
            f32 m_smooth;
            
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
