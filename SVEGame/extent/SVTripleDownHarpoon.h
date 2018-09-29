//
// SVTripleDownHarpoon.h
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//

#ifndef SV_TRIPLEDOWN_HARPOON_H
#define SV_TRIPLEDOWN_HARPOON_H

#include "../event/SVEventProc.h"
#include "../base/SVCoreArray.h"
#include "../base/SVVector3d.h"
#include "SVTripleDownDef.h"

//note 三消
class SVSpineNode;
class SVTripleDownUnit;
class SVTripleDownHarpoon : public SVEventProc {
public:
    SVTripleDownHarpoon(SVInst *_app);

    ~SVTripleDownHarpoon();
    
    virtual void init(const char* _scenename,const char* _spinename);
    
    virtual void destroy();
    
    void enter();
    
    void exit();
    
    void stop();
    
    virtual void update(float _dt,float _gametime);
  
    void setPosition(vector3df Pos);
    
    void setHeight(float fHeight);
    float getHeight();

    void setWidth(float fWidth);
    float getWidth();
    
    void showAni(HARPOONANI eAni);
    
    void addCircle(SVTripleDownUnit* pUnit);
    
    void setType(HARPOONTYPE eType);
    HARPOONTYPE getType();
    
protected:
    SVSpineNode*    m_AniHarpoon;
    stringc         m_strAction;
    stringc         m_strSceneName;
    stringc         m_strSpineName;
    float           m_fWidth;
    float           m_fHeight;

    typedef list<SVTripleDownUnit*> CIRCLEPOOL;
    CIRCLEPOOL m_circlePool;
    CIRCLEPOOL m_readyPool;
    
    void _checkDown();
    void _clearCircle();
    void _dealCircle();
    
    HARPOONTYPE m_eType;
};


#endif //SV_TRIPLEDOWN_HARPOON_H
