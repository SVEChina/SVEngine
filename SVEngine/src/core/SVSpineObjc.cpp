//
// SVSpineObjc.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVSpineObjc.h"
#include "../../../base/SVDataChunk.h"
#include "../../../mtl/SVTexture.h"
#include "../../../mtl/SVTexMgr.h"
#include "../../../file/SVFileMgr.h"
#include "../../../app/SVInst.h"
#include "SVSpine.h"
SVSpineObjc::SVSpineObjc(){
    m_texture = nullptr;
}
SVSpineObjc::~SVSpineObjc(){
    m_texture = nullptr;
}

//获取数据内容接口
char *_spUtil_readFile(void *_app, const char *path, int *length) {
    SVInst *t_App = (SVInst *) _app;
    //返回文件内容
    SVDataChunk tChunk;
    bool t_resule = t_App->getFileMgr()->loadFileContent(&tChunk, path);
    (*length) = tChunk.m_size;
    char *t_data = (char *) malloc(tChunk.m_size);
    memcpy(t_data, tChunk.m_data, tChunk.m_size);
    return t_data;
}

//创建纹理接口
void _spAtlasPage_createTexture(void *_app, spAtlasPage *self, const char *path, char enableMipMap) {
    SVInst *_tApp = (SVInst *) _app;
    SVSpineObjc* pSpineObj = new SVSpineObjc();
    pSpineObj->m_texture = _tApp->getTexMgr()->getTextureSync(path, true, enableMipMap);
    if (pSpineObj && pSpineObj->m_texture) {
        self->pRenderObj = (void*)pSpineObj;
    }
}

//纹理析构接口
void _spAtlasPage_disposeTexture(spAtlasPage *self) {
    if (self->pRenderObj){
        SVSpineObjc *t_spineObj = (SVSpineObjc *) self->pRenderObj;
        if (t_spineObj){
            delete t_spineObj;
        }
    }
}

