//
// SVUIBase.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_UIBASE_H
#define SV_UIBASE_H

#include "../node/SVNode.h"

namespace sv {
    
    namespace node{

        class SVUIBase: public SVNode {
        public:
            SVUIBase(SVInst *_app);
            
            ~SVUIBase();

            virtual void setArcho(EUIARCHO _cho);
            
            EUIARCHO getArcho();
            
            static SVRect getRect(EUIARCHO _archo,f32 _w,f32 _h);

        protected:
            EUIARCHO m_archo;
        };

    }//!namespace node
    
}//!namespace sv


#endif //SV_UIBASE_H
