//
// SVUIButton.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_UIBUTTON_H
#define SV_UIBUTTON_H

#include "SVUIPanel.h"
typedef void (*sv_uibutton_callback)(SVUIButtonPtr _button, void* _obj);
namespace sv {
    
    namespace node{
        /*
         ui 按钮
         */
        class SVUIButton: public SVUIPanel {
        public:
            SVUIButton(SVInst *_app);
            
            SVUIButton(SVInst *_app,f32 _w,f32 _h);
            
            ~SVUIButton();
            
            bool procEvent(SVEventPtr _event);
            
            void setCallBack(sv_uibutton_callback _cb, void* _obj);
        protected:
            void* m_p_cb_obj;
            sv_uibutton_callback m_callback;
        };
        
    }//!namespace node
    
}//!namespace sv


#endif //SV_UIBUTTON_H
