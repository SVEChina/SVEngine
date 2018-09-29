//
// SVBasicModel.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVBasicModel.h"
#include "../app/SVInst.h"
#include "../file/SVFileMgr.h"
#include "../base/SVDataChunk.h"

#ifdef CONFIG_IS_LOAD_ASSIMP

#include "../../library/3d/assimpd/assimp/scene.h"
#include "../../library/3d/assimpd/assimp/postprocess.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

SVBasicModel::SVBasicModel(SVInst *_app)
:SVGBase(_app)
,m_pModelScene(nullptr) {
}

SVBasicModel::~SVBasicModel() {
    clear();
}

cptr8 SVBasicModel::getname(){
    return m_name.c_str();
}

void SVBasicModel::setname(cptr8 _name){
    m_name = _name;
}

bool SVBasicModel::InitFromScene(const aiScene *pScene, cptr8 Filename) {

    return true;
}

bool SVBasicModel::LoadModel(cptr8 pPath ,cptr8 Filename) {
    clear();
    m_strPath = pPath;
    SVString strRealPath = m_strPath + Filename;
    bool Ret = false;

    SVDataChunk tChunk;
    mApp->getFileMgr()->loadFileContent(&tChunk, strRealPath.c_str());

    //加载出错输出错误信息
    m_pModelScene = m_Importer.ReadFileFromMemory(tChunk.m_data, tChunk.m_size,
                                                  aiProcess_Triangulate | aiProcess_FlipUVs |
                                                  aiProcess_JoinIdenticalVertices |
                                                  aiProcess_OptimizeMeshes);
    if (m_pModelScene) {
        Ret = InitFromScene(m_pModelScene, Filename);
    } else {
        printf("Error parsing '%s': '%s'\n", Filename, m_Importer.GetErrorString());
    }

    return Ret;

}

const aiScene *SVBasicModel::getModelData() {
    return m_pModelScene;
}

void SVBasicModel::clear() {

}

#endif
