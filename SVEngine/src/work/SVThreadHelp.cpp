// SVThreadHelp.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include <unistd.h>
#include "SVThreadHelp.h"

SVThreadHelp::SVThreadHelp(SVInst *_app)
: SVThreadWork(_app,"SVThreadHelp") {
}

SVThreadHelp::~SVThreadHelp() {
    SV_LOG_INFO("SVThreadHelp::~SVThreadHelp\n");
}

void SVThreadHelp::_innerUpdateBegin() {
    int a = 0;
}

void SVThreadHelp::_innerUpdate() {
    int a = 0;
}

void SVThreadHelp::_innerUpdateEnd() {
    int a = 0;
}

bool SVThreadHelp::pushThreadOp(SVOpBasePtr _op){
    SVThreadWork::pushThreadOp(_op);
    notice();
    return true;
}

bool SVThreadHelp::_checkAutoWait(){
    if( !m_run ) {
        return false;
    }
    if( m_opPoolWait->size()>0 || m_opPoolRun->size()>0 ){
        return false;
    }
    return true;
}
