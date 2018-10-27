//
// SVOutMission.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_OUTMISSION_H
#define SV_OUTMISSION_H

#include "../../event/SVEventProc.h"
#include "../SVLogicDeclare.h"

//输出任务

namespace sv{
    
    enum MISSIONSTATE{
        E_MS_RUN = 0,
        E_MS_END
    };
    
    class SVOutMission : public SVEventProc {
    public:
        SVOutMission(SVInst* _app,cptr8 _name);
        
        ~SVOutMission();
        
        virtual void output();
        
        virtual bool isEnd();
        
        void sendEnd();
        
        void setStateCallback(cb_outstream_state _cb) { m_pOutStreamStateCB = _cb; }
        
        cptr8 getMissionName();
        
    protected:
        cb_outstream_state m_pOutStreamStateCB;
        SVString m_name;
    };
    
}//!namespace sv





#endif //SV_OUTMISSION_H
