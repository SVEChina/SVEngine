//
// Created by lidm on 17/9/5.
//

#include "SVFaceDanceUnitMgr.h"
#include "SVFaceDanceUnit.h"
#include "SVFaceDanceEvent.h"
#include "../app/SVInst.h"
#include "../event/SVEventMgr.h"

SVFaceDanceUnitMgr::SVFaceDanceUnitMgr(SVInst*_app)
:SVEventProc(_app) {
    m_acctime = 0;
    m_numPerSec = 4;
    m_maxNum = 20;
    m_maxspeed = 200;
    m_minspeed = 100;
    m_downPos  = 4;
    m_lastFaceType = 0;
    m_lastFacePos = 0;
    //
    m_model = 0;            //0 正常模式 1教学模式
    m_teachChange = false;
    m_teachAniName = "";
}

SVFaceDanceUnitMgr::~SVFaceDanceUnitMgr() {
}

void SVFaceDanceUnitMgr::init(){
    for(s32 i=0;i<m_maxNum;i++){
        SVFaceDanceUnit* t_unit = new SVFaceDanceUnit(mApp);
        t_unit->init(m_scenename.c_str(),m_spinename.c_str(),m_scorepng.c_str());
        m_faceFreePool.push_back(t_unit);
    }
    _initBornPos();
    enterex();
}

void SVFaceDanceUnitMgr::destroy(){
    //移除
    exitex();
    //使用池
    FACEPOOL::Iterator it_use = m_faceUsePool.begin();
    while( it_use!=m_faceUsePool.end() ){
        SVFaceDanceUnit* t_unit = (*it_use);
        t_unit->destroy();
        it_use++;
    }
    m_faceUsePool.clear();
    //缓存池
    FACEPOOL::Iterator it_free = m_faceFreePool.begin();
    while( it_free!=m_faceFreePool.end() ){
        SVFaceDanceUnit* t_unit = (*it_free);
        t_unit->destroy();
        it_free++;
    }
    m_faceFreePool.clear();
}

void SVFaceDanceUnitMgr::enalbeTeachMode(){
    m_model = 1;
}

void SVFaceDanceUnitMgr::enableNorMode(){
    m_model = 0;
}

void SVFaceDanceUnitMgr::stop(){
    //暂停
    FACEPOOL::Iterator it = m_faceUsePool.begin();
    while( it!=m_faceUsePool.end() ){
        SVFaceDanceUnit* t_unit = (*it);
        t_unit->stop();
        it++;
    }
}

void SVFaceDanceUnitMgr::update(f32 _dt,f32 _gametime){
    //产生Face
    //计算生成的个数
    m_acctime += _dt;
    int t_genNum = int(m_acctime*m_numPerSec);
    if(t_genNum>0 && m_faceUsePool.getSize()<m_maxNum){
        //修正累计时间
        m_acctime -= (1.0f*t_genNum/m_numPerSec);
        //生成节点
        for(s32 i=0;i<t_genNum;i++){
            SVFaceDanceUnit* t_unit = nullptr;
            FACEPOOL::Iterator it_free = m_faceFreePool.begin();
            if( it_free!=m_faceFreePool.end() ){
                t_unit = (*it_free);
                m_faceFreePool.erase(it_free);
            }
            if(t_unit){
                if(m_model == 0){
                    _genFaceUnitNor(t_unit);
                }else if(m_model == 1){
                    _genFaceUnitTeach(t_unit);
                }
                m_faceUsePool.push_back(t_unit);
            }
        }
    }
    //更新Face
    FACEPOOL::Iterator it = m_faceUsePool.begin();
    while( it!=m_faceUsePool.end() ){
        SVFaceDanceUnit* t_unit = (*it);
        if(t_unit->getFaceState() == 0 || t_unit->getFaceState() == 1 ){
            //生成或者暂停状态
            t_unit->update(_dt,_gametime);
            it++;
        }else if(t_unit->getFaceState() == 2){
            //死亡状态
            t_unit->exit();
            m_faceFreePool.push_back(t_unit);
            it = m_faceUsePool.erase(it);
        }else{
            it++;
        }
    }
}

