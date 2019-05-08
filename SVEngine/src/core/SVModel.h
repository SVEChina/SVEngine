//
// SVModel.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MODEL_H
#define SV_MODEL_H

#include "../base/SVObject.h"
#include "../base/SVPreDeclare.h"
#include "SVMesh.h"
#include "../base/SVBounds.h"

namespace sv {
    
    namespace util {

        class SVModel : public SVObject {
        public:
            SVModel();
            
            ~SVModel();
            
            void clear(){};
            
            void setMesh(SVMeshPtr _mesh);
        
            SVMeshPtr getMesh();
            
            void clearMesh();
            
            SVBoundBox getBox();
            
            cptr8 getName();
            
            void setName(cptr8 _name);
            
        protected:
            SVString m_name;
            SVBoundBox m_box;
            SVMeshPtr m_pMesh;
        };
        
        //模型池
        class SVModelMgr : public SVGBase{
        public:
            SVModelMgr(SVInst* _app);
            
            ~SVModelMgr();
            
        };
        
    }//!namespace util
    
}//!namespace sv



#endif //SV_MODEL_H
