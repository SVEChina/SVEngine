//
// SVParseFilter.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVParseFilter.h"
#include "../basesys/filter/SVFilterShinning.h"
#include "../basesys/filter/SVADFilterBase.h"
#include "../mtl/SVMtlShinning.h"
#include "../mtl/SVMtlBlackWhite.h"
#include "../mtl/SVMtlStreak.h"
SVFilterBasePtr
SVParseFilter::parseFilter(SVInst *_app, RAPIDJSON_NAMESPACE::Value &item, s32 _resid, cptr8 _path) {
    SVString mtlName;
    if (item.HasMember("name") && item["name"].IsString()) {
        RAPIDJSON_NAMESPACE::Value &name = item["name"];
        mtlName = name.GetString();
    }
    SVMtlCorePtr t_mtl = nullptr;
    if (mtlName == "filtershinning") {
        t_mtl = MakeSharedPtr<SVMtlShinning>(_app);//闪光滤镜
    }else if (mtlName == "filterblackwhite"){
        t_mtl = MakeSharedPtr<SVMtlBlackWhite>(_app);//黑白滤镜
    }else if (mtlName == "filterstreak"){
        t_mtl = MakeSharedPtr<SVMtlStreak>(_app);//条纹滤镜
    }
    SVADFilterBasePtr t_filter = MakeSharedPtr<SVADFilterBase>(_app, t_mtl);
    return t_filter;
}


