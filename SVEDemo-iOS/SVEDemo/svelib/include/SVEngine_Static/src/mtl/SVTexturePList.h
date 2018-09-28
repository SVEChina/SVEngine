//
// SVTexturePList.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_TEXTUREPLIST_H
#define SV_TEXTUREPLIST_H

#include "SVTexture.h"
#include "../base/SVRect.h"

namespace sv {
    
    namespace mtl {

        //PList假纹理
        class SVTexturePList : public SVTexture {
        public:
            SVTexturePList(SVInst *_app);

            ~SVTexturePList();

            virtual void init(cptr8 _name, s32 _type, s32 _width, s32 _height, s32 _informate, s32 _dateformate);
            
            virtual void create(SVRendererBasePtr _renderer);
            
            virtual void destroy(SVRendererBasePtr _renderer);
            
            virtual void refreshParam();
            
            virtual void apply();
            
            virtual void apply(void *data);
            
            virtual void commit();
            
            SVRect *getSrcRect();

            SVRect *getDstRect();
            
            void bindTexset(SVTextureSetPtr _texset);
            
            bool getbLoad();
            
            void setRot(bool _rot);
            
            void setTrim(bool _trim);
            
            void setSrcw(bool _srcw);
            
            void setSrch(bool _srch);
        protected:
            void _updateData();
        private:
            bool m_rot;    //是否旋转
            bool m_trim;   //是否旋转
            f32  m_srcw;   //原有纹理的宽度
            f32  m_srch;   //原有纹理的高度
            bool m_bApply;
            bool m_bApplyData;
            SVTextureSetPtr m_texset;
            bool m_btexSet;
        };

        //PList真纹理
        class SVTextureSet : public SVTexture {
        public:
            SVTextureSet(SVInst *_app);
            
            ~SVTextureSet();
            
            virtual void init(cptr8 _name, s32 _type, s32 _width, s32 _height, s32 _informate, s32 _dateformate);
            
            virtual void create(SVRendererBasePtr _renderer);
            
            virtual void destroy(SVRendererBasePtr _renderer);
        };
        
    }//!namespace mtl
    
}//!namespace sv


#endif //SV_TEXTUREPLIST_H
