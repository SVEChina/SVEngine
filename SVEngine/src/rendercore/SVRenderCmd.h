//
// SVRenderCmd.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RENDERCMD_H
#define SV_RENDERCMD_H

#include "SVRenderDef.h"
#include "SVRenderMesh.h"
#include "../base/SVObject.h"
#include "../base/SVPreDeclare.h"
#include "../mtl/SVMtlDeclare.h"
namespace sv {
    
    namespace render{
        
        //渲染命令基类
        class SVRenderCmd : public SVObject {
        public:
            SVRenderCmd();
            
            ~SVRenderCmd();
            
            void setRenderer(SVRendererBasePtr _renderer);
            
            virtual void render();
            
            SVString mTag;
            
        protected:
            SVRendererBasePtr m_pRenderer;
        };
        
        //创建指令
        class SVRCmdCreate : public SVRenderCmd {
        public:
            SVRCmdCreate(SVRObjBasePtr _robj);
            
            ~SVRCmdCreate();
            
            virtual void render();
            
        protected:
            SVRObjBasePtr m_pRObj;
        };

        //普通渲染命令
        class SVRenderCmdNor : public SVRenderCmd {
        public:
            SVRenderCmdNor();
            
            ~SVRenderCmdNor();
            
            virtual void render();
            
            void setMesh(SVRenderMeshPtr _mesh);
            
            void setMaterial(SVMtlCorePtr _material);
            
        protected:
            SVRenderMeshPtr m_pMesh;
            SVMtlCorePtr m_pMtl;
        };
        
        //清空命令
        class SVRenderCmdClear : public SVRenderCmdNor {
        public:
            SVRenderCmdClear();
            
            ~SVRenderCmdClear();
            
            void setClearColor(f32 _r,f32 _g,f32 _b,f32 _a);
            
            virtual void render();
            
        protected:
            f32 m_color_r;
            f32 m_color_g;
            f32 m_color_b;
            f32 m_color_a;
        };
        
        //适配命令
        class SVRenderCmdAdapt : public SVRenderCmdClear {
        public:
            SVRenderCmdAdapt();
            
            ~SVRenderCmdAdapt();
            
            void setWinSize(s32 _w,s32 _h);
            
            virtual void render();
            
        protected:
            s32 m_winWidth;
            
            s32 m_winHeight;
        };
        
        //多批次渲染命令
        class SVRenderCmdPass : public SVRenderCmdNor {
        public:
            SVRenderCmdPass();
            
            ~SVRenderCmdPass();

            virtual void render();
            
            void setFbo(SVRenderTexturePtr _fbo);
            
            void setTexture(SVTexturePtr _tex);
            
        protected:
            SVRenderTexturePtr m_fbo;
            
            SVTexturePtr m_tex;
        };
        
        //多批次渲染命令集合
        class SVRenderCmdPassCollection : public SVRenderCmdNor {
        public:
            SVRenderCmdPassCollection();
            
            ~SVRenderCmdPassCollection();
            
            virtual void render();
            
            void setFbo(SVRenderTexturePtr _fbo);
            
            void setTexture(SVTexturePtr _tex);
            
            void addMtlMesh(SVMtlCorePtr _mtl , SVRenderMeshPtr _mesh);
            
        protected:
            SVRenderTexturePtr m_fbo;
            
            SVTexturePtr m_tex;
            
            SVArray<SVMtlCorePtr> m_MtlArray;
            SVArray<SVRenderMeshPtr> m_MeshArray;
        };
        
        //FBO绑定(推送FBO)
        class SVRenderCmdFboBind : public SVRenderCmd {
        public:
            SVRenderCmdFboBind(SVFboObjectPtr _fbo);
            
            ~SVRenderCmdFboBind();
            
            virtual void render();
            
        protected:
            SVFboObjectPtr m_fbo;
        };
        
        //FBO解绑定(弹出FBO)
        class SVRenderCmdFboUnbind : public SVRenderCmd {
        public:
            SVRenderCmdFboUnbind(SVFboObjectPtr _fbo);
            
            ~SVRenderCmdFboUnbind();
            
            virtual void render();
        protected:
            SVFboObjectPtr m_fbo;
        };
        
    }//!namespace render
    
}//!namespace sv




//
#endif //SV_RENDERCMD_H
