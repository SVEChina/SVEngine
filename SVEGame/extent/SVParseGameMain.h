//
// SVParseGameMain.h
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//

#ifndef SV_PARASEGAME_MAIN_H
#define SV_PARASEGAME_MAIN_H

#include "../base/SVGBase.h"

//解析游戏包的入口

class SVGameBase;

namespace sv {
    class SVParseGameBase;
    class SVParseGameMain : public SVGBase {
    public:
        SVParseGameMain(SVInst *_app);

        ~SVParseGameMain();

        bool parse(cptr8 _filename);

        SVGameBase* generate();

    protected:
        stringc m_path;
        stringc m_gametype;
        SVParseGameBase* m_pParseGame;
    };
//!namespace
}


#endif //SV_PARASEGAME_MAIN_H
