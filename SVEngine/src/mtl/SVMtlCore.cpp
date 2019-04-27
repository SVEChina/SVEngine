//
// SVMtlCore.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVMtlCore.h"
#include "SVGLModify.h"
#include "../operate/SVOpCreate.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/SVRenderScene.h"
#include "../rendercore/SVGL/SVRResGL.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/SVRendererBase.h"
#include "../rendercore/SVResShader.h"

SVMtlCoreParam::SVMtlCoreParam(){
    m_shader = "normal2d";
}

SVMtlCorePtr SVMtlCoreParam::genMtl(SVInst *_app){
    SVMtlCorePtr t_mtl = MakeSharedPtr<SVMtlCore>(_app,m_shader.c_str());
    return t_mtl;
}

//
SVMtlCore::SVMtlCore(SVInst *_app, cptr8 _shader)
:SVGBase(_app)
,m_pShader(nullptr)
,m_mtlname(_shader)
{
    reset();
    m_renderPool = new MODPOOL();
    m_logicPool = new MODPOOL();
}

SVMtlCore::SVMtlCore(SVMtlCore* _mtl)
:SVGBase(_mtl->mApp){
    m_renderPool = new MODPOOL();
    m_logicPool = new MODPOOL();
    m_mtlname = _mtl->m_mtlname;
    m_pShader = _mtl->m_pShader;
    m_LogicMtlFlag0 = _mtl->m_LogicMtlFlag0;
    m_LogicParamTex.copy(_mtl->m_LogicParamTex);
    m_LogicParamBlend.copy(_mtl->m_LogicParamBlend);
    m_LogicParamCull.copy(_mtl->m_LogicParamCull);
    m_LogicParamDepth.copy(_mtl->m_LogicParamDepth);
    m_LogicParamStencil.copy(_mtl->m_LogicParamStencil);
    m_LogicParamAlpha.copy(_mtl->m_LogicParamAlpha);
    m_LogicParamSize.copy(_mtl->m_LogicParamSize);
    m_LogicParamMatrix.copy(_mtl->m_LogicParamMatrix);
    m_LogicParamZOff.copy(_mtl->m_LogicParamZOff);
}

SVMtlCore::~SVMtlCore() {
    reset();
    //
    if(m_renderPool){
        m_renderPool->destroy();
        delete m_renderPool;
        m_renderPool = nullptr;
    }
    if(m_logicPool){
        m_logicPool->destroy();
        delete m_logicPool;
        m_logicPool = nullptr;
    }
}

SVMtlCorePtr SVMtlCore::clone() {
    return PointerSharedPtr<SVMtlCore>(new SVMtlCore(this));
}

void SVMtlCore::reset() {
    m_pShader = nullptr;
    m_LogicMtlFlag0 = 0;
    m_LogicParamTex.reset();
    m_LogicParamBlend.reset();
    m_LogicParamCull.reset();
    m_LogicParamDepth.reset();
    m_LogicParamStencil.reset();
    m_LogicParamAlpha.reset();
    m_LogicParamSize.reset();
    m_LogicParamMatrix.reset();
    m_LogicParamZOff.reset();
}

void SVMtlCore::setModelMatrix(f32 *_mat) {
    memcpy(m_LogicParamMatrix.m_mat_model, _mat, sizeof(f32) * 16);
    m_LogicMtlFlag0 |= MTL_F0_MAT_M;
}

void SVMtlCore::setTexcoordFlip(f32 _x, f32 _y) {
    for(s32 i=0;i<MAX_TEXUNIT;i++){
        m_LogicParamTex.setTexClip(i,_x,_y);
    }
    m_LogicMtlFlag0 |= MTL_F0_TEX_FLIP;
}

void SVMtlCore::setTextureParam(s32 _chanel,TEXTUREPARAM _type,s32 _value) {
    if(_chanel>=0 && _chanel<MAX_TEXUNIT) {
        if(_type == E_T_PARAM_FILTER_MAG) {
            //filter_max
            m_LogicParamTex.m_texUnit[_chanel].m_mag_filter = _value;
        }else if(_type == E_T_PARAM_FILTER_MIN) {
            //filter_min
            m_LogicParamTex.m_texUnit[_chanel].m_min_filter = _value;
        }else if(_type == E_T_PARAM_WRAP_S) {
            //wrap_s
            m_LogicParamTex.m_texUnit[_chanel].m_s_wrap = _value;
        }else if(_type == E_T_PARAM_WRAP_T) {
            //wrap_t
            m_LogicParamTex.m_texUnit[_chanel].m_t_wrap = _value;
        }
    }
}

