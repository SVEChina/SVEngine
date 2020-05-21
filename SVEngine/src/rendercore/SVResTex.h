//
// SVResTex.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SVResTex_h
#define SVResTex_h

#include "SVRObjBase.h"

namespace sv {
    
    namespace render{
        
        class SVResTex: public SVRObjBase{
        public:
            SVResTex(SVInst* _app);
            
            virtual ~SVResTex();
            
            virtual void create(SVRendererPtr _renderer);
            
            virtual void destroy(SVRendererPtr _renderer);
            
            virtual void setTexData(void *_data, s32 _len);
            
            virtual void commit();  //数据提交到显卡

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
            inline u32  getTexID(){ return m_id; }
            inline u32  getuid(){ return m_uid; }
            inline bool getbLoad(){ return m_bLoad; }
            inline bool getMipMap(){ return m_enableMipMap; }
            
        protected:
            SVString m_name;
            SVLockPtr m_texLock;
            u32     m_uid;          //唯一ID对象
            u32     m_id;           //纹理ID
            s32     m_type;         //类型
            s32     m_width;        //宽
            s32     m_height;       //高
            s32     m_informate;    //内部格式
            s32     m_dataformate;  //数据格式
            bool    m_bLoad;
            bool    m_enableMipMap;
        };
        
    }//!namespace render
}//!namespace sv


#endif /* SVResTex_h */
