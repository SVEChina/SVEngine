//
// SVRResGL.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RRESGL_H
#define SV_RRESGL_H

#include "../SVRObjBase.h"
#include "../SVResTex.h"
#include "../SVResFBO.h"
#include "../SVResShader.h"
#include "../../base/SVPreDeclare.h"
#include "../../base/SVRect.h"
#include "../../core/SVVertDef.h"
#include "../../base/SVDataChunk.h"

namespace sv {

    namespace render{

        //纹理资源
        class SVRResGLTex: public SVResTex {
        public:
            SVRResGLTex(SVInst* _app);

            virtual ~SVRResGLTex();

            virtual void create(SVRendererBasePtr _renderer);

            virtual void destroy(SVRendererBasePtr _renderer);

            virtual void setTexData(void *_data, s32 _len);

            virtual void commit();  //数据提交到显卡
        };
        
        class SVRResGLTexWithTexID: public SVRResGLTex {
        public:
            SVRResGLTexWithTexID(SVInst* _app, s32 _id);
            
            virtual ~SVRResGLTexWithTexID();
            
            virtual void create(SVRendererBasePtr _renderer);
            
            virtual void destroy(SVRendererBasePtr _renderer);
            
        };
        
        //PList假纹理
        class SVRResGLTexPlist : public SVRResGLTex{
        public:
            SVRResGLTexPlist(SVInst *mApp);

            ~SVRResGLTexPlist();

            virtual void refreshParam();

            virtual void apply();

            virtual void apply(void *data);

            void commit();

            SVRect *getSrcRect();

            SVRect *getDstRect();

            void bindTexset(SVRResGLTexSetPtr _texset);

            void unbindTexset();

            bool getbLoad();

        public:
            bool rot;    //是否旋转
            bool trim;   //是否旋转
            f32 srcw;   //原有纹理的宽度
            f32 srch;   //原有纹理的高度

        private:
            SVRect m_srcRect; //在原图的尺寸
            SVRect m_dstRect; //在大纹理中的位置
            SVRResGLTexSetPtr m_pTexset;
        };
        //
        //PList真纹理
        class SVRResGLTexSet : public SVRResGLTex {
        public:
            SVRResGLTexSet(SVInst *mApp);

            ~SVRResGLTexSet();
        };

        //
        //iOS纹理
        class SVRResGLTexiOS : public SVRResGLTex {
        public:
            SVRResGLTexiOS(SVInst *mApp);

            ~SVRResGLTexiOS();

            virtual void create(SVRendererBasePtr _renderer);

            virtual void destroy(SVRendererBasePtr _renderer);

            void pushData(u8* _srcPtr,s32 _w,s32 _h,s32 _pixelformate);

            void fetchData(u8* _dstPtr,s32 _w,s32 _h);

        protected:
            s32 m_formate;
#ifdef SV_IOS
            CVOpenGLESTextureCacheRef m_pTexCacheRef;
            CVOpenGLESTextureRef m_pTexRef;
            CVPixelBufferRef m_pPixelBuf;
#endif
        };
        
        
        /*
         GL Shader
         */
        
        class SVRResGLShader: public SVResShader {
        public:
            SVRResGLShader(SVInst* _app);

            virtual ~SVRResGLShader();

            virtual void create(SVRendererBasePtr _renderer);

            virtual void destroy(SVRendererBasePtr _renderer);

            void setTechFName(cptr8 _filename);

        private:
            bool _parseTech();

            u32 _loadTechVS(cptr8 _precision,cptr8 _src);

            u32 _loadTechFS(cptr8 _precision,cptr8 _src);

            u32 _loadTechGS(cptr8 _precision,cptr8 _src);

            u32 _loadVS(cptr8 _filename);

            u32 _loadFS(cptr8 _filename);

            u32 _loadGS(cptr8 _filename);

            u32 _loadCS(cptr8 _filename);

            u32 _loadTSC(cptr8 _filename);

            u32 _loadTSE(cptr8 _filename);

            u32 _createProgram();

            void _clearShaderRes();

            bool m_use_tech;

            SVString m_tech_fname;   //技术

            u32 m_vs;

            u32 m_fs;

            u32 m_gs;

            u32 m_tsc;

            u32 m_tse;

            u32 m_cs;
        };
        //////////////////////////////////////////////////////////FBO资源
        class SVRResGLFBO: public SVResFBO {
        public:
            SVRResGLFBO(SVInst* _app);

            virtual ~SVRResGLFBO();

            virtual void create(SVRendererBasePtr _renderer);

            virtual void destroy(SVRendererBasePtr _renderer);

            virtual void refresh();

            virtual void bind();
            
            virtual void clear();

            virtual void unbind();

            inline u32 getFboID(){ return m_fboID; }

            inline u32 getWidth(){ return m_width; }

            inline u32 getHeight(){ return m_height; }

            inline bool hasDepth(){ return m_depth; }

            inline bool hasStencil(){ return m_stencil; }

        protected:

            virtual void _bindColor();

            virtual void _bindDepth();

            u32 m_fboID;

            u32 m_depthID;

            s32 m_lastFboID;

            u32 m_width;

            u32 m_height;

            bool m_depth;

            bool m_stencil;

            bool m_dirty;

        };

