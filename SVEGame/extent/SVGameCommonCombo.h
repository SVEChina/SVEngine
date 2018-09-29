//
// Created by yinlong on 17/10/20.
//

#ifndef SV_GAMECOMMON_COMBO_H
#define SV_GAMECOMMON_COMBO_H

#include "../base/SVVector3d.h"
#include "../node/SVNode.h"

//通用的连击
class SVSpineNode;
class SVUIParamBase;
class SVBitFontNode;

class SVGameCommonCombo : public SVNode {
public:
    SVGameCommonCombo(SVInst*_app);

    ~SVGameCommonCombo();
    
    virtual void init();
    
    virtual void destroy();
    
    virtual void enter();
    
    virtual void exit();
    
    virtual void update(float _dt);
    
    virtual bool procEvent(SVEvent *_event);
    
    void createUI(SVUIParamBase* _param);
    
    void setSceneName(const char* pSceneName);
protected:
    stringc                 m_scenename;
    SVSpineNode*            m_pUiCombo;
    SVBitFontNode*          m_pUiComboValue;      //连击数
    vector3df               m_vecCombo;
    typedef array<stringc>  ANIPOOL;
    ANIPOOL                 m_arrAni;
};


#endif //SV_GAMECOMMON_COMBO_H
