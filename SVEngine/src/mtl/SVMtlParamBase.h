//
// SVMtlParamBase.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTLPARAM_H
#define SV_MTLPARAM_H

#include "../base/SVObject.h"
#include "../base/SVPreDeclare.h"
#include "SVMtlDef.h"

#define MAX_TEXUNIT 8

namespace sv {
    
    namespace mtl{
        
        class SVMtlParam : public SVObject {
        public:
            SVMtlParam();
            
            ~SVMtlParam();
            
            virtual void reset();
        };
        
        //
        class SVMatrixParam : public SVMtlParam {
        public:
            SVMatrixParam();
            
            ~SVMatrixParam();
            
            void reset();
            
            void copy(SVMatrixParam& _param);
            
            f32 m_mat_model[16];
            f32 m_mat_view[16];
            f32 m_mat_project[16];
            f32 m_mat_vp[16];
        };
        
        //
        class TexUnit {
        public:
            TexUnit();
            
            ~TexUnit();
            
            void copy(TexUnit& _texunit);
            
            void reset();
            
            SVTexturePtr m_pTex;
            
            f32 m_texcoordFlip[2];
            
            SVTEXTYPE m_texForm;  //纹理来源
            
            u32 m_min_filter;
            
            u32 m_max_filter;
            
            u32 m_s_wrap;
            
            u32 m_t_wrap;
             
        };
        
        class SVTextureParam : public SVMtlParam {
        public:
            SVTextureParam();
            
            ~SVTextureParam();
            
            void reset();
            
            void copy(SVTextureParam& _param);
            
            void setTexture(u32 _index,SVTexturePtr _tex);
            
            void setTexture(u32 _index,SVTEXTYPE _from);
            
            void setTexClip(u32 _index,f32 _x,f32 _y);
            
            TexUnit m_texUnit[MAX_TEXUNIT];
        };
        
        //
        class SVBlendParam : public SVMtlParam {
        public:
            SVBlendParam();
            
            ~SVBlendParam();
            
            void reset();
            
            void copy(SVBlendParam& _param);
            
            bool enable;
            s32 srcParam;
            s32 dstParam;
        };
        
        //
        class SVCullFaceParam : public SVMtlParam {
        public:
            SVCullFaceParam();
            
            ~SVCullFaceParam();
            
            void reset();
            
            void copy(SVCullFaceParam& _param);
            
            bool enable;
            s32 frontFace;
            s32 cullFace;
        };
        
        //
        class SVDepthParam : public SVMtlParam {
        public:
            SVDepthParam();
            
            ~SVDepthParam();
            
            void reset();
            
            void copy(SVDepthParam& _param);
            
            bool clear;
            bool enable;        //开启深度测试
            s32 testMethod;     //深度测试方法(GL_NEVER,GL_ALWAYS,GL_LESS,GL_LEQUAL,GL_GREATER,GL_GEQUAL,GL_NOTEQUAL)
        };
        
        //
        class SVStencilParam : public SVMtlParam {
        public:
            SVStencilParam();
            
            ~SVStencilParam();
            
            void copy(SVStencilParam& _param);
            
            void reset();
            
            bool clear;         //
            bool enable;        //开启模板测试
            s32 passMethod;     //通过模板测试的规则(GL_NEVER,GL_LESS < ,GL_LEQUAL <= ,GL_GREATER > ,GL_GEEQUAL >= ,GL_EQUAL == ,GL_ALWAYS)
            s32 refValue;       //通过模板测试的参考值(0-255)
            s32 maskValue;      //掩码( 模板值&掩码 * 参考值&掩码 )
            s32 sfail;          //模板测试失败     (GL_KEEP,GL_ZERO,GL_REPLACE(参考值代替写入),GL_INCR,GL_DECR,GL_INVERT(按位取反))
            s32 zfail;          //模板通过 深度失败 (GL_KEEP,GL_ZERO,GL_REPLACE(参考值代替写入),GL_INCR,GL_DECR,GL_INVERT(按位取反))
            s32 zpass;          //模板通过 深度通过 (GL_KEEP,GL_ZERO,GL_REPLACE(参考值代替写≤
        };
        
        //
        class SVAlphaParam : public SVMtlParam {
        public:
            SVAlphaParam();
            
            ~SVAlphaParam();
            
            void reset();
            
            void copy(SVAlphaParam& _param);
            
            bool enable;        //开启alpha测试
            s32 testMethod;    //alpha测试方法(GL_NEVER,GL_ALWAYS,GL_LESS,GL_LEQUAL,GL_GREATER,GL_GEQUAL,GL_NOTEQUAL)
        };
        
        //
        class SVSizeParam : public SVMtlParam {
        public:
            SVSizeParam();
            
            ~SVSizeParam();
            
            void reset();
            
            void copy(SVSizeParam& _param);
            
            f32 m_tex0size[2];
            f32 m_tex1size[2];
            f32 m_tex2size[2];
            f32 m_tex3size[2];
            f32 m_ptsize;
            f32 m_linewidth;
        };
        
        //
        class SVBlendModeParam : public SVMtlParam {
        public:
            SVBlendModeParam();
            
            ~SVBlendModeParam();
            
            void reset();
            
            void copy(SVBlendModeParam& _param);
            
            s32 m_blendmode;
        };
        
        
    }//!namespace mtl
}//!namespace sv




#endif //SV_MTLPARAM_H
