//
// SVStreamOut.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_STREAMOUT_H
#define SV_STREAMOUT_H

#include "SVProcess.h"
#include "../node/SVNodeDeclare.h"
#include "out/SVOutMission.h"
#include "../base/SVArray.h"

namespace sv {

    namespace logic {
        
        enum OUTMETHOD{
            E_OUT_M_NULL = 0,
            E_OUT_M_READPIEXL,
            E_OUT_M_MAP,
            E_OUT_M_PBO,
            E_OUT_M_IOS,
            E_OUT_M_ANDRIOD
        };
        
        class SVStreamOut : public SVProcess {
        public:
            SVStreamOut(SVInst *_app);
            
            ~SVStreamOut();
            
            //打开输出流
            void openOutStream();
            
            //关闭输出输出流
            void closeOutStream();
            
            bool isOpen();
            
            //改变输出流方式
            void changeOutMethod(OUTMETHOD _method);
            
            void setOutMethod(OUTMETHOD _method);
            
            void setOutSize(s32 _width,s32 _height);
            
            void setOutFormat(SV_OUTSTEAMFORMATE _format);
            
            //
            void createOutStream(cptr8 _name, s32 _type, s32 _format, s32 _steamType = 0);//加了个StreamType 当为0的时候正常读取输出流数据，当为1的时候只读取美颜完之前的数据
            
            void destroyOutStream();
            
            void active();
            
            void unactive();

            s32 getTexId();
            
            void setStreamOutCB(cb_out_stream _cb);
            
            void output();
            
        protected:
            
            void _refreshOutStream();
            
            OUTMETHOD m_outMethod;
            
            s32 m_outWidth;
            
            s32 m_outHeight;
            
            SV_OUTSTEAMFORMATE m_outFormat;
            
            SVFrameOutNodePtr m_pFrameOut;

            cb_out_stream m_pOutStreamCB;
            
            SVLockPtr m_lock;
            
        public:
            void addOutMission(SVOutMissionPtr _mission);
            
            void delOutMission(cptr8 _name);
            
            void clearAllMission();
            
        protected:
            SVLockPtr m_missionlock;
            typedef SVArray<SVOutMissionPtr> MISSIONPOOL;
            MISSIONPOOL m_mission;
        };
        
    }//!namespace logic

}//!namespace sv



#endif //SV_STREAMOUT_H
