//
// SVProcess.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVProcess.h"

SVProcess::SVProcess(SVInst *_app)
:SVEventProc(_app) {
    m_subProcType = "SVProcess";

}

SVProcess::~SVProcess() {
}
