//
// Created by yinlong on 17/10/20.
//

#ifndef TESTMAGICENGINE_SVPARSEGAMECOMMON_H
#define TESTMAGICENGINE_SVPARSEGAMECOMMON_H

#include "SVParseData.h"
#include "../ui/SVUIParamBase.h"

class SVInst;
class SVUIParamUI;
class SVGameUIMgr;
class SVParseGameCommon  {
public:
    static SVUIParamBase* parseGameUi(SVInst* _app , RAPIDJSON_NAMESPACE::Value& GUIItem ,cptr8 _path);
    
};


#endif //TESTMAGICENGINE_SVPARSEGAMECOMMON_H
