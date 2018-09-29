//
// SVTripleDownPlaySys.h
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//

#ifndef SV_TRIPLEDOWN_PLAYSYS_H
#define SV_TRIPLEDOWN_PLAYSYS_H

#include "../event/SVEventProc.h"
#include "../base/SVVector3d.h"
#include "SVTripleDownDef.h"

class SVTripleDownMgr;
class SVTripleDownUnit;
class SVTripleDownHarpoon;
class SVSpineNode;
class SVTripleDownPlaySys : public SVEventProc {
    friend class SVParseGameTripleDown;
public:
    SVTripleDownPlaySys(SVInst *_app , SVTripleDownMgr* pUnitMgr);

    ~SVTripleDownPlaySys();
    
    virtual void init();
    
    virtual void destroy();
    
    virtual void update(float _dt);
    
    virtual bool procEvent(SVEvent *_event);
    
    void    enter();
    void    exit();
protected:
    ////////////////////////////////
    stringc             m_strSceneName;
    stringc             m_strHarpoonAniLeft;
    stringc             m_strHarpoonAniMiddle;
    stringc             m_strHarpoonAniRight;
    stringc             m_strBgMusic;
    stringc             m_strXiaoChu;
    
    vector3df           m_posLeft;
    vector3df           m_posMiddle;
    vector3df           m_posRight;
    f32                 m_width;
    f32                 m_height;
    
    SVTripleDownUnit*   m_pCurGen;
    MOUTHSTATE          m_eMouthState;
    FACETOWARDS         m_eFaceTowards;
    SVTripleDownMgr*    m_pUnitMgr;
    
    typedef sv::array<stringc> TRIPLEANINAME;
    TRIPLEANINAME m_arrayAniName;
private:
    void _openMouth();
    void _closeMouth();
    void _updateCurrent();
    void _showHarpoonSelect(FACETOWARDS eTowards);
    SVTripleDownHarpoon* m_pHarpoonLeft;
    SVTripleDownHarpoon* m_pHarpoonRight;
    SVTripleDownHarpoon* m_pHarpoonMiddle;
    
    int m_iBgMusic;
};


#endif //SV_TRIPLEDOWN_PLAYSYS_H
