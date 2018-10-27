//
// SVOutMissionNor.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_OUTMISSION_NOR_H
#define SV_OUTMISSION_NOR_H

#include "SVOutMission.h"

//输出任务

namespace sv{
    
    class SVOutMissionNor : public SVOutMission {
    public:
        SVOutMissionNor(SVInst* _app,cptr8 _name);
        
        virtual ~SVOutMissionNor();
        
        void output();
        
        bool isEnd();
        
        void setEnd();
        
    protected:
        bool m_end;
    };
    
}//!namespace sv





#endif //SV_OUTMISSION_NOR_H
