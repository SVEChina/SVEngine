//
// Created by lidm on 17/9/5.
//

#ifndef SV_FACEDANCE_BATUI_H
#define SV_FACEDANCE_BATUI_H

#include "../event/SVEventProc.h"
#include "../base/SVVector3d.h"
#include "SVGameUIBase.h"
//战斗主UI
//class SVBitFontNode;
//class SVSpineNode;
class SVFaceDanceBatUI : public SVGameUIBase {
public:
    SVFaceDanceBatUI(SVInst *_app);

    ~SVFaceDanceBatUI();
    
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


#endif //SV_FACEDANCE_BATUI_H
