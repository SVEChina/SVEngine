//
// SV2DFaceMaskSTNode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_2DFACEMASK_ST_NODE_H
#define SV_2DFACEMASK_ST_NODE_H

#include "SV2DFaceMaskNode.h"


namespace sv {
    
    namespace node{
        
        //sensetime
        class SVSTPointExt {
        public:
            static void st_foreHeadPointExtWithFaceLandMark(f32 *faceLandMark, s32 &markIndex);
            
            static void st_lipsPointExtWithFaceLandMark(f32 *faceLandMark, s32 &markIndex);
            
            static void st_faceOutlinePointExtWithFaceLandMark(f32 *faceLandMark, s32 &markIndex);
        };
        
        class SV2DFaceMaskSTNode : public SV2DFaceMaskNode {
        public:
            SV2DFaceMaskSTNode(SVInst *_app);
            
            ~SV2DFaceMaskSTNode();
            
            void update(f32 dt);
            
            void render();
            
        protected:
            void _updateVerts();
            
            //序列化接口
            void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue);
            
            void fromJSON(RAPIDJSON_NAMESPACE::Value &item);
        };
        
    }//!namespace node
    
}//!namespace sv


#endif //SV_2DFACEMASK_ST_NODE_H
