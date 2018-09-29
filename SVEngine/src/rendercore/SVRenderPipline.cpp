//
// SVRenderPipline.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVRenderPipline.h"
#include "SVRenderStream.h"
#include "SVRenderCmd.h"
#include "SVRenderMgr.h"
#include "../app/SVInst.h"
#include "../base/SVLock.h"
#include <sys/time.h>
//
SVRenderPipline::SVRenderPipline(SVInst *_app)
:SVGBase(_app) {
    for (s32 i = RST_BEGIN; i <= RST_END; i++) {
        SVRenderStreamPtr t_stream = MakeSharedPtr<SVRenderStream>();
        m_rstream_vec.append(t_stream);
    }
}

SVRenderPipline::~SVRenderPipline() {
    m_rstream_vec.destroy();
}

void SVRenderPipline::render() {
    for (s32 i = RST_BEGIN; i <= RST_END; i++) {
        m_rstream_vec[i]->render();
    }
    glFlush();
}

void SVRenderPipline::pushRenderCmd(RENDERSTREAMTYPE _rst, SVRenderCmdPtr _rcmd) {
    if (_rst >= RST_BEGIN && _rst <= RST_END) {
        m_rstream_vec[_rst]->addSVRenderCmd(_rcmd);
    }
}

void SVRenderPipline::clearRenderCmd(){
    for (s32 i = RST_BEGIN; i <= RST_END; i++) {
        m_rstream_vec[i]->clearSVRenderCmd();
        m_rstream_vec[i] = nullptr;
    }
}
