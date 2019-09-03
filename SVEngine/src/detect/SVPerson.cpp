//
// SVPerson.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVPerson.h"
#include "SVPersonTracker.h"
#include "SVDetectMgr.h"
#include "SVDetectST.h"

SVPerson::SVPerson(SVInst* _app)
:SVListenBase(_app){
    m_facePtNum = 0;
    m_personID = 0;
    m_personDirty = true;
    m_exist = false;
    m_detectType = DETECT_T_NULL;
    m_pTracker = MakeSharedPtr<SVPersonTracker>(mApp);
    m_pFaceData = new f32[MAX_FACE_PT_NUM * 2];
    m_pFaceDataOriginal= new f32[MAX_FACE_PT_NUM * 2];
    memset(m_pFaceData, 0, sizeof(f32) * 2 * MAX_FACE_PT_NUM);
    memset(m_pFaceDataOriginal, 0, sizeof(f32) * 2 * MAX_FACE_PT_NUM);
}

SVPerson::~SVPerson() {
    if (m_pFaceData) {
        delete m_pFaceData;
        m_pFaceData = nullptr;
    }
    if (m_pFaceDataOriginal) {
        delete m_pFaceDataOriginal;
        m_pFaceDataOriginal = nullptr;
    }
    m_pTracker = nullptr;
}

void SVPerson::update(f32 _dt) {
    m_dataLock.lock();
        if(!m_pTracker){
            return;
        }
        if(m_detectType == DETECT_T_AS){
            
        }else if(m_detectType == DETECT_T_FP){
     
        }else if(m_detectType == DETECT_T_ST){
            m_pTracker->track_st(m_pFaceData,
                                 m_facePtNum,
                                 &m_facerect,
                                 m_facerot.x,
                                 m_facerot.y,
                                 m_facerot.z,
                                 m_personID);
        }
    m_dataLock.unlock();
}

void SVPerson::setPersonID(s32 _id){
    m_personID = _id;
}

s32 SVPerson::getPersonID() {
    return m_personID;
}

void SVPerson::setDetectType(DETECTTYPE type) {
    if (m_detectType != type) {
        //去掉原来的监听
        SVDetectBasePtr t_detect = mApp->getDetectMgr()->getDetect();
        t_detect->unregistListenFunc(this, &SVObject::listenData);
        //增加新的监听
        m_detectType = type;
        t_detect->registListenFunc(this, &SVObject::listenData);
    }
}

void SVPerson::setFaceData(void* _pdata,s32 _len) {
    m_dataLock.lock();
    memcpy(m_pFaceData, _pdata, _len);
    m_dataLock.unlock();
}

void SVPerson::setFaceDataOriginal(void* _pdata,s32 _len){
    m_dataLock.lock();
    memcpy(m_pFaceDataOriginal, _pdata, _len);
    m_dataLock.unlock();
}

void SVPerson::setFaceRect(f32 _left,f32 _top,f32 _right,f32 _bottom){
    m_dataLock.lock();
    m_facerect.m_lb_pt.x = _left;
    m_facerect.m_lb_pt.y = _top;
    m_facerect.m_rt_pt.x = _right;
    m_facerect.m_rt_pt.y = _bottom;
    m_dataLock.unlock();
}

SVRect SVPerson::getFaceRect(){
    return m_facerect;
}

void SVPerson::setFaceRot(f32 _yaw,f32 _pitch,f32 _roll){
    m_dataLock.lock();
    m_facerot.x = _pitch;
    m_facerot.y = _yaw;
    m_facerot.z = _roll;
    m_dataLock.unlock();
}

FVec3 SVPerson::getFaceRot(){
    return m_facerot;
}

f32 *SVPerson::getFaceData() {
    return m_pFaceData;
}

f32 SVPerson::getFaceDataX(s32 _index) {
    if (_index >= 0 && _index < m_facePtNum) {
        return m_pFaceData[2 * _index];
    }
    return 0.0f;
}

f32 SVPerson::getFaceDataY(s32 _index) {
    if (_index >= 0 && _index < m_facePtNum) {
        return m_pFaceData[2 * _index + 1];
    }
    return 0.0f;
}

s32 SVPerson::getFacePtNum() {
    return m_facePtNum;
}

f32 *SVPerson::getFaceDataOriginal(){
    return m_pFaceDataOriginal;
}

f32 SVPerson::getFaceDataOriginalX(s32 _index){
    if (_index >= 0 && _index < m_facePtNum) {
        return m_pFaceDataOriginal[2 * _index];
    }
    return 0.0f;
}

f32 SVPerson::getFaceDataOriginalY(s32 _index){
    if (_index >= 0 && _index < m_facePtNum) {
        return m_pFaceDataOriginal[2 * _index + 1];
    }
    return 0.0f;
}

SVPersonTrackerPtr SVPerson::getTracker() {
    return m_pTracker;
}

void SVPerson::listenData(SVObjectPtr datagen) {
    m_dirty = true;
    if (m_detectType == DETECT_T_AS) {
        m_facePtNum = 101;// + 8 + 32 + 48;    //ARCSOFT
    } else if (m_detectType == DETECT_T_ST) {
        m_facePtNum = 106;// + 8;          //ST
        SVDetectSTPtr t_detectST = std::dynamic_pointer_cast<SVDetectST>(datagen);
        if (t_detectST) {
            _listenData_ST(t_detectST);
        }
    } else if (m_detectType == DETECT_T_FP) {
        m_facePtNum = 106;// + 8;          //FACE++
    }
    m_dirty = false;
}

void SVPerson::_listenData_ST(SVDetectSTPtr detect) {
    PERSON_STDATA *personData = detect->getData(m_personID);
    PERSON_STDATA *personDataOriginal = detect->getDataOriginal(m_personID);
    if (!personData)
        return;
    if (!personData->has){
        //开始做数据更新
        setExist(false);
        setFaceRot(0, 0, 0);
        setFaceRect(0, 0, 0, 0);
    } else {
        //开始做数据更新
        setExist(true);
        setFaceRot(personData->yaw, personData->pitch, personData->roll);
        setFaceRect(personData->rectleft, personData->recttop, personData->rectright, personData->rectbottom);
        setFaceData(personData->pointdata, sizeof(f32) * 2 * ST_POINT_NUM);
        setFaceDataOriginal(personDataOriginal->pointdata, sizeof(f32) * 2 * ST_POINT_NUM);
        if (m_pTracker) {
            m_pTracker->track_st(m_pFaceData, m_facePtNum, &m_facerect, m_facerot.y, m_facerot.x, m_facerot.z, m_personID);
        }
    }
    //通知其他监听者更新数据
    _notice(0);
}

