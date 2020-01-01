//
// SVMtlNocolor.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVMtlNocolor.h"
#include "../rendercore/SVRendererBase.h"

//
SVMtlNocolor::SVMtlNocolor(SVInst *_app)
:SVMtlCore(_app,"mvnocolor") {
    m_lerp = 0.0f;
    m_accTime = 0.0f;
    m_transTime = 5.0f;
}

SVMtlNocolor::SVMtlNocolor(SVMtlNocolor *_mtl)
:SVMtlCore(_mtl){
    m_lerp = _mtl->m_lerp;
    m_accTime = _mtl->m_accTime;
    m_transTime = _mtl->m_transTime;
}

SVMtlNocolor::~SVMtlNocolor() {
}

void SVMtlNocolor::update(f32 dt) {
    //这里来计算时间
    if (m_transTime == 0.0f) {
        m_lerp = 1.0f;  //黑白效果
    } else {
        m_accTime += dt;
        f32 t_value = m_accTime / m_transTime;
        m_lerp = sv::clamp(t_value, 0.0f, 1.0f);
    }
}

void SVMtlNocolor::_submitMtl(SVRendererBasePtr _render) {
    SVMtlCore::_submitMtl(_render);
    _render->submitUniformf("u_lerp", m_lerp);
}

//
SVMtlGeo3d::SVMtlGeo3d(SVInst *_app)
:SVMtlCore(_app,"geo3d") {//geo3d
    m_color.setColorARGB(0xff00ffff);
}

SVMtlGeo3d::SVMtlGeo3d(SVMtlGeo3d *_mtl)
:SVMtlCore(_mtl){
    m_color = _mtl->m_color;
}

SVMtlGeo3d::~SVMtlGeo3d() {
}

void SVMtlGeo3d::setColor(f32 _r,f32 _g,f32 _b,f32 _a){
    m_color.setColor(_r, _g, _b, _a);
}

void SVMtlGeo3d::_submitMtl(SVRendererBasePtr _render) {
    SVMtlCore::_submitMtl(_render);
    _render->submitUniformf4("u_color", m_color.r, m_color.g, m_color.b, m_color.a);
}

//坐标
SVMtlCoord::SVMtlCoord(SVInst *_app)
:SVMtlCore(_app,"normal3d1") {
}

SVMtlCoord::SVMtlCoord(SVMtlCoord *_mtl)
:SVMtlCore(_mtl){
}

SVMtlCoord::~SVMtlCoord() {
}

void SVMtlCoord::_submitMtl(SVRendererBasePtr _render) {
    SVMtlCore::_submitMtl(_render);
}

//网格
SVMtlNetGrid::SVMtlNetGrid(SVInst *_app)
:SVMtlCore(_app,"netgrid") {
    m_boder = 2.0f;
    m_gridsize = 10.0f;
    m_width = 100.0f;
    m_height = 100.0f;
    m_bg_color.setColor(0.01f,0.01f,0.01f,0.02f);
    m_border_color.setColor(0.2f,0.2f,0.2f,0.2f);
    m_line_color.setColor(0.1f,0.1f,0.1f,0.01f);
}

SVMtlNetGrid::SVMtlNetGrid(SVMtlNetGrid *_mtl)
:SVMtlCore(_mtl){
    m_boder = _mtl->m_boder;
    m_gridsize = _mtl->m_gridsize;
    m_width = _mtl->m_width;
    m_height = _mtl->m_height;
    m_bg_color = _mtl->m_bg_color;
    m_border_color = _mtl->m_border_color;
    m_line_color = _mtl->m_line_color;
}

SVMtlNetGrid::~SVMtlNetGrid() {
}

//uniform vec4 u_bg_color;
//uniform vec4 u_border_color;
//uniform vec4 u_line_color;

void SVMtlNetGrid::_submitMtl(SVRendererBasePtr _render) {
    SVMtlCore::_submitMtl(_render);
    _render->submitUniformf("u_gsize", m_gridsize);
    _render->submitUniformf("u_border", m_boder); //最小是2.0f
    _render->submitUniformf("u_tbl_w", m_width); //最小是2.0f
    _render->submitUniformf("u_tbl_h", m_height); //最小是2.0f
    //设置颜色
    _render->submitUniformf4("u_bg_color", m_bg_color.r,
                             m_bg_color.g,
                             m_bg_color.b,
                             m_bg_color.a);
    _render->submitUniformf4("u_border_color", m_border_color.r,
                             m_border_color.g,
                             m_border_color.b,
                             m_border_color.a);
    _render->submitUniformf4("u_line_color", m_line_color.r,
                             m_line_color.g,
                             m_line_color.b,
                             m_line_color.a);
}

void SVMtlNetGrid::setGridParam(s32 _gridsize,s32 _border) {
    m_boder = _border;
    m_gridsize = _gridsize;
}

void SVMtlNetGrid::setGridSize(s32 _width,s32 _height) {
    m_width = _width;
    m_height = _height;
}

void SVMtlNetGrid::setGridBGColor(f32 _r,f32 _g,f32 _b,f32 _a) {
    m_bg_color.setColor(_r,_g,_b,_a);
}

void SVMtlNetGrid::setGridLineColor(f32 _r,f32 _g,f32 _b,f32 _a) {
    m_line_color.setColor(_r,_g,_b,_a);
}

void SVMtlNetGrid::setGridBoderColor(f32 _r,f32 _g,f32 _b,f32 _a) {
    m_border_color.setColor(_r,_g,_b,_a);
}

//
SVMtlParticleAni::SVMtlParticleAni(SVInst *_app)
:SVMtlCore(_app,"particle_ani_ambient") {
    m_ani_trans.set(1.0f,1.0f,1.0f,0.5f);
    m_diffuse_color = FVec4(1.0f,8.0f,1.0f,1.0f);
    m_ambient_color  = FVec4(0.4f,0.4f,0.4f,1.0f);
    m_p_fade = 1.0f;
    m_diffuse_scale = 1.0f;
    m_p_radius = 1.0f;
}

SVMtlParticleAni::SVMtlParticleAni(SVMtlParticleAni *_mtl)
:SVMtlCore(_mtl){
    m_p_transform = _mtl->m_p_transform;
    m_ani_trans = _mtl->m_ani_trans;
    m_diffuse_color = _mtl->m_diffuse_color;
    m_diffuse_scale = _mtl->m_diffuse_scale;
    m_ambient_color = _mtl->m_ambient_color;
    m_p_fade = _mtl->m_p_fade;
    m_p_radius = _mtl->m_p_radius;
}

SVMtlParticleAni::~SVMtlParticleAni() {
}

void SVMtlParticleAni::_submitMtl(SVRendererBasePtr _render) {
    SVMtlCore::_submitMtl(_render);
    _render->submitUniformMatrix("s_p_transform", m_p_transform.get());
    _render->submitUniformf4v("animation_transform", m_ani_trans.get());
    _render->submitUniformf4v("diffuse_color", m_diffuse_color.get());
    _render->submitUniformf("diffuse_scale", m_diffuse_scale);
    _render->submitUniformf4v("s_ambient_color", m_ambient_color.get());
    _render->submitUniformf("s_particles_fade", m_p_fade);
    _render->submitUniformf("s_particles_radius", m_p_radius);
}


