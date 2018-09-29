//
// SVParseDef.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PARSEDEF_H
#define SV_PARSEDEF_H

#include "../third/rapidjson/document.h"
#include "../third/rapidjson/stringbuffer.h"
#include "../third/rapidjson/writer.h"
#include "../base/SVObject.h"
#include "../node/SVNodeDeclare.h"

typedef SVNodePtr (*prase_content_fun)(sv::SVInst *app,
                                  RAPIDJSON_NAMESPACE::Value &item,
                                  s32 _resid,
                                  cptr8 _path);

#endif //SV_PARSEDEF_H
