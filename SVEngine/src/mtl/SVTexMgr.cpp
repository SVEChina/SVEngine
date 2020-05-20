//
// SVTexMgr.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVTexMgr.h"
#include "SVTexturePList.h"
#include "SVTexture.h"
#include "../third/rapidjson/document.h"
#include "../file/SVLoaderPng.h"
#include "../file/SVFileMgr.h"
#include "../base/SVDataChunk.h"
#include "../app/SVInst.h"
#include "../operate/SVOpCreate.h"
#include "../operate/SVOpParse.h"
#include "../rendercore/SVRenderMgr.h"
#include "../work/SVThreadPool.h"

SVTexMgr::SVTexMgr(SVInst *_app)
:SVGBase(_app) {
    mAsync = false;
    m_sveTexture = nullptr;
    m_texLock = MakeSharedPtr<SVLock>();
}

SVTexMgr::~SVTexMgr() {
    m_texLock = nullptr;
    m_sveTexture = nullptr;
}

void SVTexMgr::init() {
    m_sveTexture = getTextureSync("svres/sve.png",true);
}

void SVTexMgr::destroy() {
    m_sveTexture = nullptr;
    clear();
}

SVTexturePtr SVTexMgr::getSVETexture(){
    return m_sveTexture;
}

void SVTexMgr::update(f32 _dt){
    _removeUnuseTexture();
}

void SVTexMgr::clear() {
    _removeUnuseTexture();
}

//这里才是真正干掉纹理的地方
void SVTexMgr::_removeUnuseTexture() {
    m_texLock->lock();
    TEXPOOL::Iterator it = mTexpool.begin();
    while ( it!=mTexpool.end() ) {
        if (it->data && it->data.use_count() == 2) {
            mTexpool.remove(it);    //移除是不是会导致迭代器失效?
            it = mTexpool.begin();  //重置迭代器
        } else {
            it++;
        }
    }
    m_texLock->unlock();
}

//返回一个空壳纹理
SVTexturePtr SVTexMgr::getTexture(cptr8 _name, bool _create, bool _enableMipMap) {
    TEXPOOL::Iterator it = mTexpool.find(_name);
    if( it!=mTexpool.end() ) {
        return it->data;
    }
    if (_create) {
        return _createTexture(_name, !mAsync, _enableMipMap);
    }
    return nullptr;
}

//同步创建纹理
SVTexturePtr SVTexMgr::getTextureSync(cptr8 _name, bool _create, bool _enableMipMap) {
    SVTexturePtr t_tex = nullptr;
    m_texLock->lock();
    TEXPOOL::Iterator it = mTexpool.find(_name);
    if( it!=mTexpool.end() ) {
        t_tex = it->data;
    }
    m_texLock->unlock();
    if(!t_tex) {
        if (_create) {
            t_tex = _createTexture(_name, true, _enableMipMap);
        }
    }
    return t_tex;
}

SVTexturePtr SVTexMgr::createUnctrlTextureWithTexID(s32 _texId, s32 _width, s32 _height, s32 _informat, s32 _dataformat, bool _enableMipMap){
    SVTexturePtr tTexture = MakeSharedPtr<SVTextureInputTexID>(mApp, _texId);
    tTexture->init("", GL_TEXTURE_2D, _width, _height, _informat, _dataformat, _enableMipMap);
    mApp->getRenderMgr()->pushRCmdCreate(tTexture);
    return tTexture;
}

SVTexturePtr SVTexMgr::createUnctrlTextureWithData(s32 _width, s32 _height, s32 _informat, s32 _dataformat, void* _data, bool _enableMipMap){
    SVTexturePtr tTexture = MakeSharedPtr<SVTexture>(mApp);
    SVString t_name = SVString::format("%d", tTexture->getuid());
    t_name = SVString("unctrltexdata") + SVString("_") + t_name;
    tTexture->init(t_name.c_str(), GL_TEXTURE_2D, _width, _height, _informat, _dataformat, _enableMipMap);
    s32 t_len = 0;//应该根据格式算的
    if(_informat == GL_RGB){
        t_len = _width*_height*3;
    }else if(_informat == GL_RGBA){
        t_len = _width*_height*4;
    }else if(_informat == GL_LUMINANCE){
        t_len = _width*_height;
    }else if(_informat == GL_LUMINANCE_ALPHA){
        t_len = _width*_height*2;
    }
    tTexture->setTexData(_data, t_len);
    mApp->getRenderMgr()->pushRCmdCreate(tTexture);
    mTexpool.append(t_name, tTexture);
    return tTexture;
}

SVTexturePtr SVTexMgr::createUnctrlTexture(s32 _w, s32 _h, s32 _informat,  s32 _dataformat, bool _enableMipMap){
    SVTexturePtr tTexture = MakeSharedPtr<SVTexture>(mApp);
    SVString t_name = SVString::format("%d", tTexture->getuid());
    t_name = SVString("unctrltex") + SVString("_") + t_name;
    tTexture->init(t_name.c_str(), GL_TEXTURE_2D, _w, _h, _informat, _dataformat, _enableMipMap);
    mApp->getRenderMgr()->pushRCmdCreate(tTexture);
    mTexpool.append(t_name, tTexture);
    return tTexture;
}

SVTexturePtr SVTexMgr::_createTexture(cptr8 _name, bool _sync, bool _enableMipMap) {
    //返回空壳纹理
    m_texLock->lock();
    SVTexturePtr tTexture = MakeSharedPtr<SVTexture>(mApp);
    if (_sync) {
        _loadTextureSync(tTexture, _name, _enableMipMap);
    } else {
        _loadTextureAsync(tTexture, _name, _enableMipMap);
    }
    mTexpool.append(_name, tTexture);
    m_texLock->unlock();
    return tTexture;
}

SVTexturePtr SVTexMgr::_createTextureSet(cptr8 _name, bool _sync, bool _enableMipMap) {
    //返回空壳纹理
    m_texLock->lock();
    SVTexturePtr tTexture = MakeSharedPtr<SVTextureSet>(mApp);
    if (_sync) {
        _loadTextureSync(tTexture, _name, _enableMipMap);
    } else {
        _loadTextureAsync(tTexture, _name, _enableMipMap);
    }
    mTexpool.append(_name, tTexture);
    m_texLock->unlock();
    return tTexture;
}

void SVTexMgr::_loadTextureSync(SVTexturePtr _tex,cptr8 _name, bool _enableMipMap) {
    SVOpTexLoad tTexLoad(mApp, _tex, _name, _enableMipMap);
    tTexLoad.process(0.0f);
}

void SVTexMgr::_loadTextureAsync(SVTexturePtr _tex, cptr8 _name, bool _enableMipMap) {
//    SVOpTexLoadPtr tTexLoad = MakeSharedPtr<SVOpTexLoad>(mApp, _tex, _name, _enableMipMap);
//    if(mApp->m_pTPool->getHelpThread()){
//        mApp->m_pTPool->getHelpThread()->pushThreadOp(tTexLoad);
//    }
}

bool SVTexMgr::hasTexture(SVTexturePtr _tex){
    TEXPOOL::Iterator it = mTexpool.findData(_tex);
    if(it != mTexpool.end() ) {
        return true;
    }
    return false;
}

bool SVTexMgr::hasTexture(cptr8 _name) {
    TEXPOOL::Iterator it = mTexpool.find(_name);
    if( it!=mTexpool.end() ) {
        return true;
    }
    return false;
}

//加载纹理
SVTexturePtr SVTexMgr::loadImgPList(cptr8 _plistname,cptr8 _path) {
    return nullptr;
}
