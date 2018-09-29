//
// SVTripleDownUnit.h
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//

#ifndef SV_FLOWERDANCE_UNIT_H
#define SV_FLOWERDANCE_UNIT_H

#include "../event/SVEventProc.h"
#include "../base/SVVector2d.h"
#include "../base/SVRect.h"
#include "../act/SVActBase.h"
#include "SVFlowerDanceDef.h"

//note 三消
class SVSpineNode;
class SVSpriteNode;
class SVActionBase;
class SVFlowerDanceUnit : public SVEventProc {
public:
    SVFlowerDanceUnit(SVInst *_app);

    ~SVFlowerDanceUnit();
    
    virtual void init(cptr8 _scenename,cptr8 _spinename ,cptr8 _circleName,cptr8 _pcname);
    
    virtual void destroy();
    
    void enter();
    
    void exit();
    
    void stop();
    
    void setType(int iType);
    int  getType();
    
    void setOpened(bool bOpened);
    bool getOpened();
    
    virtual void update(float _dt);
    
    FLOWERDANCESTATE getFlowerDanceState();
    
    void setPosition(vector3df vecPos);
    vector3df getPosition();
    
    void  setOutterSize(float fSize);
    float getOutterSize();
    
    void  setInnerSize(float fSize);
    float getInnerSize();
    
    void playAni(const char* pName , bool bLoop = false);
    
    SVSpineNode* getPC();
    
protected:
    SVSpineNode*        m_AniTriple;
    SVSpineNode*        m_AniPC;        //瓢虫
    SVSpriteNode*       m_spCircle;
    stringc             m_strSceneName;
    stringc             m_spinename;
    stringc             m_strCircleName;
    stringc             m_spinenamePC;
    stringc             m_strLastAni;
    FLOWERDANCESTATE    m_eState;
    float               m_fDeadingTime;
    
    float               m_fInnerSize;
    float               m_fOutterSize;
    
    vector3df           m_vecPos;
    int                 m_iType;
    bool                m_bOpened;
};

#endif //SV_FLOWERDANCE_UNIT_H
