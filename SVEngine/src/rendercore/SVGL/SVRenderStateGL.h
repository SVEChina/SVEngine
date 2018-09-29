//
// SVRenderStateGL.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RENDERSTATE_GL_H
#define SV_RENDERSTATE_GL_H

#include "../renderer/SVRenderState.h"
#include "../../core/SVVertDef.h"

//渲染状态 主要用于 状态重复调用的控制

namespace sv {
    
    namespace render{
        
        class SVRenderStateGL : public SVRenderState {
        public:
            SVRenderStateGL(SVInst* _app);
            
            ~SVRenderStateGL();
            
            //当前FBO
            u32 m_fbo;
            
            u32 m_colorBufferID;
            
            u32 m_vertexBufID;
            
            u32 m_indexBufID;
            
            //顶点格式描述
            VFTYPE m_VFType;
            
            //当前视口
            u32 m_view_x;
            u32 m_view_y;
            u32 m_view_w;
            u32 m_view_h;
            
            //当前纹理通道绑定的纹理
            u32 m_tex_channel0;
            u32 m_tex_channel1;
            u32 m_tex_channel2;
            u32 m_tex_channel3;
            
            //当前融合参数
            bool m_blend;
            u32 m_blend_srcparam_rgb;
            u32 m_blend_dstparam_rgb;
            u32 m_blend_srcparam_alpha;
            u32 m_blend_dstparam_alpha;
            
            //当前模板参数(这块暂时不管)
            bool m_stencil;
            u32 m_stencil_refvalue;
            u32 m_stencil_maskvalue;
            u32 m_stencil_zpass;
            u32 m_stencil_zfail;
            //u32 m_stencil_dstparam_alpha;
            
            //当前深度信息
            bool m_depth;
            f32 m_depth_dirth;
            
            //当前线宽信息
            f32 m_line_width;
            
            //当前点信息
            f32 m_point_size;
            
            //纹理通道状态
            struct TexState {
                s32 m_texID;
                s32 m_min_filter;
                s32 m_max_filter;
                s32 m_s_wrap;
                s32 m_t_wrap;
            };
            TexState m_texState[8];
        };
        
    }//!namespace render
    
}//!namespace sv


#endif //SV_RENDERSTATE_GL_H
