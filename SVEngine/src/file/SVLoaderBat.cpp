//
// SVLoaderBat.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVLoaderBat.h"
#include "../app/SVInst.h"
#include "../base/SVDataChunk.h"
#include "SVFileMgr.h"

SVLoaderBat::SVLoaderBat(SVInst *_app)
:SVFileLoader(_app) {
}

bool SVLoaderBat::loadFromFile(cptr8 _filename) {
    SVDataChunk tSVDataChunk;
    bool t_flag = mApp->getFileMgr()->loadFileContent(&tSVDataChunk, _filename);
    if (t_flag) {
        //
        int a = 0;
    }
    return true;
}
