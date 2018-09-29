//
// SVGeoGen.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_GEOMETRYBASE_H
#define SV_GEOMETRYBASE_H

//基本图元
#include "SVVertDef.h"
#include "../base/SVBounds.h"
#include "../rendercore/SVRenderDeclare.h"

namespace sv {
    
    class SVGeoGen {
    public:
        //三角形
        static SVRenderMeshPtr genTri(SVInst* _app,f32 _len,SVBoundBox& _aabb);
        
        //方形
        static SVRenderMeshPtr genRect(SVInst* _app,f32 _w,f32 _h,f32 _inx,f32 _iny,f32 _inw,f32 _inh,SVBoundBox& _aabb);
        
        //菱形
        static SVRenderMeshPtr genLing(SVInst* _app,f32 _len,SVBoundBox& _aabb);
        
        //五边
        static SVRenderMeshPtr genFiveEdage(SVInst* _app,f32 _len,SVBoundBox& _aabb);
        
        //六边
        static SVRenderMeshPtr genSixEdage(SVInst* _app,f32 _len,SVBoundBox& _aabb);
        
        //圆形
        static SVRenderMeshPtr genCircle(SVInst* _app,f32 _len,SVBoundBox& _aabb);
        
        //AABB盒
        static SVRenderMeshPtr genAABB(SVInst* _app,SVBoundBox& _aabb);
        
        //创建矩形mesh
        static SVRenderMeshPtr createRectMesh(SVInst* _app,f32 _w ,f32 _h , s32 _wPointCount , s32 _hPointCont);
        
        //
        static SVRenderMeshPtr createNetGrid(SVInst* _app,s32 _num,f32 _unit);
        
    private:
        static SVRenderMeshPtr _getPolygonDiy(SVInst* _app,s32 _edagenum,f32 _startangle,f32 _len,SVBoundBox& _aabb);
    };
    
}



#endif //SV_GEOMETRYBASE_H
