//
// SVParseGameBase.h
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//

#ifndef SV_PARASEGAME_BASE_H
#define SV_PARASEGAME_BASE_H

#include "../base/SVGBase.h"
#include "../third/rapidjson/document.h"

class SVGameBase;

namespace sv {
    
    class SVParseGameBase : public SVGBase {
    public:
        SVParseGameBase(SVInst *_app);
        
        ~SVParseGameBase();
        
        virtual bool parse(RAPIDJSON_NAMESPACE::Document& doc,cptr8 _filename);
        
        virtual SVGameBase* generate();
        
    protected:
        stringc m_path;
        stringc m_version;
    };
    
//!namespace
}


#endif //SV_PARASEGAME_BASE_H
