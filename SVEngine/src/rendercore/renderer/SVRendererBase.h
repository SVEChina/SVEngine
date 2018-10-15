//
// SVRendererBase.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RENDERER_BASE_H
#define SV_RENDERER_BASE_H

#include "../SVRenderDef.h"
#include "../../base/SVGBase.h"
#include "../../base/SVArray.h"
#include "../../base/SVStack.h"
#include "../../core/SVVertDef.h"
#include "../../mtl/SVMtlParamBase.h"

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
        
        class SVRendererBase: public SVGBase {
        public:
            SVRendererBase(SVInst* _app);
            
            ~SVRendererBase();
            //初始化
            virtual void init(s32 _ver,void* _windows,void* context,s32 _w,s32 _h);
            //销毁
            virtual void destroy();
            //重置大小
            virtual void resize(s32 _w,s32 _h);
            //增加渲染内核资源
            void addRes(SVRObjBasePtr _res);
            //移除渲染内核资源
            void removeRes(SVRObjBasePtr _res);
            //清理渲染内核资源
            void clearRes();
            //自动回收
            virtual void removeUnuseRes();
            //设置渲染上下文
            void setRendererContext(SVContextBasePtr _context);
            //获取渲染上下文
            SVContextBasePtr getRenderContext();
            //获取渲染到纹理
            SVRenderTexturePtr getRenderTexture();
            //获取内置纹理
            SVTexturePtr getSVTex(SVTEXTYPE _type);
            //是否存在内置纹理
            bool hasSVTex(SVTEXTYPE _type);
            //创建内置纹理
            SVTexturePtr createSVTex(SVTEXTYPE _type,s32 _w,s32 _h,s32 _formate);
            //销毁内置纹理
            void destroySVTex(SVTEXTYPE _type);
            //获取渲染状态
            SVRenderStatePtr getState();
            //重置状态
            void resetState();
        protected:
            //渲染上下文
            SVContextBasePtr m_pRenderContext;
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
            
        public:
            //renderder interface
            //提交纹理
            virtual void submitShader(u32 _shader){}
            //提交纹理
            virtual void submitTex(u32 _channel,TexUnit& _unit){}
            //提交unifrom matrix
            virtual void submitUniformMatrix(cptr8 _name,f32* _data){}
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
            virtual void svUpdateVertexFormate(VFTYPE _vf){}
            //视口
            virtual void svPushViewPort(u32 _x,u32 _y,u32 _w,u32 _h);
            //退出视口
            virtual void svPopViewPort();
        };
    
    }//!namespace render
    
}//!namespace sv

//
#endif //SV_RENDERER_BASE_H
