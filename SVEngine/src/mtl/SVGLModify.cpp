//
// SVGLModify.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVGLModify.h"

//
SVGLModify::SVGLModify(SVInst *_app)
:SVModify(_app){
}

//
SVGLModifyTex::SVGLModifyTex(SVInst *_app)
:SVGLModify(_app){
    m_texChanel = 0;
    m_texID = 0;
}

void SVGLModifyTex::exec(SVObjectPtr _obj) {
}

//
SVGLModifyTexParam::SVGLModifyTexParam(SVInst *_app)
:SVGLModify(_app){
    m_texChanel = 0;
    m_texID = 0;
    m_paramType = 0;
    m_paramValue = 0;
}

void SVGLModifyTexParam::exec(SVObjectPtr _obj) {
}

//线宽
SVGLModifyLineWidth::SVGLModifyLineWidth(SVInst *_app)
:SVGLModify(_app){
    m_size = 1.0f;
}

void SVGLModifyLineWidth::exec(SVObjectPtr _obj) {
}

//点
SVGLModifyPointSize::SVGLModifyPointSize(SVInst *_app)
:SVGLModify(_app){
    m_size = 1.0f;
}

void SVGLModifyPointSize::exec(SVObjectPtr _obj) {
}

//
SVGLModifyBlend::SVGLModifyBlend(SVInst *_app)
:SVGLModify(_app){
}

void SVGLModifyBlend::exec(SVObjectPtr _obj){
}

//
SVGLModifyCullFace::SVGLModifyCullFace(SVInst *_app)
:SVGLModify(_app){
}

void SVGLModifyCullFace::exec(SVObjectPtr _obj){
}

//深度测试
SVGLModifyDepth::SVGLModifyDepth(SVInst *_app)
:SVGLModify(_app){
}

void SVGLModifyDepth::exec(SVObjectPtr _obj){
}

//修正模板
SVGLModifyStencil::SVGLModifyStencil(SVInst *_app)
:SVGLModify(_app){
}

void SVGLModifyStencil::exec(SVObjectPtr _obj){
}

//
SVGLModifyShader::SVGLModifyShader(SVInst *_app)
:SVGLModify(_app){
    m_shaderID = 0;
}

void SVGLModifyShader::exec(SVObjectPtr _obj) {
}

//
SVGLModifyUnif32::SVGLModifyUnif32(SVInst *_app)
:SVGLModify(_app){
}

void SVGLModifyUnif32::exec(SVObjectPtr _obj) {
}

//
SVGLModifyUnif32v2::SVGLModifyUnif32v2(SVInst *_app)
:SVGLModify(_app){
}

void SVGLModifyUnif32v2::exec(SVObjectPtr _obj) {
}

//
SVGLModifyUniMatrix::SVGLModifyUniMatrix(SVInst *_app)
:SVGLModify(_app){
}

void SVGLModifyUniMatrix::exec(SVObjectPtr _obj) {
}



