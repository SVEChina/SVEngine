//
// Created by yinlong on 17/10/27.
//

#include "SVGameUIMgr.h"
#include "../app/SVInst.h"
#include "../game/SVGameUIShow.h"
#include "../ui/SVUIParamUI.h"

SVGameUIMgr::SVGameUIMgr(SVInst* _app):SVGBase(_app){
    
}
SVGameUIMgr::~SVGameUIMgr(){
    
}
    
void SVGameUIMgr::init(){
    
}
    
void SVGameUIMgr::destory(){
    clear();
}
    
void SVGameUIMgr::update(float _dt){

}
    
void SVGameUIMgr::addUI(SVGameUIShow* _pUI){
    if (_pUI){
        return;
    }
    
    MAPUIMGR::Node* it = m_mapUIMgr.find(_pUI->getName());
    if (it == NULL){
        _pUI->retain();
        m_mapUIMgr.insert(_pUI->getName() , _pUI);
    }
}
    
void SVGameUIMgr::removeUI(const char* _strUIName){
    MAPUIMGR::Node* it = m_mapUIMgr.find(_strUIName);
    if (it != NULL){
        it->getValue()->release();
        m_mapUIMgr.remove(_strUIName);
    }
}
    
void SVGameUIMgr::clear(){
    MAPUIMGR::Iterator it = m_mapUIMgr.getIterator();
    while (it.atEnd() == false) {
        SVGameUIShow* _uiSHow = it->getValue();
        _uiSHow->release();
        it++;
    }
}
    
SVGameUIShow* SVGameUIMgr::getUI(const char* _strUIName){
    MAPUIMGR::Node* it = m_mapUIMgr.find(_strUIName);
    if (it == NULL){
        return NULL;
    }
    return it->getValue();
}

void SVGameUIMgr::create(SVUIParamUI* _pUIParam){
    if (_pUIParam == NULL){
        return;
    }
    
    int iSize = _pUIParam->m_arrayUIData.size();
    for (int i = 0 ; i < iSize ; ++i){
        UIData* pUiData = _pUIParam->m_arrayUIData[i];
        if (pUiData == NULL){
            continue;
        }
        
        SVGameUIShow* pShow = new SVGameUIShow(mApp);
        pShow->init(pUiData->m_strUIName.c_str());
        
        int iUiSize = pUiData->m_arrayUI.size();
        for (int j = 0 ; j < iUiSize ; ++j){
            SVUIParamBase* pUiParam = pUiData->m_arrayUI[j];
            if (pUiParam != NULL){
                pShow->createUI(pUiParam);
            }
        }
        
        m_mapUIMgr.insert(pUiData->m_strUIName.c_str(), pShow);
    }
}
