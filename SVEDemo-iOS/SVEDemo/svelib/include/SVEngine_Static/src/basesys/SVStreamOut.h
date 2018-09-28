//
// SVStreamOut.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_STREAMOUT_H
#define SV_STREAMOUT_H

#include "../base/SVGBase.h"

namespace sv {

    namespace logic {
        
        class SVStreamOut : public SVGBase {
        public:
            SVStreamOut(SVInst *_app);
            
            ~SVStreamOut();
            
            void createOutStream(cptr8 _name, s32 _type, s32 _format, s32 _steamType = 0);//加了个StreamType 当为0的时候正常读取输出流数据，当为1的时候只读取美颜完之前的数据
            
            void destroyOutStream();
            
            void active();
            
            void unactive();

            s32 getTexId();
            
            void setStreamOutCB(cb_out_stream _cb);
            
        protected:
            SVFrameOutNodePtr m_outStream;

            cb_out_stream m_pOutStreamCB;
            
            SVLockPtr m_lock;
        };
        
    }//!namespace logic

}//!namespace sv



#endif //SV_STREAMOUT_H
