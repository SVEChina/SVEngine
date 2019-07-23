//
// SVStaticData.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVStaticData.h"
#include "SVFaceDataMesh.h"
#include "../app/SVInst.h"
#include "../base/SVDataSwap.h"
#include "../rendercore/SVRenderMesh.h"
#include "../rendercore/SVRenderCmd.h"
#include "../rendercore/SVRenderMgr.h"

V3_C_T0 SVStaticData::m_baseRect[6];

SVStaticData::SVStaticData(SVInst* _app)
:SVGBase(_app) {
    m_screenMesh = nullptr;
    m_screenMeshRot90 = nullptr;
    m_screenMeshRot180 = nullptr;
    m_screenMeshRot270 = nullptr;
    m_screenLTMesh = nullptr;
    m_screenLBMesh = nullptr;
    m_screenRTMesh = nullptr;
    m_screenRBMesh = nullptr;
    m_screenTwoDivisionMesh = nullptr;
    m_screenFourDivisionMesh = nullptr;
    m_screenFourXDivisionMesh = nullptr;
    m_faceDataMesh = nullptr;
    init();
    //
}

SVStaticData::~SVStaticData() {
    destroy();
}

void SVStaticData::init() {
    //
    _initBaseRect();
    //
    m_screenMesh        = MakeSharedPtr<SVRenderMesh>(mApp);
    m_screenLTMesh      = MakeSharedPtr<SVRenderMesh>(mApp);
    m_screenLBMesh      = MakeSharedPtr<SVRenderMesh>(mApp);
    m_screenRTMesh      = MakeSharedPtr<SVRenderMesh>(mApp);
    m_screenRBMesh      = MakeSharedPtr<SVRenderMesh>(mApp);
    m_screenMeshRot90   = MakeSharedPtr<SVRenderMesh>(mApp);
    m_screenMeshRot180  = MakeSharedPtr<SVRenderMesh>(mApp);
    m_screenMeshRot270  = MakeSharedPtr<SVRenderMesh>(mApp);
    //构建屏幕渲染Mesh
    m_screenMesh->setVertexType(E_VF_V2_C_T0);
    m_screenLTMesh->setVertexType(E_VF_V2_C_T0);
    m_screenLBMesh->setVertexType(E_VF_V2_C_T0);
    m_screenRTMesh->setVertexType(E_VF_V2_C_T0);
    m_screenRBMesh->setVertexType(E_VF_V2_C_T0);
    m_screenMeshRot90->setVertexType(E_VF_V2_C_T0);
    m_screenMeshRot180->setVertexType(E_VF_V2_C_T0);
    m_screenMeshRot270->setVertexType(E_VF_V2_C_T0);
    //索引数据
    u16 t_index[6] = {0, 1, 2, 2, 1, 3};
    SVDataSwapPtr t_pIndexData = MakeSharedPtr<SVDataSwap>();
    t_pIndexData->writeData(t_index,sizeof(u16)*6);
    m_screenMesh->setIndexData(t_pIndexData,6);
    m_screenLTMesh->setIndexData(t_pIndexData,6);
    m_screenLBMesh->setIndexData(t_pIndexData,6);
    m_screenRTMesh->setIndexData(t_pIndexData,6);
    m_screenRBMesh->setIndexData(t_pIndexData,6);
    m_screenMeshRot90->setIndexData(t_pIndexData,6);
    m_screenMeshRot180->setIndexData(t_pIndexData,6);
    m_screenMeshRot270->setIndexData(t_pIndexData,6);
    //
    SVDataSwapPtr t_pMeshData = MakeSharedPtr<SVDataSwap>();
    SVDataSwapPtr t_pLTMeshData = MakeSharedPtr<SVDataSwap>();
    SVDataSwapPtr t_pLBMeshData = MakeSharedPtr<SVDataSwap>();
    SVDataSwapPtr t_pRTMeshData = MakeSharedPtr<SVDataSwap>();
    SVDataSwapPtr t_pRBMeshData = MakeSharedPtr<SVDataSwap>();
    SVDataSwapPtr t_pMeshData90 = MakeSharedPtr<SVDataSwap>();
    SVDataSwapPtr t_pMeshData180 = MakeSharedPtr<SVDataSwap>();
    SVDataSwapPtr t_pMeshData270 = MakeSharedPtr<SVDataSwap>();
    
    //渲染数据
    V2_C_T0 VerData[4];
    //颜色
    VerData[0].r = 255;
    VerData[0].g = 255;
    VerData[0].b = 255;
    VerData[0].a = 255;
    VerData[1].r = 255;
    VerData[1].g = 255;
    VerData[1].b = 255;
    VerData[1].a = 255;
    VerData[2].r = 255;
    VerData[2].g = 255;
    VerData[2].b = 255;
    VerData[2].a = 255;
    VerData[3].r = 255;
    VerData[3].g = 255;
    VerData[3].b = 255;
    VerData[3].a = 255;
    //
    VerData[0].t0x = 0.0f;
    VerData[0].t0y = 0.0f;
    VerData[1].t0x = 1.0f;
    VerData[1].t0y = 0.0f;
    VerData[2].t0x = 0.0f;
    VerData[2].t0y = 1.0f;
    VerData[3].t0x = 1.0f;
    VerData[3].t0y = 1.0f;
    //
    VerData[0].x = -1.0f;
    VerData[0].y = -1.0f;
    VerData[1].x = 1.0f;
    VerData[1].y = -1.0f;
    VerData[2].x = -1.0f;
    VerData[2].y = 1.0f;
    VerData[3].x = 1.0f;
    VerData[3].y = 1.0f;
    t_pMeshData->writeData(&VerData[0],sizeof(V2_C_T0)*4);
    //
    VerData[0].x = -1.0f;
    VerData[0].y = 0.0f;
    VerData[1].x = 0.0f;
    VerData[1].y = 0.0f;
    VerData[2].x = -1.0f;
    VerData[2].y = 1.0f;
    VerData[3].x = 0.0f;
    VerData[3].y = 1.0f;
    t_pLTMeshData->writeData(&VerData[0],sizeof(V2_C_T0)*4);
    //
    VerData[0].x = 0.0f;
    VerData[0].y = 0.0f;
    VerData[1].x = 1.0f;
    VerData[1].y = 0.0f;
    VerData[2].x = 0.0f;
    VerData[2].y = 1.0f;
    VerData[3].x = 1.0f;
    VerData[3].y = 1.0f;
    t_pLBMeshData->writeData(&VerData[0],sizeof(V2_C_T0)*4);
    //
    VerData[0].x = -1.0f;
    VerData[0].y = -1.0f;
    VerData[1].x = 0.0f;
    VerData[1].y = -1.0f;
    VerData[2].x = -1.0f;
    VerData[2].y = 0.0f;
    VerData[3].x = 0.0f;
    VerData[3].y = 0.0f;
    t_pRTMeshData->writeData(&VerData[0],sizeof(V2_C_T0)*4);
    //
    VerData[0].x = 0.0f;
    VerData[0].y = -1.0f;
    VerData[1].x = 1.0f;
    VerData[1].y = -1.0f;
    VerData[2].x = 0.0f;
    VerData[2].y = 0.0f;
    VerData[3].x = 1.0f;
    VerData[3].y = 0.0f;
    t_pRBMeshData->writeData(&VerData[0],sizeof(V2_C_T0)*4);
    //
    //构建屏幕渲染Mesh 旋转
    VerData[0].x = -1.0f;
    VerData[0].y = -1.0f;
    VerData[1].x = 1.0f;
    VerData[1].y = -1.0f;
    VerData[2].x = -1.0f;
    VerData[2].y = 1.0f;
    VerData[3].x = 1.0f;
    VerData[3].y = 1.0f;
    //(0,1)(1,1)
    //(0,0)(1,0)
    //    |
    //(1,0)(0,0)
    //(1,1)(0,1)
    VerData[0].t0x = 0.0f;
    VerData[0].t0y = 1.0f;
    VerData[1].t0x = 0.0f;
    VerData[1].t0y = 0.0f;
    VerData[2].t0x = 1.0f;
    VerData[2].t0y = 1.0f;
    VerData[3].t0x = 1.0f;
    VerData[3].t0y = 0.0f;
    t_pMeshData90->writeData(&VerData[0],sizeof(V2_C_T0)*4);
    //(0,1)(1,1)
    //(0,0)(1,0)
    //    |
    //(1,0)(0,0)
    //(1,1)(0,1)
    VerData[0].t0x = 1.0f;
    VerData[0].t0y = 1.0f;
    VerData[1].t0x = 0.0f;
    VerData[1].t0y = 1.0f;
    VerData[2].t0x = 1.0f;
    VerData[2].t0y = 0.0f;
    VerData[3].t0x = 0.0f;
    VerData[3].t0y = 0.0f;
    t_pMeshData180->writeData(&VerData[0],sizeof(V2_C_T0)*4);
    //(0,1)(1,1)
    //(0,0)(1,0)
    //    |
    //(1,0)(0,0)
    //(1,1)(0,1)
    VerData[0].t0x = 1.0f;
    VerData[0].t0y = 0.0f;
    VerData[1].t0x = 1.0f;
    VerData[1].t0y = 1.0f;
    VerData[2].t0x = 0.0f;
    VerData[2].t0y = 0.0f;
    VerData[3].t0x = 0.0f;
    VerData[3].t0y = 1.0f;
    t_pMeshData270->writeData(&VerData[0],sizeof(V2_C_T0)*4);
    //
    m_screenLTMesh->setVertexDataNum(4);
    m_screenLTMesh->setVertexData(t_pLTMeshData);
    
    m_screenLBMesh->setVertexDataNum(4);
    m_screenLBMesh->setVertexData(t_pLBMeshData);
    
    m_screenRTMesh->setVertexDataNum(4);
    m_screenRTMesh->setVertexData(t_pRTMeshData);
    
    m_screenRBMesh->setVertexDataNum(4);
    m_screenRBMesh->setVertexData(t_pRBMeshData);
    
    m_screenMesh->setVertexDataNum(4);
    m_screenMesh->setVertexData(t_pMeshData);
    
    m_screenMeshRot90->setVertexDataNum(4);
    m_screenMeshRot90->setVertexData(t_pMeshData90);
    
    m_screenMeshRot180->setVertexDataNum(4);
    m_screenMeshRot180->setVertexData(t_pMeshData180);
    
    m_screenMeshRot270->setVertexDataNum(4);
    m_screenMeshRot270->setVertexData(t_pMeshData270);
    //封装命令出去
    m_screenMesh->createMesh();
    m_screenMeshRot90->createMesh();
    m_screenMeshRot180->createMesh();
    m_screenMeshRot270->createMesh();
    m_screenLTMesh->createMesh();
    m_screenLBMesh->createMesh();
    m_screenRTMesh->createMesh();
    m_screenRBMesh->createMesh();
    //
    _initTwoDivisionMesh();
    _initFourDivisionMesh();
    _initFourDivisionMesh_X();
    //
    m_faceDataMesh = MakeSharedPtr<SVFaceDataMesh>(mApp);
    m_faceDataMesh->init();
}

