//
// SVPictureProcess.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVPictureProcess.h"
#include "../app/SVInst.h"
#include "../app/SVGlobalMgr.h"
#include "../basesys/SVSceneMgr.h"
#include "../node/SVScene.h"
#include "../node/SVMultPassNode.h"
#include "../mtl/SVMtlCore.h"
#include "../mtl/SVMtlFair1.h"
#include "../mtl/SVTexMgr.h"
#include "../mtl/SVTexture.h"
#include "../mtl/SVMtlBasedOn.h"
#include "../rendercore/SVRenderTexture.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRendererBase.h"
#include "filter/SVRGBToneCurveFilter.h"
#include "filter/SVRGBToneCurveCore.h"
#include "filter/SVBasedonFilter.h"
#include "../core/SVPass.h"
#include "filter/SVFilterBase.h"

SVPictureProcess::SVPictureProcess(SVInst *_app)
:SVProcess(_app){
    
}

SVPictureProcess::~SVPictureProcess() {
}

void SVPictureProcess::addFilter(SVFilterBasePtr _filter){
    clearFilter(_filter->getType());
    m_filterArray.append(_filter);
}

void SVPictureProcess::openFilter(SVFilterBasePtr _filter){
  
    SVScenePtr t_sc = mApp->getSceneMgr()->getScene();
    if(t_sc){
        SVNodePtr t_node=_filter->getNode();
        if(t_node){
            t_sc->addNode(_filter->getNode());
        }
    }
}

void SVPictureProcess::init() {
}

void SVPictureProcess::destroy() {
    for(int i=0;i<m_filterArray.size();i++){
        m_filterArray.get(i)->destroy();
    }
    m_filterArray.clear();
    m_filterArray.reserve(0);
}

void SVPictureProcess::update(f32 _dt) {
    for(int i=0;i<m_filterArray.size();i++){
        m_filterArray.get(i)->update(_dt);
    }
}

//更新美颜参数
void SVPictureProcess::setFilterParam(f32 _smooth,SVFILTERITEMTYPE _type){
    for(int i=0;i<m_filterArray.size();i++){
        m_filterArray.get(i)->setFilterParam(_smooth, _type);
    }
}

f32 SVPictureProcess::getFilterParam(SVFILTERITEMTYPE _type) {
    for(int i=0;i<m_filterArray.size();i++){
        f32 t_value=m_filterArray.get(i)->getFilterParam(_type);
        if(t_value){
            return t_value;
        }
    }
    return 0.0f;
}

SVFilterBasePtr SVPictureProcess::getFilter(SVString _name){
    for(int i=0;i<m_filterArray.size();i++){
        SVFilterBasePtr t_filter=m_filterArray.get(i);
        SVString t_name=t_filter->getName();
        if(strcmp(t_name.c_str(), _name.c_str())==0){
            return t_filter;
        }
    }
    return nullptr;
}

void SVPictureProcess::clearFilter(SVFilterBasePtr _filter){
    for(int i=0;i<m_filterArray.size();i++){
        if(_filter==m_filterArray.get(i)){
            SVScenePtr t_sc = mApp->getSceneMgr()->getScene();
            if(t_sc){
                SVNodePtr t_node=_filter->getNode();
                t_node->removeFromParent();
            }
              m_filterArray.get(i)->destroy();
              m_filterArray.removeForce(i);
            break;
        }
    }
}

void SVPictureProcess::clearFilter(SVString t_name){
    for(int i=0;i<m_filterArray.size();i++){
        if(strcmp(t_name.c_str(), m_filterArray.get(i)->getName().c_str())==0){
            SVFilterBasePtr t_filter=m_filterArray.get(i);
            SVScenePtr t_sc = mApp->getSceneMgr()->getScene();
            if(t_sc){
                SVNodePtr t_node=t_filter->getNode();
                t_node->removeFromParent();
            }
            m_filterArray.get(i)->destroy();
            m_filterArray.removeForce(i);
            
            break;
        }
    }
}

void SVPictureProcess::clearFilter(SVFILTERFUNCTYPE t_type){
    for(int i=0;i<m_filterArray.size();i++){
        if(m_filterArray.get(i)->getType()==t_type){
            m_filterArray.removeForce(i);
            break;
        }
    }
}

//更新曲线数据
//void SVPictureProcess::setFilterBSplineData(ptru8 _data){
//    if(m_toneCurverFilter){
//        m_toneCurverFilter->texrgb->setTexData(_data, 256*4);
//        m_toneCurverFilter->texrgb->getbLoad();
//    } else {
//        SV_LOG_ERROR("please call function openToneCurverRGB!\n");
//    }
//}