        //外部传入的FBO
        class SVRResGLOutFBO : public SVRResGLFBO {
        public:
            SVRResGLOutFBO(SVInst *_app,u32 _fboid);

            ~SVRResGLOutFBO();

            virtual void create(SVRendererBasePtr _renderer);

            virtual void destroy(SVRendererBasePtr _renderer);
        };

        //RenderTarget
        class SVResGLRenderTarget : public SVRResGLFBO {
        public:
            SVResGLRenderTarget(SVInst *_app,s32 _w, s32 _h,bool _depth,bool _stencil);

            ~SVResGLRenderTarget();

            void create(SVRendererBasePtr _renderer);

            void destroy(SVRendererBasePtr _renderer);

            inline int getColorID() { return m_colorID; }

        protected:
            u32 m_colorID;
        };
        //
        class SVResGLRenderTargetOut : public SVResGLRenderTarget {
        public:
            SVResGLRenderTargetOut(SVInst *_app,s32 _w, s32 _h,u32 _fboid,u32 _colorID);

            ~SVResGLRenderTargetOut();

            void create(SVRendererBasePtr _renderer);

            void destroy(SVRendererBasePtr _renderer);
        };
        //
        class SVResGLRenderTexture : public SVRResGLFBO {
        public:
            SVResGLRenderTexture(SVInst *_app,SVRResGLTexPtr _tex, bool _depth,bool _stencil);

            ~SVResGLRenderTexture();

            void create(SVRendererBasePtr _renderer);

            void destroy(SVRendererBasePtr _renderer);

            void setTexture(SVRResGLTexPtr _tex);

            void refresh();

        protected:
            void _bindColor();

            SVRResGLTexPtr m_tex;
        };
        /////////////////////////////////////////////////////////
        //VBO资源
        class SVRResGLVBO: public SVRObjBase {
        public:
            SVRResGLVBO(SVInst* _app);

            virtual ~SVRResGLVBO();

            virtual void create(SVRendererBasePtr _renderer);

            virtual void destroy(SVRendererBasePtr _renderer);

        };
        //mesh
        class SVResGLRenderMesh : public SVRResGLVBO {
        public:
            static int getVertexFormateSize(VFTYPE _type);

        public:
            SVResGLRenderMesh(SVInst* _app);

            ~SVResGLRenderMesh();

            virtual void create(SVRendererBasePtr _renderer);

            virtual void destroy(SVRendererBasePtr _renderer);

            virtual void render(SVRendererBasePtr _renderer);

            void setIndexPoolType(u32 itype);

            void setVertexPoolType(u32 vtype);

            void setVertexType(VFTYPE type);

            void setDrawMethod(DRAWMETHOD drawtype);

            void setVertexDataNum(s32 _vertexNum);
            
            virtual void setIndexData(SVDataSwapPtr _data,s32 _num);
            
            virtual void setVertexData(SVDataSwapPtr _data);

        protected:
            
            void _reset();
            
            virtual void _bindVerts();

            virtual void _updateVertDsp();

            virtual void _unbindVerts();

            VFTYPE m_vftype;
            u32 m_vaoID;
            u32 m_vboID;
            u32 m_indexID;
            u32 m_indexNum;    //索引数量
            u32 m_pointNum;    //顶点数量
            u32 m_vertPoolType;
            u32 m_indexPoolType;
            DRAWMETHOD m_drawmethod;
            bool m_useVAO;
        };

        class SVResGLRenderMeshDvid : public SVResGLRenderMesh {
        public:
            SVResGLRenderMeshDvid(SVInst* _app);

            ~SVResGLRenderMeshDvid();

            virtual void create(SVRendererBasePtr _renderer);

            virtual void destroy(SVRendererBasePtr _renderer);

            virtual void render(SVRendererBasePtr _renderer);

            void setVertex2Data(SVDataSwapPtr _pdata);

            void setVertex3Data(SVDataSwapPtr _pdata);

            void setColor0Data(SVDataSwapPtr _pdata);

            void setColor1Data(SVDataSwapPtr _pdata);

            void setTexcoord0Data(SVDataSwapPtr _pdata);

            void setTexcoord1Data(SVDataSwapPtr _pdata);

            void setTexcoord2Data(SVDataSwapPtr _pdata);

            void setNormalData(SVDataSwapPtr _pdata);

            void setTagentData(SVDataSwapPtr _pdata);

            void setBTagentData(SVDataSwapPtr _pdata);

        protected:
            void _reset();
        
            void _updateVertDsp();

        private:
            u32 vertex2ID_0;
            u32 vertex2ID_1;
            u32 vertex2ID_2;
            u32 vertex3ID;
            u32 color0ID;
            u32 color1ID;
            u32 texcoord0ID;
            u32 texcoord1ID;
            u32 texcoord2ID;
            u32 normalID;
            u32 tagentID;
            u32 btagentID;
        };
        
        ////////////////////////////////////////////////////////
        //VAO资源
        class SVRResGLVAO: public SVRObjBase {
        public:
            SVRResGLVAO(SVInst* _app);

            virtual ~SVRResGLVAO();

            virtual void create(SVRendererBasePtr _renderer);

            virtual void destroy(SVRendererBasePtr _renderer);
        };


    }//!namespace render

}//!namespace sv



//
#endif //SV_RRESGL_H