void SVStaticData::destroy() {
    m_screenMesh        = nullptr;
    m_screenMeshRot90   = nullptr;
    m_screenMeshRot180  = nullptr;
    m_screenMeshRot270  = nullptr;
    m_screenLTMesh      = nullptr;
    m_screenLBMesh      = nullptr;
    m_screenRTMesh      = nullptr;
    m_screenRBMesh      = nullptr;
    m_screenTwoDivisionMesh = nullptr;
    m_screenFourDivisionMesh = nullptr;
    m_screenFourXDivisionMesh = nullptr;
    m_faceDataMesh       = nullptr;
}
//单位1的基本矩形
void SVStaticData::_initBaseRect(){
    //0 1 2 2 1 3
    m_baseRect[0].x = -0.5f;
    m_baseRect[0].y = -0.5f;
    m_baseRect[0].z = 0.0f;
    m_baseRect[0].t0x = 0.0f;
    m_baseRect[0].t0y = 0.0f;
    m_baseRect[0].r = 255;
    m_baseRect[0].g = 255;
    m_baseRect[0].b = 255;
    m_baseRect[0].a = 255;
    
    m_baseRect[1].x = 0.5f;
    m_baseRect[1].y = -0.5f;
    m_baseRect[1].z = 0.0f;
    m_baseRect[1].t0x = 1.0f;
    m_baseRect[1].t0y = 0.0f;
    m_baseRect[1].r = 255;
    m_baseRect[1].g = 255;
    m_baseRect[1].b = 255;
    m_baseRect[1].a = 255;
    
    m_baseRect[2].x = -0.5f;
    m_baseRect[2].y = 0.5f;
    m_baseRect[2].z = 0.0f;
    m_baseRect[2].t0x = 0.0f;
    m_baseRect[2].t0y = 1.0f;
    m_baseRect[2].r = 255;
    m_baseRect[2].g = 255;
    m_baseRect[2].b = 255;
    m_baseRect[2].a = 255;
    
    m_baseRect[3].x = -0.5f;
    m_baseRect[3].y = 0.5f;
    m_baseRect[3].z = 0.0f;
    m_baseRect[3].t0x = 0.0f;
    m_baseRect[3].t0y = 1.0f;
    m_baseRect[3].r = 255;
    m_baseRect[3].g = 255;
    m_baseRect[3].b = 255;
    m_baseRect[3].a = 255;
    
    m_baseRect[4].x = 0.5f;
    m_baseRect[4].y = -0.5f;
    m_baseRect[4].z = 0.0f;
    m_baseRect[4].t0x = 1.0f;
    m_baseRect[4].t0y = 0.0f;
    m_baseRect[4].r = 255;
    m_baseRect[4].g = 255;
    m_baseRect[4].b = 255;
    m_baseRect[4].a = 255;
    
    m_baseRect[5].x = 0.5f;
    m_baseRect[5].y = 0.5f;
    m_baseRect[5].z = 0.0f;
    m_baseRect[5].t0x = 1.0f;
    m_baseRect[5].t0y = 1.0f;
    m_baseRect[5].r = 255;
    m_baseRect[5].g = 255;
    m_baseRect[5].b = 255;
    m_baseRect[5].a = 255;
}

