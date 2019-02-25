//
// SVMtl3D.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTL_3D_H
#define SV_MTL_3D_H


#include "SVMtlCore.h"

//3D材质
namespace sv {
    
    namespace mtl{
        
        class SVMtl3D : public SVMtlCore {
        public:
            SVMtl3D(SVInst *_app, cptr8 _shader);
            
            SVMtl3D(SVMtl3D *_mtl);
            
            ~SVMtl3D();
            
            virtual void reset();
            
            virtual SVMtlCorePtr clone();
            
            void setAmbientLightIntensit(f32 _intersit);
            
            void setAmbientLightColorFactor(FVec3& _ambientColor);
            
            f32 getAmbientLightStrength();
            
            FVec3& getAmbientLightColor();
            
            void setLightPositon(FVec3& _lightPos);
            
            void setLightColor(FVec3& _lightColor);
            
            FVec3& getLightColor();
            
            FVec3& getLightPos();
        protected:
            void _submitMtl(SVRendererBasePtr _render);
            f32   m_ambientStrength;
            FVec3 m_ambient_color;
            FVec3 m_lightPos;
            FVec3 m_lightColor;
        public:
            virtual void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator,
                                RAPIDJSON_NAMESPACE::Value &_objValue);
            
            virtual void fromJSON(RAPIDJSON_NAMESPACE::Value &item);
        };

    }//!namespace mtl
    
}//!namespace sv


#endif //SV_MTL_3D_H
