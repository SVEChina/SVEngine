//
// SVMtlFair1.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTL_FAIR1_H
#define SV_MTL_FAIR1_H

#include "SVMtlCore.h"

namespace sv {
    
    namespace mtl{
        
        //
        class SVMtlFilterBrilaterl : public SVMtlCore {
        public:
            SVMtlFilterBrilaterl(SVInst *_app);
            
            SVMtlFilterBrilaterl(SVMtlFilterBrilaterl *_mtl);
            
            ~SVMtlFilterBrilaterl();
            
            virtual SVMtlCorePtr clone();
            
            void setOffset(f32 _offw,f32 _offh);
            
            void setDistance(f32 _dis);
            
        protected:
            virtual void _submitMtl(SVRendererPtr _render); 

            f32 m_texelWidthOffset;
            f32 m_texelHeightOffset;
            f32 m_distanceNormalizationFactor;
            
            bool m_paramDirty;
        };
        
        //
        class SVMtlFilterBrilaterl2 : public SVMtlCore {
        public:
            SVMtlFilterBrilaterl2(SVInst *_app);
            
            SVMtlFilterBrilaterl2(SVMtlFilterBrilaterl2 *_mtl);
            
            ~SVMtlFilterBrilaterl2();
            
            virtual SVMtlCorePtr clone();
            
            void setOffset(f32 _offw,f32 _offh);
            
            void setDistance(f32 _dis);
            
        protected:
            virtual void _submitMtl(SVRendererPtr _render); 
            
            f32 m_texelWidthOffset;
            f32 m_texelHeightOffset;
            f32 m_distanceNormalizationFactor;
            
            bool m_paramDirty;
        };
        
        //
        class SVMtlGaussian : public SVMtlCore {
        public:
            SVMtlGaussian(SVInst *_app);
            
            SVMtlGaussian(SVMtlGaussian *_mtl);
            
            ~SVMtlGaussian();
            
            virtual SVMtlCorePtr clone();
            
            void setRadius(f32 _radius);
            
        protected:
            virtual void _submitMtl(SVRendererPtr _render);
            //
            f32 m_radius;
            //
            bool m_paramDirty;
        };

        class SVMtlBlur : public SVMtlCore {
        public:
            SVMtlBlur(SVInst *_app);
            
            SVMtlBlur(SVMtlBlur *_mtl);

            ~SVMtlBlur();
            
            virtual SVMtlCorePtr clone();

            void setRadius(f32 _radius);

        protected:
            virtual void _submitMtl(SVRendererPtr _render);

            f32 m_radius;
            //
            bool m_paramDirty;
        };
        
        class SVMtlBlurFair : public SVMtlCore {
        public:
            SVMtlBlurFair(SVInst *_app);
            
            SVMtlBlurFair(SVMtlBlurFair *_mtl);
            
            ~SVMtlBlurFair();
            
            void setSmooth(f32 _smooth);
            
            virtual void _submitMtl(SVRendererPtr _render); 
            
            virtual SVMtlCorePtr clone();
            
        protected:
            f32 m_blurAlpha;
        };
        
        class SVMtlFairLtraLow : public SVMtlCore {
        public:
            SVMtlFairLtraLow(SVInst *_app);
            
            SVMtlFairLtraLow(SVMtlFairLtraLow *_mtl);
            
            ~SVMtlFairLtraLow();
            
            void setSmooth(f32 _smooth);
            
            virtual void _submitMtl(SVRendererPtr _render); 
            
            virtual SVMtlCorePtr clone();
            
        protected:
            f32 m_smooth;
        };
        
        class SVMtlBlurFairGus : public SVMtlCore {
        public:
            SVMtlBlurFairGus(SVInst *_app);
            
            SVMtlBlurFairGus(SVMtlBlurFairGus *_mtl);
            
            ~SVMtlBlurFairGus();
            
            void setSmooth(f32 _smooth);
            
            virtual void _submitMtl(SVRendererPtr _render);
            
            virtual SVMtlCorePtr clone();
            
        protected:
            f32 m_smooth;
        };
        
        
    }//!namespace mtl
    
}//!namespace sv




#endif //SV_MTL_FAIR1_H
