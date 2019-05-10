//
//  SVDeformScale.cpp
//  SVEngine
//
//  Created by 徐子昱 on 2019/5/9.
//  Copyright © 2019 李晓帆. All rights reserved.
//

#include "SVDeformScale.h"
#include "../base/SVLock.h"

SVDeformScale::SVDeformScale(){
    m_lock=MakeSharedPtr<SVLock>();
    m_smooth=0.0;
    clearCtl();
}

SVDeformScale::~SVDeformScale(){
    clearCtl();
}

void SVDeformScale::setSmooth(f32 _smooth){
    m_smooth = _smooth;
}

void SVDeformScale::pushCtl(f32 x , f32 y , f32 z){
    m_controlArray.append(FVec3(x,y,z));
}

void SVDeformScale::clearCtl(){
    m_controlArray.clear();
}

FVec2 SVDeformScale::getScalePostion(const FVec2& t){
    m_lock->lock();
    FVec2 t_newCoord=t;
    f32 t_aspect=0.5625;
    f32 t_weight=0.0;
    for(s32 i = 0; i<m_controlArray.size(); i++){
        FVec2 t_control = FVec2(m_controlArray[i].x, m_controlArray[i].y);
        f32 t_dis = getDistanceFrom(FVec2(t_newCoord.x*t_aspect,t_newCoord.y), FVec2(t_control.x*t_aspect,t_control.y));
        if(t_dis < m_controlArray[i].z){
            t_weight=pow(t_dis/m_controlArray[i].z, m_smooth);
            t_newCoord.x = t_control.x +(t_newCoord.x - t_control.x) * t_weight;
            t_newCoord.y = t_control.y +(t_newCoord.y - t_control.y) * t_weight;
        }
    }
    m_lock->unlock();
    return t_newCoord;
}
