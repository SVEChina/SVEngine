//
// SVFaceDataMesh.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_FACEMESHDATA_H
#define SV_FACEMESHDATA_H

#include "../base/SVGBase.h"
#include "../base/SVMap.h"

namespace sv {
    
    struct StanderFace{
        s32 facepoints_count;
        s32 face_top = 0;
        s32 face_left = 0;
        s32 face_bottom = 0;
        s32 face_right = 0;
        f32 face_yaw = 0;
        f32 face_roll = 0;
        f32 face_pitch = 0;
        s32 design_img_width;
        s32 design_img_height;
        f32 points[800];
    };
    
    struct FaceMesh{
        s32 tricount;
        s32 vertexcount;
        s32 triangles[1200];
    };
    
    typedef SVMap<cptr8, StanderFace*> STANDERFACEPOOL;
    typedef SVMap<cptr8, FaceMesh*> FACEMESHPOOL;
    
    class SVFaceDataMesh : public SVGBase{
    public:
        SVFaceDataMesh(SVInst *_app);
        
        ~SVFaceDataMesh();
        //
        void init();
        //
        void destory();
        //
        StanderFace* getStanderFace(cptr8 _name);
        
        FaceMesh* getFaceMesh(cptr8 _name);
        
    protected:
        void _loadStanderFaceData(cptr8 _name);
        
        void _loadFaceMeshData(cptr8 _name);
        
        void _clearFaceMeshPool();
        
        void _clearStanderFacePool();

        STANDERFACEPOOL m_standerfacepool;
        
        FACEMESHPOOL m_facemeshpool;
    };
};

#endif /* SV_FACEMESHDATA_H */
