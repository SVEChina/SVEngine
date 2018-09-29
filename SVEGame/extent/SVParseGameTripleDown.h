//
// SVParseGameTripleDown.h
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//

#ifndef SV_PARSEGAME_TRIPLEDOWN_H
#define SV_PARSEGAME_TRIPLEDOWN_H

#include "SVParseGameBase.h"
#include "../base/SVVector2d.h"

//解析FaceGame的表
class SVUIParamUI;
namespace sv{
    class SVParseGameTripleDown : public SVParseGameBase {
    public:
        SVParseGameTripleDown(SVInst *_app);
        
        ~SVParseGameTripleDown();
    public:
        bool parse(RAPIDJSON_NAMESPACE::Document& doc,cptr8 _filename);
        
        SVGameBase* generate();

        
    private:
        stringc m_SceneName;
        
        void generate(int resid, const char *scenename, char *path);

        stringc m_gameName;
        f32 m_gametime;   //游戏时间长度
        s32 m_donutHeight;
        s32 m_donutWidth;
        
        stringc m_donutLeftName;
        stringc m_donutMidName;
        stringc m_donutRightName;
        vector2di m_donutSize;
        vector2di m_harpoonSize;
        vector2di m_harpoonLeftPos;
        vector2di m_harpoonMidPos;
        vector2di m_harpoonRightPos;
        stringc   m_harpoonLeftName;
        stringc   m_harpoonMidName;
        stringc   m_harpoonRightName;
        //表情的动画
        SVUIParamUI* m_pUIParamUI;
    };
}


#endif //TESTMAGICENGINE_MAPARSEGAMETRIPLEDOWN_H
