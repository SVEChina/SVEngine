//
// SVParseMusic.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVParseMusic.h"
#include "../module/SVEffectMusic.h"
#include "../app/SVInst.h"
SVEffectMusicPtr SVParseMusic::parseMusic(SVInst *app, RAPIDJSON_NAMESPACE::Value &item, s32 _resid, cptr8 _path){
    SVEffectMusicPtr effectMusic = MakeSharedPtr<SVEffectMusic>(app);
    effectMusic->setRootPath(_path);
    effectMusic->fromJSON(item);
    return effectMusic;
}