//逻辑更新
void SVMtlCore::update(f32 dt) {
}

void SVMtlCore::reloadShader(cptr8 _shader){
    m_mtlname = _shader;
    m_pShader = nullptr;
}

//渲染更新(跑渲染参数)
bool SVMtlCore::submitMtl() {
//    //单线程(交换改变池)
//    swap();
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(!t_renderer)
        return false;
    if (!m_pShader){
        _loadShader();//加载shader
        if(!m_pShader){
            SV_LOG_INFO("SHADER ERROR %s \n",m_mtlname.c_str());
            return false;
        }
    }
    _refreshMatrix();
    _refreshModify();
    //提交shader
    if(m_pShader) {
        m_pShader->active(t_renderer);
    }
    _submitUniform(t_renderer);
    _submitState(t_renderer);
    _submitMtl(t_renderer);
    return true;
}

void SVMtlCore::recoverMtl() {
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(!t_renderer)
        return ;
    //状态回滚 先
    //融合
    if((m_LogicMtlFlag0&MTL_F0_BLEND)>0){
        m_LogicParamBlend.enable = false;
        t_renderer->submitBlend(m_LogicParamBlend);
    }
    //隐藏面消除
    if((m_LogicMtlFlag0&MTL_F0_CULL)>0){
        m_LogicParamCull.enable = false;
        t_renderer->submitCull(m_LogicParamCull);
    }
    //模板测试
    if((m_LogicMtlFlag0&MTL_F0_STENCIL)>0){
        m_LogicParamStencil.enable = false;
        t_renderer->submitStencil(m_LogicParamStencil);
    }
    //alpha测试
    if((m_LogicMtlFlag0&MTL_F0_ALPHA)>0){
    }
    //深度测试
    if((m_LogicMtlFlag0&MTL_F0_DEPTH)>0){
        m_LogicParamDepth.enable = false;
        t_renderer->submitDepth(m_LogicParamDepth);
    }
    //Z冲突
    if((m_LogicMtlFlag0&MTL_F0_ZOFF)>0){
        m_LogicParamZOff.enable = false;
        t_renderer->submitZOff(m_LogicParamZOff);
    }
}

//增加修正
void SVMtlCore::addModify(SVModifyPtr _modify) {
    if(_modify && m_logicPool){
        m_logicPool->append(_modify);
    }
}

//交换
void SVMtlCore::swap() {
    if(m_renderPool && m_logicPool) {
        MODPOOL* tmp = m_renderPool;
        m_renderPool = m_logicPool;
        m_logicPool = tmp;
    }
}

void SVMtlCore::_loadShader() {
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(t_renderer){
        m_pShader = mApp->getShaderMgr()->getShader(m_mtlname.c_str());
    }
}

void SVMtlCore::_refreshMatrix(){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(t_renderer){
        FMat4 t_mat_view = t_renderer->getViewMat();
        memcpy(m_LogicParamMatrix.m_mat_view, t_mat_view.get(), sizeof(f32) * 16);
        m_LogicMtlFlag0 |= MTL_F0_MAT_V;
        
        FMat4 t_mat_proj = t_renderer->getProjMat();
        memcpy(m_LogicParamMatrix.m_mat_project, t_mat_proj.get(), sizeof(f32) * 16);
        m_LogicMtlFlag0 |= MTL_F0_MAT_P;
        
        FMat4 t_mat_vp = t_renderer->getVPMat();
        memcpy(m_LogicParamMatrix.m_mat_vp, t_mat_vp.get(), sizeof(f32) * 16);
        m_LogicMtlFlag0 |= MTL_F0_MAT_VP;
    }
}

//执行修正系统
void SVMtlCore::_refreshModify(){
    if(m_renderPool){
        for(s32 i=0;i<m_renderPool->size();i++) {
            (*m_renderPool)[i]->exec( shared_from_this() );
        }
        m_renderPool->destroy();
    }
}

