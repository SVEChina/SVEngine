//
// Created by 徐子昱 on 2018/10/15.
//


#include "SVWatermark.h"
#include "rendercore/SVRenderDef.h"
#include "mtl/SVTexMgr.h"
#include "node/SVSpriteNode.h"
#include "node/SVScene.h"
#include "mtl/SVTexture.h"
#include "basesys/SVSceneMgr.h"
#include "work/SVThreadPool.h"
#include "work/SVThreadMain.h"
#include "SVOpFaceBeautyExt.h"
#include "SVFilterExtDef.h"


SVWatermark::SVWatermark(SVInst *_app)
:SVGBase(_app){
    m_sprite= nullptr;
}

SVWatermark::~SVWatermark(){
    m_sprite= nullptr;
}

void SVWatermark::createNode(void* _rgba,int _width,int _height){
    m_sprite =MakeSharedPtr<SVSpriteNode>(mApp,_width,_height);
    m_sprite->setPosition(-200.0,-500.0,0.0);
    m_sprite->setRSType(RST_ANIMATE);
    SVTexturePtr t_tex=mApp->getTexMgr()->createUnctrlTextureWithData(_width,  _height, GL_RGBA, GL_RGBA,_rgba,false);
    m_sprite->setTexture(t_tex->getname());
    SVOpWatermarkExtPtr t_op = MakeSharedPtr<SVOpWatermarkExt>(mApp,m_sprite);
    mApp->m_pTPool->getMainThread()->pushThreadOp(t_op);
}

void SVWatermark::clearNode(){
    m_sprite->removeFromParent();
}

