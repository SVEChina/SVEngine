//
// SVModel.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MODEL_H
#define SV_MODEL_H

#include "../base/SVGBase.h"
#include "../base/SVPreDeclare.h"
#include "../base/SVBounds.h"
#include "../base/SVArray.h"

namespace sv {
    
    namespace util {

        /* 模型 由多个mesh构成 */
        class SVModel : public SVGBase {
        public:
            SVModel(SVInst* _app);
            
            ~SVModel();
            
            void clear(){};
            
            void addMesh(SVMeshPtr _mesh);
        
            SVMeshPtr getMesh(s32 _index);
            
            void clearMesh();
            
            SVBoundBox getBox();
            
            cptr8 getName();
            
            void setName(cptr8 _name);
            
            void update(f32 _dt,FMat4& _mat);
            
            void render();
            
            void bindSke(SVSkeletonPtr _ske);
            
            void unbindSke();
            
        protected:
            SVString m_name;
            SVBoundBox m_box;
            //一个模型包含多个mesh
            typedef SVArray<SVMeshPtr> MESHPOOL;
            MESHPOOL m_meshPool;
        };
        
    }//!namespace util
    
}//!namespace sv



#endif //SV_MODEL_H
