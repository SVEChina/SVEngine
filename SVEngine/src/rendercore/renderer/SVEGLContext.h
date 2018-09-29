//
// SVEGLContext.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//


#ifndef SVE_EGLCONTEXT_H
#define SVE_EGLCONTEXT_H

//c++层context

#include "SVContextBase.h"

//只有一个后台  所谓多场景输出 指的是多纹理输出
#ifdef SV_ANDROID
namespace sv {

    namespace render {

        class SVEGLContext : public SVContextBase {

        public:
            SVEGLContext(SVInst *_app,void* _window, void *_context, s32 _glversion);

            ~SVEGLContext();

            virtual void swapRenderTarget(SVRenderTargetPtr _target);

            virtual bool activeContext();

            virtual void* getContext();

        protected:
            EGLDisplay m_Display;
            EGLConfig *m_pConfigsList;
            EGLConfig m_GLConfigOff;
            EGLContext m_pGLContext;
            EGLContext m_pGLContextShare;
            EGLSurface m_pGLSurface;
            u32 m_color_size_;
            u32 m_depth_size_;
            GLint m_screen_width_;
            GLint m_screen_height_;
            void* m_pwindow;
            bool m_context_valid_;
            bool m_egl_context_initialized_;
            bool m_gles_initialized_;
            bool m_es3_supported_;
            s32 m_gl_version_;

            void initGLES();

            bool init(ANativeWindow* window);

            bool initEGLContext();

            void createSurface();

            EGLint resume(ANativeWindow* window);

            void destroyContext();

            bool active();

            bool swap();
        };
    }//!namespace render

}//!namespace sv
#endif  //SVE_ANDROID
#endif //SVE_EGLCONTEXT_H
