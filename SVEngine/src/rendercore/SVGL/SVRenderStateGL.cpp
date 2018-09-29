//
// SVRenderStateGL.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVRenderStateGL.h"
#include "../../app/SVInst.h"
#include "../SVRenderTarget.h"
#include "../../basesys/SVSceneMgr.h"

SVRenderStateGL::SVRenderStateGL(SVInst *_app)
:SVRenderState(_app){
    //
    m_fbo = 0;
    m_colorBufferID = 0;
    m_vertexBufID = 0;
    m_indexBufID = 0;
    m_VFType = E_VF_BASE;
    //
    m_view_x = 0;
    m_view_y = 0;
    m_view_w = 0;
    m_view_h = 0;
    //当前纹理通道绑定的纹理
    m_tex_channel0 = 0;
    m_tex_channel1 = 0;
    m_tex_channel2 = 0;
    m_tex_channel3 = 0;
    //当前融合参数
    m_blend = false;
    m_blend_srcparam_rgb = GL_SRC_ALPHA;
    m_blend_dstparam_rgb = GL_ONE_MINUS_SRC_ALPHA;
    m_blend_srcparam_alpha = GL_SRC_ALPHA;
    m_blend_dstparam_alpha = GL_ONE_MINUS_SRC_ALPHA;
    //当前模板参数(这块暂时不管)
    m_stencil = false;
    m_stencil_refvalue = 0;
    m_stencil_maskvalue = 0xff;
    m_stencil_zpass = GL_FALSE;
    m_stencil_zfail = GL_FALSE;
    //u32 m_stencil_dstparam_alpha;
    //当前深度信息
    m_depth = false;
    m_depth_dirth = 0.0f;
    //当前线宽信息
    m_line_width = 1.0f;
    //当前点信息
    m_point_size = 1.0f;
    //纹理状态
    for(s32 i=0;i<8;i++){
        m_texState[i].m_texID = 0;
        m_texState[i].m_min_filter = 0;
        m_texState[i].m_max_filter = 0;
        m_texState[i].m_s_wrap = 0;
        m_texState[i].m_t_wrap = 0;
    }
}

SVRenderStateGL::~SVRenderStateGL() {
}
