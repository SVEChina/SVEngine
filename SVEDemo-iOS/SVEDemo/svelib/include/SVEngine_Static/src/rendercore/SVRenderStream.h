//
// SVRenderStream.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_RENDER_STREAM_H
#define SV_RENDER_STREAM_H

#include "../base/SVObject.h"
#include "SVRenderDeclare.h"

namespace sv {
    
    namespace render{
        
        class SVRenderStream : public SVObject {
            friend class sv::SVRenderMgr;
        public:
            SVRenderStream();
            
            ~SVRenderStream();
            
            void render();
            
            void addSVRenderCmd(SVRenderCmdPtr cmd);
            
            void clearSVRenderCmd();
            
        protected:
            typedef SVArray<SVRenderCmdPtr> CMDPOOL;
            CMDPOOL m_cmdArray;
        };
        
    }//!namespace render
    
}//!namespace sv


#endif //SV_RENDER_STREAM_H
