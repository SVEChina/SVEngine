//
// Created by lidm on 17/9/5.
//

#ifndef SV_FlOWERDANCE_BATUI_H
#define SV_FlOWERDANCE_BATUI_H

#include "../event/SVEventProc.h"
#include "../base/SVVector3d.h"
#include "SVGameUIBase.h"
//战斗主UI
class SVBitFontNode;
class SVSpineNode;
class SVFlowerDanceBatUI : public SVGameUIBase {
public:
    SVFlowerDanceBatUI(SVInst *_app);

    ~SVFlowerDanceBatUI();
    
    virtual void init(SVGameUIMgr* pUIMgr);
    
    virtual void destroy();
    
    virtual void enter();
    
    virtual void exit();
    
    virtual void update(float _dt);
    
    virtual bool procEvent(SVEvent *_event);
protected:
    stringc m_comboAni;         //连击动画
    SVBitFontNode *m_uiComboValue;      //连击数
    SVSpineNode* m_uiCombo;          //连击
    
};


#endif //SV_FlOWERDANCE_BATUI_H
