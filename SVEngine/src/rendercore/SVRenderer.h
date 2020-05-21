//
// SVRenderer.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RENDERER_BASE_H
#define SV_RENDERER_BASE_H

#include "SVRenderDef.h"
#include "../base/SVGBase.h"
#include "../base/SVMat4.h"
#include "../base/SVArray.h"
#include "../base/SVStack.h"
#include "../core/SVVertDef.h"
#include "../mtl/SVMtlParamBase.h"

namespace sv {
    
    namespace render{
        
        //
        struct VPParam{
            u32 m_x;
            u32 m_y;
            u32 m_width;
            u32 m_height;
        };
        //渲染器
        
        class SVRenderer: public SVGBase {
        public:
            SVRenderer(SVInst* _app);
            
            ~SVRenderer();
            //初始化
            virtual void init(s32 _w,s32 _h);
            //销毁
            virtual void destroy();
            //重置大小
            virtual void resize(s32 _w,s32 _h);
            //
            virtual void renderBegin();
            //
            virtual void renderEnd();
            //增加渲染内核资源
            void addRes(SVRObjBasePtr _res);
            //移除渲染内核资源
            void removeRes(SVRObjBasePtr _res);
            //清理渲染内核资源
            void clearRes();
            //自动回收
            virtual void removeUnuseRes();
            //获取渲染到纹理
            SVRenderTexturePtr getRenderTexture();
            //创建内置纹理
            virtual SVTexturePtr createSVTex(SVTEXTYPE _type,s32 _w,s32 _h,s32 _formate, bool _enableMipMap = false);
            //创建内置纹理
            virtual SVTexturePtr createSVTex(SVTEXTYPE _type,s32 _w,s32 _h,s32 _informate,s32 _daformate, bool _enableMipMap = false);
            //创建内置纹理
            virtual SVTexturePtr createSVTexIOS(SVTEXTYPE _type,s32 _w,s32 _h,s32 _formate, bool _enableMipMap = false);
            //创建内置纹理 ios
            virtual SVTexturePtr createSVTexIOS(SVTEXTYPE _type,s32 _w,s32 _h,s32 _informate,s32 _daformate, bool _enableMipMap = false);
            //销毁内置纹理
            void destroySVTex(SVTEXTYPE _type);
            //获取内置纹理
            SVTexturePtr getSVTex(SVTEXTYPE _type);
            //是否存在内置纹理
            bool hasSVTex(SVTEXTYPE _type);
            //获取渲染状态
            SVRenderStatePtr getState();
            //重置状态
            void resetState();
            //投影矩阵
            void pushProjMat(FMat4 _mat);
            FMat4 getProjMat();
            void popProjMat();
            //视矩阵
            void pushViewMat(FMat4 _mat);
            FMat4 getViewMat();
            void popViewMat();
            //vp矩阵
            void pushVPMat(FMat4 _mat);
            FMat4 getVPMat();
            void popVPMat();
            //
            void clearMatStack();
            
        protected:
            //主FBO
            SVRenderTexturePtr m_pRenderTex;
            //各种内置纹理
            SVTexturePtr m_svTex[E_TEX_END];
            //渲染内核资源
            typedef SVArray<SVRObjBasePtr> ROBJLIST;
            ROBJLIST m_robjList;
            //资源锁
            SVLockPtr m_resLock;
            //渲染状态
            SVRenderStatePtr m_pRState;
            //渲染VP
            SVStack<VPParam,10> m_vpStack;  //viewport堆栈
            //
            typedef SVStack<FMat4,10> MAT4STACK;//注意：栈最大支持的矩阵个数为10个
            MAT4STACK m_stack_proj;
            MAT4STACK m_stack_view;
            MAT4STACK m_stack_vp;

            //inner size
            s32 m_inWidth;
            s32 m_inHeight;
            s32 m_outWidth;
            s32 m_outHeight;
            
