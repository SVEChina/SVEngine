//
// SVMtlCore.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MTLCORE_H
#define SV_MTLCORE_H

#include "SVMtlDef.h"
#include "../base/SVGBase.h"
#include "../app/SVInst.h"
#include "../rendercore/SVRenderDeclare.h"
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
            SVMtlCore(SVInst *_app, cptr8 _shader);
            
            SVMtlCore(SVMtlCore* _mtl);
            
            ~SVMtlCore();
            
            virtual SVMtlCorePtr clone();
            
            virtual void reset();
            
            virtual void update(f32 dt);
            
            virtual bool submitMtl();
            
            void swap();
                        
            void setTexture(s32 _chanel,SVTexturePtr _texture);
            
            void setTexture(s32 _chanel,SVTEXTYPE _from);
            
            void setTexcoordFlip(f32 _x, f32 _y);
            
            void setModelMatrix(f32 *_mat);
            
            void setTexSizeIndex(s32 index, f32 _w, f32 _h);
            
            void setLineSize(f32 _linewidth);
            
            void setBlendEnable(bool _bBlendEnable);
            
            void setBlendState(s32 _src , s32 _dst);
            
            void setDepthEnable(bool _bDepthEnable);
            
            void setCullEnable(bool _bCullEnable);
            
            void setCullFace(s32 _frontFace, s32 _cullFace);
            
            void reloadShader(cptr8 _shader);

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
            
        protected:
            void _loadShader();
            virtual void _refreshMatrix();
            virtual void _refreshModify();
            virtual void _submitUniform(SVRendererBasePtr _render);
            virtual void _submitState(SVRendererBasePtr _render);
            virtual void _submitMtl(SVRendererBasePtr _render);
            
        public:
            void addModify(SVModifyPtr _modify);
            
        protected:
            typedef SVArray<SVModifyPtr> MODPOOL;
            MODPOOL* m_renderPool;
            MODPOOL* m_logicPool;
        public:
            virtual void toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator,
                                RAPIDJSON_NAMESPACE::Value &_objValue);
            
            virtual void fromJSON(RAPIDJSON_NAMESPACE::Value &item);
        protected:
            virtual void _toJsonData(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator,
                                     RAPIDJSON_NAMESPACE::Value &locationObj);
            virtual void _fromJsonData(RAPIDJSON_NAMESPACE::Value &item);
        };

    }//!namespace mtl
    
}//!namespace sv



#endif //SV_MTLCORE_H
