//
// SVTripleDownBatUI.h
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//

#ifndef SV_TRIPLEDOWN_BATUI_H
#define SV_TRIPLEDOWN_BATUI_H

#include "../event/SVEventProc.h"
#include "../base/SVVector3d.h"
#include "SVGameUIBase.h"

class SVGameUIMgr;
class SVBitFontNode;
//战斗主UI
class SVTripleDownBatUI : public SVGameUIBase {
public:
    SVTripleDownBatUI(SVInst *_app);

    ~SVTripleDownBatUI();
    
    virtual void init(SVGameUIMgr* pUIMgr);
    
    virtual void destroy();
    
    virtual void enter();
    
    virtual void exit();
    
    virtual void update(float _dt);
    
    virtual bool procEvent(SVEvent *_event);
protected:
    bool m_bShowCurScore;
    float m_fShowTime;
    
    SVBitFontNode* m_uiGameScore;
    SVBitFontNode* m_uiCurScore;
};


#endif //SVTripleDownBatUI