void SVMtlCore::_submitUniform(SVRendererBasePtr _render) {
    //get uniform
    if((m_LogicMtlFlag0&MTL_F0_MAT_M)>0){
        _render->submitUniformMatrix(NAME_M_MATRIX, m_LogicParamMatrix.m_mat_model);
    }
    //
    if((m_LogicMtlFlag0&MTL_F0_MAT_V)>0){
        _render->submitUniformMatrix(NAME_V_MATRIX, m_LogicParamMatrix.m_mat_view);
    }
    //
    if((m_LogicMtlFlag0&MTL_F0_MAT_P)>0){
        _render->submitUniformMatrix(NAME_P_MATRIX, m_LogicParamMatrix.m_mat_project);
    }
    //
    if((m_LogicMtlFlag0&MTL_F0_MAT_VP)>0){
        _render->submitUniformMatrix(NAME_VP_MATRIX, m_LogicParamMatrix.m_mat_vp);
    }
    //
    if((m_LogicMtlFlag0&MTL_F0_TEX_SIZE)>0){
        _render->submitUniformf2v(NAME_TEXSIZE_0, m_LogicParamSize.m_tex0size);
    }
    //
    if((m_LogicMtlFlag0&MTL_F1_TEX_SIZE)>0){
        _render->submitUniformf2v(NAME_TEXSIZE_1, m_LogicParamSize.m_tex1size);
    }
    //
    if((m_LogicMtlFlag0&MTL_F2_TEX_SIZE)>0){
        _render->submitUniformf2v(NAME_TEXSIZE_2, m_LogicParamSize.m_tex2size);
    }
    //
    if((m_LogicMtlFlag0&MTL_F3_TEX_SIZE)>0){
        _render->submitUniformf2v(NAME_TEXSIZE_3, m_LogicParamSize.m_tex3size);
    }
    //
    if((m_LogicMtlFlag0&MTL_F0_POINT_SIZE)>0){
        _render->submitUniformf(NAME_POINTSIZE, m_LogicParamSize.m_ptsize);
    }
}

void SVMtlCore::_submitState(SVRendererBasePtr _render) {
    //更新纹理
    if((m_LogicMtlFlag0&MTL_F0_TEX0)>0){
        _render->submitTex(0, m_LogicParamTex.m_texUnit[0]);
    }
    if((m_LogicMtlFlag0&MTL_F0_TEX1)>0){
        _render->submitTex(1, m_LogicParamTex.m_texUnit[1]);
    }
    if((m_LogicMtlFlag0&MTL_F0_TEX2)>0){
        _render->submitTex(2, m_LogicParamTex.m_texUnit[2]);
    }
    if((m_LogicMtlFlag0&MTL_F0_TEX3)>0){
        _render->submitTex(3, m_LogicParamTex.m_texUnit[3]);
    }
    if((m_LogicMtlFlag0&MTL_F0_TEX4)>0){
        _render->submitTex(4, m_LogicParamTex.m_texUnit[4]);
    }
    if((m_LogicMtlFlag0&MTL_F0_TEX5)>0){
        _render->submitTex(5, m_LogicParamTex.m_texUnit[5]);
    }
    if((m_LogicMtlFlag0&MTL_F0_TEX6)>0){
        _render->submitTex(6, m_LogicParamTex.m_texUnit[6]);
    }
    if((m_LogicMtlFlag0&MTL_F0_TEX7)>0){
        _render->submitTex(7, m_LogicParamTex.m_texUnit[7]);
    }
    //
    if((m_LogicMtlFlag0&MTL_F0_LINE_SIZE)>0){
        _render->submitLineWidth(m_LogicParamSize.m_linewidth);
    }
    //融合
    if((m_LogicMtlFlag0&MTL_F0_BLEND)>0){
        _render->submitBlend(m_LogicParamBlend);
    }
    //隐藏面消除
    if((m_LogicMtlFlag0&MTL_F0_CULL)>0){
        _render->submitCull(m_LogicParamCull);
    }
    //模板测试
    if((m_LogicMtlFlag0&MTL_F0_STENCIL)>0){
        _render->submitStencil(m_LogicParamStencil);
    }
    //alpha测试
    if((m_LogicMtlFlag0&MTL_F0_ALPHA)>0){
    }
    //深度测试
    if((m_LogicMtlFlag0&MTL_F0_DEPTH)>0){
        _render->submitDepth(m_LogicParamDepth);
    }
    //Z冲突
    if((m_LogicMtlFlag0&MTL_F0_ZOFF)>0){
        _render->submitZOff(m_LogicParamZOff);
    }else{
        _render->submitZOff(m_LogicParamZOff);
    }
}

void SVMtlCore::_submitMtl(SVRendererBasePtr _render) {
}

void SVMtlCore::setTexture(s32 _chanel,SVTexturePtr _texture) {
    if(_chanel<0 || _chanel>=MAX_TEXUNIT)
        return;
    m_LogicParamTex.setTexture(_chanel, _texture);
    s32 t_flag = MTL_F0_TEX0;
    t_flag = t_flag<<_chanel;
    m_LogicMtlFlag0 |= t_flag;
}

