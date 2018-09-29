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
#include "../../base/SVPreDeclare.h"
#include "../../base/SVRect.h"
#include "../../core/SVVertDef.h"
#include "../../base/SVDataChunk.h"

//通道数
#define CHANNEL_POSITION    0
#define CHANNEL_NORMAL      1
#define CHANNEL_TAGENT      2
#define CHANNEL_BTAGENT     3
#define CHANNEL_COLOR0      4
#define CHANNEL_COLOR1      5
#define CHANNEL_TEXCOORD0   6
#define CHANNEL_TEXCOORD1   7
#define CHANNEL_TEXCOORD2   2
#define CHANNEL_TEXCOORD3   3
#define CHANNEL_BONE_ID     3
#define CHANNEL_BONE_WEIGHT 4
//
#define CHANNEL_ATTRI_0     0
#define CHANNEL_ATTRI_1     1
#define CHANNEL_ATTRI_2     2
#define CHANNEL_ATTRI_3     3
#define CHANNEL_ATTRI_4     4
#define CHANNEL_ATTRI_5     5
#define CHANNEL_ATTRI_6     6
#define CHANNEL_ATTRI_7     7

//内置变量
#define NAME_POSITION   "aPosition"
#define NAME_NORMAL     "aNormal"
#define NAME_COLOR      "aColor"
#define NAME_COLOR1     "aColor1"
#define NAME_TEXCOORD0  "aTexcoord0"
#define NAME_TEXCOORD1  "aTexcoord1"
#define NAME_TEXCOORD2  "aTexcoord2"
#define NAME_TEXCOORD3  "aTexcoord3"
//骨骼动画
#define NAME_BONE_ID     "aBoneID"
#define NAME_BONE_WEIGHT "aBoneWeight"

//
#define NAME_TEX0  "aTexture0"
#define NAME_TEX1  "aTexture1"
#define NAME_TEX2  "aTexture2"
#define NAME_TEX3  "aTexture3"
#define NAME_TEX4  "aTexture4"
#define NAME_TEX5  "aTexture5"
#define NAME_TEX6  "aTexture6"
#define NAME_TEX7  "aTexture7"

//uniform
#define NAME_M_MATRIX   "aMatrixM"
#define NAME_V_MATRIX   "aMatrixV"
#define NAME_P_MATRIX   "aMatrixP"
#define NAME_VP_MATRIX  "aMatrixVP"
#define NAME_MVP_MATRIX "aMatrixMVP"

#define BLEND_FARC      "blendfrac"
#define TEXCOORD_CLIP   "texcoordClip"
#define NAME_TEXSIZE_0  "tex0size"
#define NAME_TEXSIZE_1  "tex1size"
#define NAME_TEXSIZE_2  "tex2size"
#define NAME_TEXSIZE_3  "tex3size"

#define NAME_BLENDMODE  "blendmode"
#define NAME_POINTSIZE  "ptsize"
//渲染资源

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

            virtual bool getbLoad();

            u32  getTexID();

            u32  getuid();

            inline void setname(cptr8 _name){ m_name = _name; }
            inline void settype(s32 _type){ m_type = _type; }
            inline void setwidth(s32 _width){ m_width = _width; }
            inline void setheight(s32 _height){ m_height = _height; }
            inline void setinformate(s32 _informate){ m_informate = _informate; }
            inline void setdataformate(s32 _datafromate){ m_dataformate = _datafromate;}
            inline void setEnableMipMap(bool _enableMipMap){ m_enableMipMap = _enableMipMap; }
            
            inline cptr8 getname(){ return m_name.c_str(); }
            inline s32 gettype(){ return m_type; }
            inline s32 getwidth(){ return m_width; }
            inline s32 getheight(){ return m_height; }
            inline s32 getinformate(){ return m_informate; }
            inline s32 getdataformate(){ return m_dataformate; }

        protected:
            SVString m_name;
            u32     m_id;           //纹理ID
            s32     m_type;         //类型
            s32     m_width;        //宽
            s32     m_height;       //高
            s32     m_informate;    //内部格式
            s32     m_dataformate;  //数据格式
            bool    m_bLoad;
            bool    m_enableMipMap;
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
        //
        //Shader资源
        class SVRResGLShader: public SVRObjBase {
        public:
            SVRResGLShader(SVInst* _app);

            virtual ~SVRResGLShader();

            virtual void create(SVRendererBasePtr _renderer);

            virtual void destroy(SVRendererBasePtr _renderer);

            u32 getProgramm();

            void setProgrammeName(cptr8 _filename);

            cptr8 getProgrammeName();

            void setTechFName(cptr8 _filename);

            void setVSFName(cptr8 _filename);

            void setFSFName(cptr8 _filename);

            void setGSFName(cptr8 _filename);

            void setCSFName(cptr8 _filename);

            void setTSCFName(cptr8 _filename);

            void setTSEFName(cptr8 _filename);

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

            SVString m_programme_fname;

            bool m_use_tech;

            SVString m_tech_fname;   //技术

            SVString m_vs_fname;

            SVString m_fs_fname;

            SVString m_gs_fname;

            SVString m_cs_fname;

            SVString m_tsc_fname;

            SVString m_tse_fname;

            u32 m_vs;

            u32 m_fs;

            u32 m_gs;

            u32 m_tsc;

            u32 m_tse;

            u32 m_cs;

            u32 m_programm;
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

            virtual void render();

            void setIndexPoolType(u32 itype);

            void setVertexPoolType(u32 vtype);

            void setVertexType(VFTYPE type);

            void setDrawMethod(DRAWMETHOD drawtype);

            void setvisible(bool bVis);

            void setVertexDataNum(s32 _vertexNum);
            
            virtual void setIndexData(SVDataSwapPtr _data,s32 _num);
            
            virtual void setVertexData(SVDataSwapPtr _data);

        protected:
            
            void _reset();
            
            virtual void _bindVerts();

            virtual void _updateVertDsp();

            virtual void _unbindVerts();

            virtual void _updateVertex();

            virtual void _updateIndex();

            VFTYPE m_vftype;
            u32 m_vaoID;
            u32 m_vboID;
            u32 m_indexID;
            u32 m_indexNum;    //索引数量
            u32 m_pointNum;    //顶点数量
            u32 m_vertPoolType;
            u32 m_indexPoolType;
            DRAWMETHOD m_drawmethod;
            bool m_dirty;
            bool m_renderDirty;
            SVDataSwapPtr m_pDataIndex;
            SVDataSwapPtr m_pDataVertex;
            bool m_bVisible;
            bool m_verbufferNeedResize;
            bool m_indbufferNeedResize;
        };


        class SVResGLRenderMeshDvid : public SVResGLRenderMesh {
        public:
            SVResGLRenderMeshDvid(SVInst* _app);

            ~SVResGLRenderMeshDvid();

            virtual void create(SVRendererBasePtr _renderer);

            virtual void destroy(SVRendererBasePtr _renderer);

            virtual void render();

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
            
            void _bindVerts();

            void _updateVertDsp();

            void _unbindVerts();

            void _updateVertex();

        private:
            bool m_vertexDirty;
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

            SVDataSwapPtr m_pDataV2;
            SVDataSwapPtr m_pDataV3;
            SVDataSwapPtr m_pDataC0;
            SVDataSwapPtr m_pDataC1;
            SVDataSwapPtr m_pDataT0;
            SVDataSwapPtr m_pDataT1;
            SVDataSwapPtr m_pDataT2;
            SVDataSwapPtr m_pDataNor;
            SVDataSwapPtr m_pDataTag;
            SVDataSwapPtr m_pDataBTor;
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