SVFaceDataMeshPtr SVStaticData::getFaceDataMesh(){
    return m_faceDataMesh;
}

void SVStaticData::_initTwoDivisionMesh(){
    //二分屏
    m_screenTwoDivisionMesh  = MakeSharedPtr<SVRenderMesh>(mApp);
    m_screenTwoDivisionMesh->setVertexType(E_VF_V2_C_T0);
    m_screenTwoDivisionMesh->setDrawMethod(E_DM_TRIANGLES);
    
    SVDataSwapPtr t_pMeshData_TwoVidion = MakeSharedPtr<SVDataSwap>();
    //渲染数据
    V2_C_T0 VerData[12];
    //颜色
    for (s32 i=0; i<12; i++) {
        VerData[i].r = 255;
        VerData[i].g = 255;
        VerData[i].b = 255;
        VerData[i].a = 255;
    }
    //纹理
    for (s32 i = 0; i<2; i++) {
        VerData[i*6].t0x = 0.0f;
        VerData[i*6].t0y = 0.0f;
        VerData[i*6+1].t0x = 1.0f;
        VerData[i*6+1].t0y = 0.0f;
        VerData[i*6+2].t0x = 0.0f;
        VerData[i*6+2].t0y = 1.0f;
        VerData[i*6+3].t0x = 0.0f;
        VerData[i*6+3].t0y = 1.0f;
        VerData[i*6+4].t0x = 1.0f;
        VerData[i*6+4].t0y = 0.0f;
        VerData[i*6+5].t0x = 1.0f;
        VerData[i*6+5].t0y = 1.0f;
    }
    //(0,1) (1,1)
    //(-1,0)(1,0)
    // ---------
    //(-1,0) (1,0)
    //(-1,-1)(1,-1)
    VerData[0].x = -1.0f;
    VerData[0].y = -1.0f;
    VerData[1].x = 1.0f;
    VerData[1].y = -1.0f;
    VerData[2].x = -1.0f;
    VerData[2].y = 0.0f;
    VerData[3].x = -1.0f;
    VerData[3].y = 0.0f;
    VerData[4].x = 1.0f;
    VerData[4].y = -1.0f;
    VerData[5].x = 1.0f;
    VerData[5].y = 0.0f;
    VerData[6].x = -1.0f;
    VerData[6].y = 0.0f;
    VerData[7].x = 1.0f;
    VerData[7].y = 0.0f;
    VerData[8].x = -1.0f;
    VerData[8].y = 1.0f;
    VerData[9].x = -1.0f;
    VerData[9].y = 1.0f;
    VerData[10].x = 1.0f;
    VerData[10].y = 0.0f;
    VerData[11].x = 1.0f;
    VerData[11].y = 1.0f;
    t_pMeshData_TwoVidion->writeData(&VerData[0],sizeof(V2_C_T0)*12);
    m_screenTwoDivisionMesh->setVertexDataNum(12);
    m_screenTwoDivisionMesh->setVertexData(t_pMeshData_TwoVidion);
    m_screenTwoDivisionMesh->createMesh();
}

