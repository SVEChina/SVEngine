//
// SVRedPacketRun.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_REDPACKET_RUN_H
#define SV_REDPACKET_RUN_H

#include "SVGameRun.h"
#include "../base/SVVec3.h"
#include "../base/SVBounds.h"
#include "../act/SVActDeclare.h"

namespace sv{
    class SVRedPacketUnit: public SVGBase{
    public:
        enum UNITSTATE{
            UNIT_WAIT = -1,
            UNIT_FREE,
            UNIT_USE,
            UNIT_SELECTED,
            UNIT_OPEN_EXPLOSION,
            UNIT_BECOMEBIGGER,
            UNIT_SHOWGOLD,
            UNIT_OPENOTHING,
            UNIT_DISAPPEAR,
            UNIT_END
        };
        enum PACKETUNITTYPE{
            PACKET_TYPE_NONE = 0,
            PACKET_TYPE_1,
            PACKET_TYPE_2,
            PACKET_TYPE_3,
            PACKET_TYPE_4,
            PACKET_TYPE_5,
            PACKET_TYPE_6,
            PACKET_TYPE_7,
            PACKET_TYPE_8
        };
        SVRedPacketUnit(SVInst *_app);
        
        ~SVRedPacketUnit();
        
        void init(PACKETUNITTYPE _type, SVSpineNodePtr _nodePtr);
        
        void enter();
        
        void exit();
        
        void update(f32 _dt);
        
        void destory();
        
        void setBeginPos(FVec3 _pos);
        
        void setEndPos(FVec3 _pos);
        
        void setRot(FVec3 _rot);
        
        void setScale(f32 _scale);
        
        void setSpeed(f32 _speed);
        
        void setUnitName(cptr8 _name);
        
        cptr8 getUnitName();
        
        void setUnitAniName(cptr8 _name);
        
        cptr8 getUnitAniName();
        
        bool checkMoveOutBoundary();
        
        SVSpineNodePtr getSpineNode();
        
        void readyOpen();//红包准备打开，开始变大
        
        void close();
        
        void disappear();
        
        void openNothing();
        
        void setState(UNITSTATE _state);
        
        UNITSTATE getState();
        
        PACKETUNITTYPE getType();
        
        void setFlyTargetPos(FVec3 _pos);
        
        void generateGoldUnit(SVSpineNodePtr _spineNode, SVBitFontNodePtr _bitFontNode);
        
        SVGoldUnitPtr getGoldUnit();
        
        void setAniEnd();
        
        void setActEnd();
        
        bool isOpening();
    protected:
        void _openExplosion();
        
        void _openGoldUnit();
        
        void _closeGoldUnit();
    protected:
        SVString m_name;
        SVString m_aniName;
        bool m_aniEnd;
        bool m_actEnd;
        bool m_isOpening;
        FVec3 m_beginPos;
        FVec3 m_endPos;
        FVec3 m_rot;
        f32 m_scale;
        f32 m_speed;
        SVSpineNodePtr m_spineNode;
        SVActionUnitPtr m_actUnit;
        UNITSTATE m_state;
        PACKETUNITTYPE m_type;
        FVec3 m_flyTarPos;
        SVBoundBox m_box;
        SVGoldUnitPtr m_goldUnit;
    };
    
    
    class SVRedPacketUnitMgr: public SVGBase{
    public:
        SVRedPacketUnitMgr(SVInst *_app);
        
        ~SVRedPacketUnitMgr();
        
        void update(f32 _dt);
        
        void init();
        
        void destory();
        
        void exit();
        
        void enter();
        
        void setMaxSpeed(s32 _speed);
        
        void setMinSpeed(s32 _speed);
        
        void setWillShowPacketNum(s32 _num){ m_willShowPacketNum = _num;}
        
        s32 getWillShowPacketNum(){ return m_willShowPacketNum;}
        
        void generatePacketUnit(SVSpineNodePtr _nodePtr , SVRedPacketUnit::PACKETUNITTYPE _type);
        
        s32 getUnitType1Num(){ return m_unitType1Num; }
        
        s32 getUnitType2Num(){ return m_unitType2Num; }
        
        s32 getUnitType3Num(){ return m_unitType3Num; }
        
        s32 getUnitType4Num(){ return m_unitType4Num; }
        
        s32 getUnitType5Num(){ return m_unitType5Num; }
        
        s32 getUnitType6Num(){ return m_unitType6Num; }
        
        s32 getUnitType7Num(){ return m_unitType7Num; }
        
        s32 getUnitType8Num(){ return m_unitType8Num; }
        
        SVArray<SVRedPacketUnitPtr> getAllFreePacketUnit();
        
        SVArray<SVRedPacketUnitPtr> getAllUsePacketUnit();
        
        bool findSelectPacketUnit(SVNodePtr _node);
        