        public:
            //renderder interface
            //提交纹理
            virtual void submitTex(u32 _channel,TexUnit& _unit){}
            //提交unifrom matrix
            virtual void submitUniformMatrix(cptr8 _name,f32* _data){}
            //提交unifrom matrix array
            virtual void submitUniformMatrixArray(cptr8 _name,f32* _data,s32 _size){}
            //提交unifrom i1
            virtual void submitUniformi(cptr8 _name,s32 _data){}
            //提交unifrom i2
            virtual void submitUniformi2(cptr8 _name,s32 _data1,s32 _data2){}
            //提交unifrom i3
            virtual void submitUniformi3(cptr8 _name,s32 _data1,s32 _data2,s32 _data3){}
            //提交unifrom i4
            virtual void submitUniformi4(cptr8 _name,s32 _data1,s32 _data2,s32 _data3,s32 _data4){}
            //提交unifrom f1
            virtual void submitUniformf(cptr8 _name,f32 _data){}
            //提交unifrom f2
            virtual void submitUniformf2(cptr8 _name,f32 _data1,f32 _data2){}
            //提交unifrom f3
            virtual void submitUniformf3(cptr8 _name,f32 _data1,f32 _data2,f32 _data3){}
            //提交unifrom f4
            virtual void submitUniformf4(cptr8 _name,f32 _data1,f32 _data2,f32 _data3,f32 _data4){}
            //提交unifrom s32 v1
            virtual void submitUniformi1v(cptr8 _name,s32* _data,s32 _size = 1){}
            //提交unifrom s32 v2
            virtual void submitUniformi2v(cptr8 _name,s32* _data,s32 _size = 1){}
            //提交unifrom s32 v3
            virtual void submitUniformi3v(cptr8 _name,s32* _data,s32 _size = 1){}
            //提交unifrom s32 v4
            virtual void submitUniformi4v(cptr8 _name,s32* _data,s32 _size = 1){}
            //提交unifrom f32 v1
            virtual void submitUniformf1v(cptr8 _name,f32* _data,s32 _size = 1){}
            //提交unifrom f32 v2
            virtual void submitUniformf2v(cptr8 _name,f32* _data,s32 _size = 1){}
            //提交unifrom f32 v3
            virtual void submitUniformf3v(cptr8 _name,f32* _data,s32 _size = 1){}
            //提交unifrom f32 v4
            virtual void submitUniformf4v(cptr8 _name,f32* _data,s32 _size = 1){}
            //提交融合参数
            virtual void submitBlend(SVBlendParam& _param){}
            //提交模板参数
            virtual void submitStencil(SVStencilParam& _param){}
            //提交深度参数
            virtual void submitDepth(SVDepthParam& _param){}
            //提交隐藏面参数
            virtual void submitCull(SVCullFaceParam& _param){}
            //提交zfighting
            virtual void submitZOff(SVZOffParam& _param){}
            //提交线宽
            virtual void submitLineWidth(f32 _width){}
            //提交点大小
            virtual void submitPointSize(f32 _size){}
            //绑定FBO
            virtual void svBindFrameBuffer(u32 _id){}
            //清理颜色
            virtual void svBindClearColor(u32 _id){}
            //颜色缓存
            virtual void svBindColorBuffer(u32 _id){}
            //顶点缓存
            virtual void svBindVertexBuffer(u32 _id){}
            //索引缓存
            virtual void svBindIndexBuffer(u32 _id){}
            //顶点格式更新
            virtual void svUpdateVertexFormate(VFTYPE _vf,s32 _count = 0,s32 _mode = 1){}
            //视口
            virtual void svPushViewPort(u32 _x,u32 _y,u32 _w,u32 _h);
            //退出视口
            virtual void svPopViewPort();
            //设置清理颜色
            virtual void svClearColor(f32 _r,f32 _g,f32 _b,f32 _a){}
            //设置清理掩码
            virtual void svClear(s32 _mask){}
        };
    
    }//!namespace render
    
}//!namespace sv

//
#endif //SV_RENDERER_BASE_H