void SVStaticData::_initFourDivisionMesh(){
    //四分屏
    m_screenFourDivisionMesh  = MakeSharedPtr<SVRenderMesh>(mApp);
    m_screenFourDivisionMesh->setVertexType(E_VF_V2_C_T0);
    m_screenFourDivisionMesh->setDrawMethod(E_DM_TRIANGLES);
    SVDataSwapPtr t_pMeshData_FourVidion = MakeSharedPtr<SVDataSwap>();
    //渲染数据
    V2_C_T0 VerData[24];
    //颜色
    for (s32 i = 0; i<24; i++) {
        VerData[i].r = 255;
        VerData[i].g = 255;
        VerData[i].b = 255;
        VerData[i].a = 255;
    }
    //纹理
    for (s32 i = 0; i<4; i++) {
        VerData[i*6].t0x = 0.0f;
        VerData[i*6].t0y = 0.0f;
        VerData[i*6+1].t0x = 1.0f;
        VerData[i*6+1].t0y = 0.0f;
        VerData[i*6+2].t0x = 0.0f;
        VerData[i*6+2].t0y = 1.0f;
        VerData[i*6+3].t0x = 0.0f;
        VerData[i*6+3].t0y = 1.0f;
        VerData[i*6+4].t0x = 1.0f;
        VerData[i*6+4].t0y = 0.0f;
        VerData[i*6+5].t0x = 1.0f;
        VerData[i*6+5].t0y = 1.0f;
    }
    //(-1,1)(0,1)   | (0,1) (1,1)
    //(-1,0)(0,0)   | (0,0) (1,0)
    // ---------    | -----------
    //(-1,0) (0,0)  | (0,0) (1,0)
    //(-1,-1)(0,-1) | (0,-1)(1,-1)
    //顶点
    VerData[0].x = -1.0f;
    VerData[0].y = -1.0f;
    VerData[1].x = 0.0f;
    VerData[1].y = -1.0f;
    VerData[2].x = -1.0f;
    VerData[2].y = 0.0f;
    VerData[3].x = -1.0f;
    VerData[3].y = 0.0f;
    VerData[4].x = 0.0f;
    VerData[4].y = -1.0f;
    VerData[5].x = 0.0f;
    VerData[5].y = 0.0f;
    
    VerData[6].x = 0.0f;
    VerData[6].y = -1.0f;
    VerData[7].x = 1.0f;
    VerData[7].y = -1.0f;
    VerData[8].x = 0.0f;
    VerData[8].y = 0.0f;
    VerData[9].x = 0.0f;
    VerData[9].y = 0.0f;
    VerData[10].x = 1.0f;
    VerData[10].y = -1.0f;
    VerData[11].x = 1.0f;
    VerData[11].y = 0.0f;
    
    VerData[12].x = -1.0f;
    VerData[12].y = 0.0f;
    VerData[13].x = 0.0f;
    VerData[13].y = 0.0f;
    VerData[14].x = -1.0f;
    VerData[14].y = 1.0f;
    VerData[15].x = -1.0f;
    VerData[15].y = 1.0f;
    VerData[16].x = 0.0f;
    VerData[16].y = 0.0f;
    VerData[17].x = 0.0f;
    VerData[17].y = 1.0f;
    
    VerData[18].x = 0.0f;
    VerData[18].y = 0.0f;
    VerData[19].x = 1.0f;
    VerData[19].y = 0.0f;
    VerData[20].x = 0.0f;
    VerData[20].y = 1.0f;
    VerData[21].x = 0.0f;
    VerData[21].y = 1.0f;
    VerData[22].x = 1.0f;
    VerData[22].y = 0.0f;
    VerData[23].x = 1.0f;
    VerData[23].y = 1.0f;
    
    t_pMeshData_FourVidion->writeData(&VerData[0],sizeof(V2_C_T0)*24);
    
    m_screenFourDivisionMesh->setVertexDataNum(24);
    m_screenFourDivisionMesh->setVertexData(t_pMeshData_FourVidion);
    m_screenFourDivisionMesh->createMesh();
}

