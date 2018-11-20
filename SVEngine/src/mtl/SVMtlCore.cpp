//
// SVMtlCore.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#include "SVMtlCore.h"
#include "../rendercore/SVRenderScene.h"
#include "../operate/SVOpCreate.h"
#include "../mtl/SVTexture.h"
#include "../rendercore/SVRenderMgr.h"
#include "../rendercore/renderer/SVRendererBase.h"
#include "SVGLModify.h"

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
,m_mtlname(_shader){
    //
    reset();
    //
    m_renderPool = new MODPOOL();
    m_logicPool = new MODPOOL();
}

SVMtlCore::SVMtlCore(SVMtlCore* _mtl)
:SVGBase(_mtl->mApp){
    //
    m_renderPool = new MODPOOL();
    m_logicPool = new MODPOOL();
    //
    m_mtlname = _mtl->m_mtlname;
    m_programID = _mtl->m_programID;
    m_LogicMtlFlag0 = _mtl->m_LogicMtlFlag0;
    m_LogicParamTex.copy(_mtl->m_LogicParamTex);
    m_LogicParamBlend.copy(_mtl->m_LogicParamBlend);
    m_LogicParamCull.copy(_mtl->m_LogicParamCull);
    m_LogicParamDepth.copy(_mtl->m_LogicParamDepth);
    m_LogicParamStencil.copy(_mtl->m_LogicParamStencil);
    m_LogicParamAlpha.copy(_mtl->m_LogicParamAlpha);
    m_LogicParamSize.copy(_mtl->m_LogicParamSize);
    m_LogicParamMatrix.copy(_mtl->m_LogicParamMatrix);
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
    m_programID = 0;
    m_LogicMtlFlag0 = 0;
    m_LogicParamTex.reset();
    m_LogicParamBlend.reset();
    m_LogicParamCull.reset();
    m_LogicParamDepth.reset();
    m_LogicParamStencil.reset();
    m_LogicParamAlpha.reset();
    m_LogicParamSize.reset();
    m_LogicParamMatrix.reset();
}

void SVMtlCore::setModelMatrix(f32 *_mat) {
//    SVGLModifyUniMatrixPtr t_mod = MakeSharedPtr<SVGLModifyUniMatrix>(mApp);
//    t_mod->m_shaderID = m_programID;
//    t_mod->m_name = NAME_M_MATRIX;
//    memcpy(t_mod->m_mat_model, _mat, sizeof(f32) * 16);
//    addModify(t_mod);
    memcpy(m_LogicParamMatrix.m_mat_model, _mat, sizeof(f32) * 16);
    m_LogicMtlFlag0 |= MTL_F0_MAT_M;
}

void SVMtlCore::setTexcoordFlip(f32 _x, f32 _y) {
    for(s32 i=0;i<MAX_TEXUNIT;i++){
        m_LogicParamTex.setTexClip(i,_x,_y);
    }
    m_LogicMtlFlag0 |= MTL_F0_TEX_FLIP;
}

//逻辑更新
void SVMtlCore::update(f32 dt) {
}

void SVMtlCore::reloadShader(cptr8 _shader){
    m_mtlname = _shader;
    m_programID = 0;
}

//渲染更新(跑渲染参数)
bool SVMtlCore::submitMtl() {
//    //单线程(交换改变池)
//    swap();
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if(!t_renderer)
        return false;
    if (m_programID == 0){
        _loadShader();//加载shader
        if(m_programID == 0){
            SV_LOG_INFO("SHADER ERROR %s \n",m_mtlname.c_str());
            return false;
        }
    }
    _refreshMatrix();
    _refreshModify();
    //提交shader
    t_renderer->submitShader(m_programID);
    _submitUniform(t_renderer);
    _submitState(t_renderer);
    _submitMtl(t_renderer);
    return true;
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
    if(!t_renderer)
        return ;
    m_programID = mApp->getShaderMgr()->getProgramme(m_mtlname.c_str());
}

void SVMtlCore::_refreshMatrix(){
    FMat4 t_mat_view = mApp->getRenderMgr()->getViewMat();
    memcpy(m_LogicParamMatrix.m_mat_view, t_mat_view.get(), sizeof(f32) * 16);
    m_LogicMtlFlag0 |= MTL_F0_MAT_V;
    
    FMat4 t_mat_proj = mApp->getRenderMgr()->getProjMat();
    memcpy(m_LogicParamMatrix.m_mat_project, t_mat_proj.get(), sizeof(f32) * 16);
    m_LogicMtlFlag0 |= MTL_F0_MAT_P;
    
    FMat4 t_mat_vp = mApp->getRenderMgr()->getVPMat();
    memcpy(m_LogicParamMatrix.m_mat_vp, t_mat_vp.get(), sizeof(f32) * 16);
    m_LogicMtlFlag0 |= MTL_F0_MAT_VP;
    
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

void SVMtlCore::setDepthEnable(bool _bDepthEnable){
    m_LogicParamDepth.enable = _bDepthEnable;
    m_LogicMtlFlag0 |= MTL_F0_DEPTH;
}