        bool disappearOnePacketWithID(s32 _id);
    protected:
        void _genPacketDown(SVRedPacketUnitPtr _unitPtr);
        s32 m_unitType1Num;
        s32 m_unitType2Num;
        s32 m_unitType3Num;
        s32 m_unitType4Num;
        s32 m_unitType5Num;
        s32 m_unitType6Num;
        s32 m_unitType7Num;
        s32 m_unitType8Num;
        s32 m_willShowPacketNum;//红包出现总次数
        SVLockPtr m_unitMgrLock;
        s32 m_numAppearPerSec;
        f32 m_accTime;
        f32 m_lastSpeed;
        s32 m_maxSpeed;
        s32 m_minSpeed;
        f32 m_lastPacketPosX;
        f32 m_lastPacketPosY;
        typedef SVArray<SVRedPacketUnitPtr> PACKETPOOL;
        PACKETPOOL m_packetPoolUse;
        PACKETPOOL m_packetPoolFree;
    public:
        SVRedPacketUnitPtr m_selectUnit;
    };
    
    class SVGoldUnit: public SVGBase{
    public:
        enum GOLDUNITSTATE{
            GOLDUNIT_STATE_WAIT = -1,
            GOLDUNIT_STATE_SHOW,
            GOLDUNIT_STATE_MOVE,
            GOLDUNIT_STATE_END
        };
        SVGoldUnit(SVInst *_app);
        
        ~SVGoldUnit();
        
        void init(SVSpineNodePtr _spineNode, SVBitFontNodePtr _bitFontNode);
        
        void enter();
        
        void exit();
        
        void update(f32 _dt);
        
        void destory();
        
        SVSpineNodePtr getSpineNode();
        
        SVBitFontNodePtr getBitFontNode();
        
        void setScale(FVec3 _scale);
        
        void setBeginPos(FVec3 _pos);
        
        void setEndPos(FVec3 _pos);
        
        void setSpeed(f32 _speed);
        
        void setState(GOLDUNITSTATE _state);
        
        GOLDUNITSTATE getState();
        
        void setMoney(s32 _money);
        
        s32 getMoney();
        
        void setAniEnd();
        
        void setActEnd();
        
        bool isEnd();
    protected:
        void _openBitFont();
        
        void _closeBitFont();
    protected:
        SVSpineNodePtr m_spineNode;
        
        SVBitFontNodePtr m_bitfontNode;
        
        SVActionUnitPtr m_actMoveUnit;
        
        SVActionUnitPtr m_actScaleUnit;
        
        bool m_isAniEnd;
        
        bool m_isActEnd;
        
        bool m_isEnd;
        
        f32 m_accTime;
        
        FVec3 m_beginPos;
        
        FVec3 m_endPos;
        
        f32 m_speed;
        
        s32 m_money;
        
        GOLDUNITSTATE m_state;
    };
    
    class SVTouchAniUnit : public SVGBase {
    public:
        enum UNITSTATE{
            UNIT_STATE_WAIT = 0,
            UNIT_STATE_RUN,
            UNIT_STATE_END
        };
        
        SVTouchAniUnit(SVInst* _app);
        
        ~SVTouchAniUnit();
        
        void init(SVSpineNodePtr _spineNode);
        
        void destroy();
        
        void update(f32 _dt);
        
        void setAniEnd();
        
        bool isEnd();
        
        void enter();
        
        void exit();
        
        void setScale(FVec3 _scale);
        
        void setPos(FVec3 _pos);
    
        void setState(UNITSTATE _state);
        
        UNITSTATE getState();
    protected:
        SVSpineNodePtr m_spineNode;
        
        UNITSTATE m_state;
        
        bool m_isAniEnd;
        
    };
    
    class SVTouchAniUnitMgr: public SVGBase{
    public:
        SVTouchAniUnitMgr(SVInst *_app);
        
        ~SVTouchAniUnitMgr();
        
        void update(f32 _dt);
        
        void init();
        
        void destory();
        
        void exit();
        
        void enter();
        
        void generateUnit(SVSpineNodePtr _spineNode);
        
        SVTouchAniUnitPtr getOneUnit();
    protected:
        void _recyclUnit();
    protected:
        typedef SVArray<SVTouchAniUnitPtr> UNITPOOL;
        UNITPOOL m_unitPool;
    };
    
    class SVRedPacketRun : public SVGameRun {
    public:
        SVRedPacketRun(SVInst* _app);
        
        ~SVRedPacketRun();
        
        void init();
        
        void destroy();
        
        void update(f32 _dt);
        
        bool isEnd();
        
        void enter();
        
        void exit();
        
        void setRunningTime(f32 _time);
        
        void setPacketUnitMgr(SVRedPacketUnitMgrPtr _unitMgr);
        
        SVRedPacketUnitMgrPtr getPacketUnitMgr();
        
        void setTouchAniUnitMgr(SVTouchAniUnitMgrPtr _unitMgr);
        
        SVTouchAniUnitMgrPtr getTouchAniUnitMgr();
        
        void openPacket(s32 _value, s32 _targetX, s32 _targetY);
        
        void closePacket();
        
        void touchAni(FVec3 _pos);
    protected:
        
        SVRedPacketUnitMgrPtr m_pRedPacketMgr;
        
        SVTouchAniUnitMgrPtr m_pTouchAniMgr;
        
        SVLockPtr m_lock;
        
        f32 m_accTime;
    };
}//!namespace sv



#endif //SV_REDPACKET_RUN_H
