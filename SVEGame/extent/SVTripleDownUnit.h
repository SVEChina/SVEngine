//
// SVTripleDownUnit.h
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//

#ifndef SV_SVTRIPLEDOWN_UNIT_H
#define SV_SVTRIPLEDOWN_UNIT_H

#include "../event/SVEventProc.h"
#include "../base/SVVector2d.h"
#include "../base/SVRect.h"
#include "../act/SVActBase.h"
#include "SVTripleDownDef.h"

//note 三消
class SVSpineNode;
class SVActionBase;
class SVTripleDownUnit : public SVEventProc {
public:
    SVTripleDownUnit(SVInst *_app);

    ~SVTripleDownUnit();
    
    virtual void init(const char* _scenename,const char* _spinename);
    
    virtual void destroy();
    
    void enter();
    
    void exit();
    
    void stop();
    
    virtual void update(float _dt,float _gametime);
    
    void setTripleType(TRIPLEUNITTYPE eType);
    TRIPLEUNITTYPE getTripleType();
    
    void setTripleAniName(const char* cName);
    const char* getTripleAniName();
    
    void setTripleState(TRIPLEUNITSTATE eState);
    TRIPLEUNITSTATE getTripleState();
    
    void ShowAni(CIRCLEANI eAniState);
    
    void setPosition(vector3df Pos);
    vector3df getPosition();
    
    void setWidth(float fWidth);
    float getWidth();
    
    void setHeight(float fHeight);
    float getHeight();
    
    void setScale(float fScale);
    float getScale();
    
    void addAction(SVActPtr pAction);
    
    rectf& getBoundingBox();
    
protected:
    SVSpineNode*    m_AniTriple;
    stringc         m_strAction;
    stringc         m_strSceneName;
    stringc         m_spinename;
    TRIPLEUNITTYPE  m_eUnitType;
    TRIPLEUNITSTATE m_eState;
    f32             m_fWidth;
    f32             m_fHeight;
    f32             m_fDeadingTime;
    f32             m_fScale;
    rectf           m_rectBoundingBox;
};

class SVActionTripleDownChangeAni : public SVActBase {
public:
    SVActionTripleDownChangeAni(SVTripleDownUnit* pUniy);
    ~SVActionTripleDownChangeAni();
    virtual void enter(SVNode *_node);
    
    virtual void run(SVNode *_node, float dt);
    
    virtual bool isEnd();
    
protected:
    SVTripleDownUnit* m_pUnit;
    bool m_bEnd;
};



#endif //SV_SVTRIPLEDOWN_UNIT_H