void SVFaceDanceUnitMgr::_genFaceUnitNor(SVFaceDanceUnit* t_unit){
    //随机位置
    int i_downPos = 0;
    if(m_downPos == 1){
        i_downPos = 0;
    }else{
        do{
            i_downPos = rand()%m_downPos;
        }while( i_downPos == m_lastFacePos );
        m_lastFacePos = i_downPos;
    }
    float t_px = m_vecDownPos[i_downPos];
    float t_py = 640;

    //随机速度
    float t_speed = m_maxspeed;
    if(m_maxspeed - m_minspeed>1.0){
        t_speed = m_minspeed + rand()%(int(m_maxspeed - m_minspeed) );
    }
    //随机类型
    int t_faceType = -1;
    int t_type_num = (int)(m_faceTypePool.size());
    if(t_type_num>0){
        //
        if(t_type_num == 1){
            t_faceType = 0;
            m_lastFaceType = 0;
        }else{
            do{
                t_faceType = rand()%t_type_num;
            }while( t_faceType == m_lastFaceType );
            m_lastFaceType = t_faceType;
        }
        //
        stringc t_aniname = m_faceTypePool[t_faceType];
        t_unit->setFaceType(t_faceType, t_aniname.c_str(),m_anidead.c_str() );
    }
    t_unit->setInitPos(t_px, t_py, 0.0f);
    t_unit->setInitSpeed(t_speed);
    t_unit->setInitScore(10);
    t_unit->setInitEnery(m_enery);
    t_unit->enter();
}

void SVFaceDanceUnitMgr::_genFaceUnitTeach(SVFaceDanceUnit* t_unit){
    //随机位置
    int i_downPos = 0;
    if(m_downPos == 1){
        i_downPos = 0;
    }else{
        do{
            i_downPos = rand()%m_downPos;
        }while( i_downPos == m_lastFacePos );
        m_lastFacePos = i_downPos;
    }
    float t_px = m_vecDownPos[i_downPos];
    float t_py = 640;
    //随机速度
    float t_speed = m_maxspeed;
    if(m_maxspeed - m_minspeed>1.0){
        t_speed = m_minspeed + rand()%(int(m_maxspeed - m_minspeed) );
    }
    //教学类型 切换
    if( m_teachChange ){
        m_teachChange = false;
        m_lastFaceType++;
        if(m_lastFaceType>=m_faceTypePool.size()){
            m_lastFaceType = 0;
        }
    }
    //教学动画名称
    m_teachAniName = m_faceTypePool[m_lastFaceType];
    t_unit->setFaceType(m_lastFaceType, m_teachAniName.c_str(),m_anidead.c_str() );
    t_unit->setInitPos(t_px, t_py, 0.0f);
    t_unit->setInitSpeed(t_speed);
    t_unit->setInitScore(10);
    t_unit->setInitEnery(m_enery);
    t_unit->enter();
}

bool SVFaceDanceUnitMgr::procEvent(SVEvent *_event){
    if(m_model == 1){
        //教学模式下 有脸消亡
        SVEventFDUnitDead* t_event_unitdead = dynamic_cast<SVEventFDUnitDead*>(_event);
        if(t_event_unitdead){
            if( t_event_unitdead->m_aniname == m_teachAniName.c_str() ){
                m_teachChange = true;
            }
        }
    }
    return true;
}

void SVFaceDanceUnitMgr::_initBornPos(){
//    f32 fFaceWidth = 160.0f;
    f32 fScreenWidth = 720.0f;
    f32 fNullWidth = 110.0f;             //左右空像素
    if (m_downPos > MAX_DOWNPOS){
        m_downPos = MAX_DOWNPOS;
    }
    if(m_downPos <= 1){
        m_vecDownPos[0] = 0;
    }else{
        f32 fTotalsize = fScreenWidth - 2.0f*fNullWidth;
        f32 fSegmentLen = fTotalsize/(m_downPos - 1);
        memset(m_vecDownPos , 0 , MAX_DOWNPOS * sizeof(s32));
        for (s32 i = 0 ; i < m_downPos ; ++i){
            m_vecDownPos[i] = fNullWidth + i*fSegmentLen - 0.5 * fScreenWidth;
        }
    }
}
