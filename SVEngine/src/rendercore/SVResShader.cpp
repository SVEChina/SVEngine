//
// SVResShader.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVResShader.h"
#include "../app/SVInst.h"
#include "../base/SVLock.h"
#include "renderer/SVRendererBase.h"

SVResShader::SVResShader(SVInst* _app)
:SVRObjBase(_app){
    m_programm = 0;
}

SVResShader::~SVResShader(){
}

void SVResShader::setProgrammeName(cptr8 _filename) {
    m_programme_fname = _filename;
}

cptr8 SVResShader::getProgrammeName(){
    return m_programme_fname.c_str();
}

void SVResShader::setVSFName(cptr8 _filename){
    m_vs_fname = _filename;
}

void SVResShader::setFSFName(cptr8 _filename){
    m_fs_fname = _filename;
}

void SVResShader::setGSFName(cptr8 _filename){
    m_gs_fname = _filename;
}

void SVResShader::setCSFName(cptr8 _filename){
    m_cs_fname = _filename;
}

void SVResShader::setTSCFName(cptr8 _filename){
    m_tsc_fname = _filename;
}

void SVResShader::setTSEFName(cptr8 _filename){
    m_tse_fname = _filename;
}

u32 SVResShader::getProgramm(){
    return m_programm;
}
