//
// Created by lidm on 17/9/5.
//

#ifndef TESTMAGICENGINE_SVPARSEGAMEFACEDANCE_H
#define TESTMAGICENGINE_SVPARSEGAMEFACEDANCE_H

#include "SVParseData.h"
#include "../game/SVFaceDanceResConfig.h"
#include "../ui/SVUIParamBase.h"

//解析FaceGame的表
class SVParseGameFaceDance : public SVParseDataEvent {
public:
    static void parseGameFaceDanceStatic(SVInst *_app, RAPIDJSON_NAMESPACE::Value &item, s32 _resid,cptr8 _path);
public:
    SVParseGameFaceDance(SVInst *_app);

    ~SVParseGameFaceDance();
    
private:
    stringc m_SceneName;
    
    void generate(s32 _resid,cptr8 _path);
    
    //游戏规则
    bool m_teachmode;
    stringc m_gameName;
    stringc m_bgMusic;
    stringc m_deadEffect;
    stringc m_endEffect;
    stringc m_djsEffect;
    float m_maxSpeed;
    float m_minSpeed;
    float m_gametime;   //游戏时间长度
    float m_enery;
    float m_genrate;
    int m_score;
    //表情的动画
    stringc m_SpineName;
    stringc m_deadAni;
    stringc m_scorepng;
    array<stringc> m_aniNamePool;
    
    SVUIParamUI* m_pUIParamUI;
};


#endif //TESTMAGICENGINE_SVPARSEGAMEFACEDANCE_H