void SVStaticData::_initFourDivisionMesh_X(){
    //四分屏
    m_screenFourXDivisionMesh  = MakeSharedPtr<SVRenderMesh>(mApp);
    m_screenFourXDivisionMesh->setVertexType(E_VF_V2_C_T0);
    m_screenFourXDivisionMesh->setDrawMethod(E_DM_TRIANGLES);
    SVDataSwapPtr t_pMeshData_FourVidion = MakeSharedPtr<SVDataSwap>();
    //渲染数据
    V2_C_T0 VerData[12];
    //颜色
    for (s32 i = 0; i<12; i++) {
        VerData[i].r = 255;
        VerData[i].g = 255;
        VerData[i].b = 255;
        VerData[i].a = 255;
    }
    //纹理
    for (s32 i = 0; i<4; i++) {
        VerData[i*3].t0x = 0.0f;
        VerData[i*3].t0y = 0.0f;
        VerData[i*3+1].t0x = 1.0f;
        VerData[i*3+1].t0y = 0.0f;
        VerData[i*3+2].t0x = 0.5f;
        VerData[i*3+2].t0y = 0.5f;
    }
    //          \                                /
    //           \                              /
    //            \                            /
    //             \                          /
    //              \                        /
    //               \                      /
    //                \                    /
    //                 \   (-1,1)(1,1)    /
    //                  \     (0,0)      /
    //                   \              /
    //                    \            /
    //                     \          /
    //                      \        /
    //                       \      /
    //                        \    /
    //                         \  /
    //         (0,0)            \/           (0,0)
    //     (-1,1)(-1,-1)        /\        (1,-1)(1,1)
    //                         /  \
    //                        /    \
    //                       /      \
    //                      /        \
    //                     /          \
    //                    /            \
    //                   /              \
    //                  /      (0,0)     \
    //                 /   (-1,-1)(1,-1)  \
    //                /                    \
    //               /                      \
    //              /                        \
    //             /                          \
    //            /                            \
    //           /                              \
    //
    //
    //
    //顶点
    VerData[0].x = -1.0f;
    VerData[0].y = -1.0f;
    VerData[1].x = 1.0f;
    VerData[1].y = -1.0f;
    VerData[2].x = 0.0f;
    VerData[2].y = 0.0f;
    
    VerData[3].x = 1.0f;
    VerData[3].y = -1.0f;
    VerData[4].x = 1.0f;
    VerData[4].y = 1.0f;
    VerData[5].x = 0.0f;
    VerData[5].y = 0.0f;
    
    VerData[6].x = 1.0f;
    VerData[6].y = 1.0f;
    VerData[7].x = -1.0f;
    VerData[7].y = 1.0f;
    VerData[8].x = 0.0f;
    VerData[8].y = 0.0f;
    
    VerData[9].x = -1.0f;
    VerData[9].y = 1.0f;
    VerData[10].x = -1.0f;
    VerData[10].y = -1.0f;
    VerData[11].x = 0.0f;
    VerData[11].y = 0.0f;
    t_pMeshData_FourVidion->writeData(&VerData[0],sizeof(V2_C_T0)*12);
    
    m_screenFourXDivisionMesh->setVertexDataNum(12);
    m_screenFourXDivisionMesh->setVertexData(t_pMeshData_FourVidion);
    m_screenFourXDivisionMesh->createMesh();
}
