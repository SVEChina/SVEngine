//
//  SVPythonSys.cpp
//  SVEngine
//
//  Created by 付一洲 on 2019/2/28.
//  Copyright © 2019 李晓帆. All rights reserved.
//

#include "SVPythonSys.h"

SVPythonSys::SVPythonSys(SVInst *_app)
:SVSysBase(_app) {
    m_subsysType = "SVBasicSys";
}

SVPythonSys::~SVPythonSys() {
}

void SVPythonSys::init() {
    //
    //Py_Initialize();    // 初始化
}

void SVPythonSys::destroy() {
}

void SVPythonSys::update(f32 dt) {
}