void SVMtlCore::setTexture(s32 _chanel,SVTEXTYPE _from) {
    if(_chanel<0 || _chanel>=MAX_TEXUNIT)
        return;
    m_LogicParamTex.setTexture(_chanel, _from);
    s32 t_flag = MTL_F0_TEX0;
    t_flag = t_flag<<_chanel;
    m_LogicMtlFlag0 |= t_flag;
}

void SVMtlCore::setTexSizeIndex(s32 index, f32 _w, f32 _h) {
    if (index == 0) {
        m_LogicParamSize.m_tex0size[0] = _w;
        m_LogicParamSize.m_tex0size[1] = _h;
        m_LogicMtlFlag0 |= MTL_F0_TEX_SIZE;
    } else if (index == 1) {
        m_LogicParamSize.m_tex1size[0] = _w;
        m_LogicParamSize.m_tex1size[1] = _h;
        m_LogicMtlFlag0 |= MTL_F1_TEX_SIZE;
    } else if (index == 2) {
        m_LogicParamSize.m_tex2size[0] = _w;
        m_LogicParamSize.m_tex2size[1] = _h;
        m_LogicMtlFlag0 |= MTL_F2_TEX_SIZE;
    } else if (index == 3) {
        m_LogicParamSize.m_tex3size[0] = _w;
        m_LogicParamSize.m_tex3size[1] = _h;
        m_LogicMtlFlag0 |= MTL_F3_TEX_SIZE;
    }
}

void SVMtlCore::setLineSize(f32 _linewidth){
    if(m_LogicParamSize.m_linewidth!=_linewidth) {
        m_LogicParamSize.m_linewidth = _linewidth;
        m_LogicParamSize.m_linewidth = _linewidth;
        m_LogicMtlFlag0 |= MTL_F0_LINE_SIZE;
    }
}

void SVMtlCore::setBlendState(s32 _src , s32 _dst){
    m_LogicParamBlend.srcParam = _src;
    m_LogicParamBlend.dstParam = _dst;
}

void SVMtlCore::setBlendEnable(bool _bBlendEnable){
    m_LogicParamBlend.enable = _bBlendEnable;
    m_LogicMtlFlag0 |= MTL_F0_BLEND;
}

void SVMtlCore::setCullEnable(bool _bCullEnable){
    m_LogicParamCull.enable = _bCullEnable;
    m_LogicMtlFlag0 |= MTL_F0_CULL;
}

void SVMtlCore::setCullFace(s32 _frontFace, s32 _cullFace){
    m_LogicParamCull.frontFace = _frontFace;
    m_LogicParamCull.cullFace = _cullFace;
}

void SVMtlCore::setDepthEnable(bool _bDepthEnable){
    m_LogicParamDepth.enable = _bDepthEnable;
    m_LogicMtlFlag0 |= MTL_F0_DEPTH;
}

void SVMtlCore::setZOffEnable(bool _enable) {
    m_LogicParamZOff.enable = _enable;
    m_LogicMtlFlag0 |= MTL_F0_ZOFF;
}

void SVMtlCore::setZOffParam(f32 _factor,f32 _unit) {
    m_LogicParamZOff.m_factor = _factor;
    m_LogicParamZOff.m_unit = _unit;
}

void SVMtlCore::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator,
                       RAPIDJSON_NAMESPACE::Value &_objValue){
    _toJsonData(_allocator, _objValue);
}

void SVMtlCore::fromJSON(RAPIDJSON_NAMESPACE::Value &item){
    _fromJsonData(item);
}

void SVMtlCore::_toJsonData(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator,
                 RAPIDJSON_NAMESPACE::Value &locationObj){
    //blending
    RAPIDJSON_NAMESPACE::Value blendObj(RAPIDJSON_NAMESPACE::kObjectType);
    blendObj.AddMember("enable", m_LogicParamBlend.enable, _allocator);
    blendObj.AddMember("src", m_LogicParamBlend.srcParam, _allocator);
    blendObj.AddMember("dst", m_LogicParamBlend.dstParam, _allocator);
    //shader
    locationObj.AddMember("shader", RAPIDJSON_NAMESPACE::StringRef(m_mtlname.c_str()), _allocator);
    //parameter
    RAPIDJSON_NAMESPACE::Value parameterObj(RAPIDJSON_NAMESPACE::kObjectType);
    parameterObj.AddMember("depth", m_LogicParamDepth.enable, _allocator);
    RAPIDJSON_NAMESPACE::Value cullObj(RAPIDJSON_NAMESPACE::kObjectType);
    //cull
    cullObj.AddMember("enable", m_LogicParamCull.enable, _allocator);
    cullObj.AddMember("frontFace", m_LogicParamCull.frontFace, _allocator);
    cullObj.AddMember("cullFace", m_LogicParamCull.cullFace, _allocator);
    parameterObj.AddMember("cull", cullObj, _allocator);
}

