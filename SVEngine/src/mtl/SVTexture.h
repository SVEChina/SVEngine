//
// SVTexture.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_TEXTURE_H
#define SV_TEXTURE_H

#include "SVMtlDef.h"
#include "../rendercore/SVResTex.h"
#include "../rendercore/SVRenderDef.h"

namespace sv {
    
    namespace mtl {
        
        class SVTexture : public SVRObjBase {
        public:
            SVTexture(SVInst *_app);
            
            ~SVTexture();
            
            virtual void init(cptr8 _name, s32 _type, s32 _width, s32 _height, s32 _informate, s32 _dateformate, bool _enableMipMap = false);
            
            virtual void create(SVRendererBasePtr _renderer);
            
            virtual void destroy(SVRendererBasePtr _renderer);
            
            virtual void setTexData(void *_data, s32 _len);
            
            virtual void commit();  //数据提交到显卡
            
            virtual bool getbLoad();
            
            u32  getTexID();
            
            u32  getuid();
            
            cptr8 getname();
            
            s32 gettype();
            
            s32 getwidth();
            
            s32 getheight();
            
            s32 getinformate();
            
            s32 getdataformate();
            
            SVResTexPtr getResTex();
            
        protected:
            void _updateData();
            SVResTexPtr m_objTexPtr;
            SVString m_name;
            s32   m_type;
            s32   m_width;
            s32   m_height;
            s32   m_informate;
            s32   m_dataformate;
            bool  m_bData;
            bool  m_bEnableMipMap;
            bool  m_bCreated;
            SVDataSwapPtr m_pData;
            u32   m_uid;          //唯一ID对象
            
        public:
            virtual void pushData(u8* _srcPtr,s32 _w,s32 _h,s32 _pixelformate){}
            
            virtual void fetchData(u8* _dstPtr,s32 _w,s32 _h){}
        };
        
        class SVTextureInputTexID : public SVTexture{
        public:
            SVTextureInputTexID(SVInst *_app, s32 _texID);
            
            ~SVTextureInputTexID();
            
            virtual void init(cptr8 _name, s32 _type, s32 _width, s32 _height, s32 _informate, s32 _dateformate, bool _enableMipMap = false);
            
            virtual void create(SVRendererBasePtr _renderer);
            
            virtual void destroy(SVRendererBasePtr _renderer);
            
        protected:
            s32 m_texID;
        };
        
    }//!namespace mtl
    
}//!namespace sv



#endif //SV_TEXTURE_H
