//
// Created by lidm on 17/9/5.
//

#include "SVFaceDanceHelpSys.h"
#include "SVFaceDanceEvent.h"
#include "SVFaceDanceAudioSys.h"
#include "SVGameCommonEvent.h"
#include "../app/SVInst.h"
#include "../app/SVInst.h"
#include "../event/SVEventMgr.h"

SVFaceDanceAudioSys::SVFaceDanceAudioSys(SVInst* _app)
:SVEventProc(_app)
,m_iGameBgmId(-1){
}
    
SVFaceDanceAudioSys::~SVFaceDanceAudioSys(){
}
    
void SVFaceDanceAudioSys::init(){
    enterex();
}
    
void SVFaceDanceAudioSys::destroy(){
    exitex();
    if (m_iGameBgmId >= 0){
    }
}

void SVFaceDanceAudioSys::update(float _dt){
}
    
bool SVFaceDanceAudioSys::procEvent(SVEvent *_event){
    //
    SVEventGameStart* t_event_game_start = dynamic_cast<SVEventGameStart*>(_event);
    if (t_event_game_start){
    }
    //
    SVEventFDUnitDead* t_event_unitdead = dynamic_cast<SVEventFDUnitDead*>(_event);
    if(t_event_unitdead){
    }
    return true;
}
