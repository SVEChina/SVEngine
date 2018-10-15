//
// Created by 徐子昱 on 2018/10/15.
//


#include "SVWatermark.h"
#include "rendercore/SVRenderDef.h"
#include "mtl/SVTexMgr.h"
#include "node/SVSpriteNode.h"
#include "node/SVScene.h"
#include "basesys/SVSceneMgr.h"




SVWatermark::SVWatermark(SVInst *_app)
:SVGBase(_app){
    m_sprite= nullptr;
}

SVWatermark::~SVWatermark(){
    m_sprite= nullptr;
}

void SVWatermark::createNode(void* _rgba,int _width,int _height){
//    m_sprite =MakeSharedPtr<SVSpriteNode>(mApp,200,200);
//    m_sprite->setPosition(0.0,0.0,0.0);
//    m_sprite->setRSType(RST_BACKGROUND);
//    m_sprite->setTexture(mApp->getTexMgr()->createUnctrlTextureWithData(_width,  _height, GL_RGBA, GL_RGBA,_rgba,false));
//    SVScenePtr t_scene = mApp->getSceneMgr()->getScene();
//    if( t_scene ) {
//        t_scene->addNode(m_sprite);
//    }
}

void SVWatermark::clearNode(){
    m_sprite->removeFromParent();
}
