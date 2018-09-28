//
// SVModuleDef.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_MODULEDEF_H
#define SV_MODULEDEF_H

#include "../base/SVPreDeclare.h"

namespace sv {
    
    class SVGameBase;
    typedef std::shared_ptr<SVGameBase> SVGameBasePtr;
    class SVGameReady;
    typedef std::shared_ptr<SVGameReady> SVGameReadyPtr;
    class SVGameRun;
    typedef std::shared_ptr<SVGameRun> SVGameRunPtr;
    class SVGameEnd;
    typedef std::shared_ptr<SVGameEnd> SVGameEndPtr;
    class SVRedPacket;
    typedef std::shared_ptr<SVRedPacket> SVRedPacketPtr;
    class SVRedPacketReady;
    typedef std::shared_ptr<SVRedPacketReady> SVRedPacketReadyPtr;
    class SVRedPacketReadyUnit;
    typedef std::shared_ptr<SVRedPacketReadyUnit> SVRedPacketReadyUnitPtr;
    class SVRedPacketRun;
    typedef std::shared_ptr<SVRedPacketRun> SVRedPacketRunPtr;
    class SVRedPacketEnd;
    typedef std::shared_ptr<SVRedPacketEnd> SVRedPacketEndPtr;
    class SVRedPacketUnit;
    typedef std::shared_ptr<SVRedPacketUnit> SVRedPacketUnitPtr;
    class SVRedPacketUnitMgr;
    typedef std::shared_ptr<SVRedPacketUnitMgr> SVRedPacketUnitMgrPtr;
    class SVGoldUnit;
    typedef std::shared_ptr<SVGoldUnit> SVGoldUnitPtr;
    class SVTouchAniUnit;
    typedef std::shared_ptr<SVTouchAniUnit> SVTouchAniUnitPtr;
    class SVTouchAniUnitMgr;
    typedef std::shared_ptr<SVTouchAniUnitMgr> SVTouchAniUnitMgrPtr;
    class SVEffectPackage;
    typedef std::shared_ptr<SVEffectPackage> SVEffectPackagePtr;
    
}//!namespace sv



#endif //SV_MODULEDEF_H
