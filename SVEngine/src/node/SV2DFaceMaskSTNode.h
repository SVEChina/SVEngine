//
// SV2DFaceMaskSTNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_2DFACEMASK_ST_NODE_H
#define SV_2DFACEMASK_ST_NODE_H

#include "SV2DFaceMaskNode.h"
//sensetime
class SVSTPointExt {
public:
    static void st_foreHeadPointExtWithFaceLandMark(f32 *faceLandMark, s32 &markIndex);
    
    static void st_lipsPointExtWithFaceLandMark(f32 *faceLandMark, s32 &markIndex);
    
    static void st_faceOutlinePointExtWithFaceLandMark(f32 *faceLandMark, s32 &markIndex);
};

namespace sv {
    
    namespace node{
        class SV2DFaceMaskSTNode : public SV2DFaceMaskNode {
        public:
            SV2DFaceMaskSTNode(SVInst *_app);
            
            ~SV2DFaceMaskSTNode();
            
            void update(f32 dt);
            
            void render();
        protected:
            void _updateVerts();
            
        };
        
    }//!namespace node
    
}//!namespace sv


#endif //SV_2DFACEMASK_ST_NODE_H
