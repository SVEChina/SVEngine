//
// SVMtlColorSelect.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#ifndef SV_MTL_COLORSELECT_H
#define SV_MTL_COLORSELECT_H

#include "SVMtlCore.h"
#include "../base/SVColor.h"

namespace sv {
    
    namespace mtl{
        
        class SVMtlColorSelect2d : public SVMtlCore {
        public:
            SVMtlColorSelect2d(SVInst *_app);
            
            SVMtlColorSelect2d(SVMtlColorSelect2d* _mtl);
            
            virtual SVMtlCorePtr clone();

            void _submitMtl();
            
            void setColor(u32 colorid);
            
        private:
            SVColor mColor;
        };
        
        class SVMtlColorSelect2dSpine : public SVMtlColorSelect2d {
        public:
            SVMtlColorSelect2dSpine(SVInst *_app);
            
            SVMtlColorSelect2dSpine(SVMtlColorSelect2dSpine* _mtl);
            
            virtual SVMtlCorePtr clone();
        };
        
        class SVMtlColorSelect3d : public SVMtlColorSelect2d {
        public:
            SVMtlColorSelect3d(SVInst *_app);
            
            SVMtlColorSelect3d(SVMtlColorSelect3d* _mtl);
            
            virtual SVMtlCorePtr clone();
        };
        
        class SVMtlColorLine : public SVMtlColorSelect2d {
        public:
            SVMtlColorLine(SVInst *_app);
            
            SVMtlColorLine(SVMtlColorLine* _mtl);
            
            virtual SVMtlCorePtr clone();
        };
        
    }//!namespace mtl
    
}//!namespace sv




#endif //SV_MTL_COLORSELECT_H
