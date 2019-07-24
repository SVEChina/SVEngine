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
#include "../base/SVBounds.h"
#include "../base/SVArray.h"
#include "SVMesh.h"

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
            
            void update(f32 _dt);
            
            void render();
            
        protected:
            SVString m_name;
            SVBoundBox m_box;
            //一个模型包含多个mesh
            typedef SVArray<SVMeshPtr> MESHPOOL;
            MESHPOOL m_meshPool;
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