void SVMtlCore::_fromJsonData(RAPIDJSON_NAMESPACE::Value &item){
    //blending
    if (item.HasMember("blend") && item["blend"].IsObject()) {
        RAPIDJSON_NAMESPACE::Value &t_blend = item["blend"];
        if (t_blend.HasMember("enable") && t_blend["enable"].IsBool()) {
            bool enable = t_blend["enable"].GetBool();
            setBlendEnable(enable);
        }
        s32 src = 1;
        s32 dst = 1;
        if (t_blend.HasMember("src") && t_blend["src"].IsInt()) {
            src = t_blend["src"].GetInt();
        }
        if (t_blend.HasMember("dst") && t_blend["dst"].IsInt()) {
            dst = t_blend["dst"].GetInt();
        }
        setBlendState(src, dst);
    }
    //shader
    if (item.HasMember("shader") && item["shader"].IsString()) {
        SVString shader = item["shader"].GetString();
        reloadShader(shader);
    }
    //parameter
    if (item.HasMember("parameter") && item["parameter"].IsObject()) {
        RAPIDJSON_NAMESPACE::Value &t_paprmeter = item["parameter"];
        if (t_paprmeter.HasMember("depth") && t_paprmeter["depth"].IsBool()) {
            bool enable = t_paprmeter["depth"].GetBool();
            setDepthEnable(enable);
        }
        if (t_paprmeter.HasMember("cull") && t_paprmeter["cull"].IsObject()) {
            RAPIDJSON_NAMESPACE::Value &t_cull = item["cull"];
            if (t_cull.HasMember("enable") && t_cull["enable"].IsBool()) {
                bool enable = t_cull["enable"].GetBool();
                setCullEnable(enable);
            }
            s32 t_frontFace = 1;
            s32 t_cullFace = 1;
            if (t_cull.HasMember("frontFace") && t_cull["frontFace"].IsInt()) {
                t_frontFace = t_cull["frontFace"].GetInt();
            }
            if (t_cull.HasMember("cullFace") && t_cull["cullFace"].IsInt()) {
                t_cullFace = t_cull["cullFace"].GetInt();
            }
            setCullFace(t_frontFace, t_cullFace);
        }
    }
    //textures
    if (item.HasMember("textures") && item["textures"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &t_textures = item["textures"];
        for (s32 i = 0; i<t_textures.Size(); i++) {
            RAPIDJSON_NAMESPACE::Value &t_texture = t_textures[i];
            if(t_texture.HasMember("texture") && t_texture["texture"].IsString()){
                SVString textureName = t_texture["texture"].GetString();
            }
            if(t_texture.HasMember("channel") && t_texture["channel"].IsInt()){
                s32 channel = t_texture["channel"].GetInt();
            }
            if(t_texture.HasMember("internalformat") && t_texture["internalformat"].IsInt()){
                s32 informat = t_texture["internalformat"].GetInt();
            }
            if(t_texture.HasMember("format") && t_texture["format"].IsInt()){
                s32 format = t_texture["format"].GetInt();
            }
            if(t_texture.HasMember("sampler") && t_texture["sampler"].IsObject()){
                RAPIDJSON_NAMESPACE::Value &sampler = t_texture["sampler"];
                if(sampler.HasMember("magFilter") && sampler["magFilter"].IsInt()){
                    s32 magFilter = sampler["magFilter"].GetInt();
                }
                if(sampler.HasMember("minFilter") && sampler["minFilter"].IsInt()){
                    s32 minFilter = sampler["minFilter"].GetInt();
                }
                if(sampler.HasMember("wrapS") && sampler["wrapS"].IsInt()){
                    s32 wrapS = sampler["wrapS"].GetInt();
                }
                if(sampler.HasMember("wrapT") && sampler["wrapT"].IsInt()){
                    s32 wrapT = sampler["wrapT"].GetInt();
                }
            }
        }
    }
}
