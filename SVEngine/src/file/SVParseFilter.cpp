//
// SVParseFilter.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVParseFilter.h"
#include "../basesys/filter/SVFilterShinning.h"
SVFilterBasePtr
SVParseFilter::parseFilter(SVInst *_app, RAPIDJSON_NAMESPACE::Value &item, s32 _resid, cptr8 _path) {
    SVFilterShinningPtr t_filter = MakeSharedPtr<SVFilterShinning>(_app);
    return t_filter;
}


