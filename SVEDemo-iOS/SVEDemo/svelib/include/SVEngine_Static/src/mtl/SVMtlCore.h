//
// SVMtlCore.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTLCORE_H
#define SV_MTLCORE_H

#include "../base/SVGBase.h"
#include "../base/SVPreDeclare.h"
#include "../app/SVInst.h"
#include "SVMtlDef.h"
#include "SVShaderMgr.h"
#include "SVMtlParamBase.h"

namespace sv {
    
    namespace mtl{
        
        class SVMtlCoreParam : public SVObject {
        public:
            SVMtlCoreParam();
            
            virtual SVMtlCorePtr genMtl(SVInst *_app);
            
            SVString m_shader;
        };
        
        //
        class SVMtlCore : public SVGBase {
        public:
            enum SVMTLBLENDMODE{
                SV_MTL_BLENDMODE_NORMAL = 0,
                SV_MTL_BLENDMODE_ADDITIVE,
                SV_MTL_BLENDMODE_MULTIPLY,
                SV_MTL_BLENDMODE_SCREEN
            };
            SVMtlCore(SVInst *_app, cptr8 _shader);
            
            SVMtlCore(SVMtlCore* _mtl);
            
            ~SVMtlCore();
            
            virtual SVMtlCorePtr clone();
            
            virtual void reset();
            
            virtual void update(f32 dt);
            
            virtual bool submitMtl();
                        
            void setTexture(s32 _index,SVTexturePtr _texture);
            
            void setTexcoordFlip(f32 _x, f32 _y);
            
            void setModelMatrix(f32 *_mat);
            
            void setViewMatrix(f32 *_mat);
            
            void setProjectMatrix(f32 *_mat);
            
            void setVPMatrix(f32 *_mat);
            
            void setMVP(f32 *_mat);
            
            void setTexSizeIndex(s32 index, f32 _w, f32 _h);
            
            void setLineSize(f32 _linewidth);
            
            void setBlendMode(SVMTLBLENDMODE _blendMode);
            
            void setBlendEnable(bool _bBlendEnable);
            
            void setBlendState(s32 _src , s32 _dst);
            
            void reloadShader(cptr8 _shader);
            
            void copyMainTex();
        public:
            SVString m_mtlname;
            u32 m_programID;
            s32 m_LogicMtlFlag0;                         //MTLFLAG0;
            SVMatrixParam m_LogicParamMatrix;            //矩阵信息
            SVTextureParam m_LogicParamTex;              //纹理参数
            SVBlendParam m_LogicParamBlend;              //融合参数
            SVCullFaceParam m_LogicParamCull;            //裁剪面参数
            SVDepthParam m_LogicParamDepth;              //深度参数
            SVStencilParam m_LogicParamStencil;          //模板参数
            SVAlphaParam m_LogicParamAlpha;              //alpha参数
            SVSizeParam m_LogicParamSize;                //尺寸参数
            SVBlendModeParam m_LogicParamBlendMode;      //融合模式参数
            
        protected:
            void _loadShader();
            virtual void _bindUniform();
            virtual void _submitUniform();
            virtual void _submitState();
            virtual void _submitMtl();
            
        protected:
            //uniform cache
            s32 m_uni_m;
            s32 m_uni_v;
            s32 m_uni_p;
            s32 m_uni_vp;
            s32 m_uni_texcoord_clip ;   //vec2
            s32 m_uni_tex0size;
            s32 m_uni_tex1size;
            s32 m_uni_tex2size;
            s32 m_uni_tex3size;
            s32 m_uni_ptsize;
            s32 m_uni_blendmode;
            bool m_main_tex_copy;
        };

    }//!namespace mtl
    
}//!namespace sv



#endif //SV_MTLCORE_H
