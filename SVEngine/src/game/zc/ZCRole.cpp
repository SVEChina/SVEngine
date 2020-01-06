//
//  ZCRole.cpp
//

#include "ZCRole.h"

using namespace zc;

ZCRole::ZCRole(SVInst *_app)
:SVGBase(_app){
    m_isLock = true;
    m_level = E_LEVEL_BASE_1;
}

ZCRole::~ZCRole(){
}

void ZCRole::unlock() {
    m_isLock = false;
}

bool ZCRole::islock() {
    return m_isLock;
}
