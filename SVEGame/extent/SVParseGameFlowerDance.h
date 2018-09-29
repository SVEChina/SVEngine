
//
// SVParseGameFlowerDance.h
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//

#ifndef SVParseGameFlowerDance_h
#define SVParseGameFlowerDance_h


#include "SVParseGameBase.h"
#include "../base/SVVector2d.h"

//解析FaceGame的表
class SVUIParamUI;
class SVNode;
namespace sv{
    class SVParticleNode;
    class SVParseGameFlowerDance : public SVParseGameBase {
    public:
        SVParseGameFlowerDance(SVInst *_app);
        
        ~SVParseGameFlowerDance();
    public:
        bool parse(RAPIDJSON_NAMESPACE::Document& doc,cptr8 _filename);
        
        SVGameBase* generate();
        
    private:
        stringc m_SceneName;
        
        void generate(int resid, const char *scenename, char *path);
        
        stringc m_gameName;
        SVParticleNode* m_pHuaFenNode;
        
        SVUIParamUI* m_pUIParamUI;
    };
}

#endif /* SVParseGameFlowerDance_h */
