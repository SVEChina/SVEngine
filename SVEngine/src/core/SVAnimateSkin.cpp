//
// SVAnimateSkin.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVAnimateSkin.h"

//清理

SVBone::SVBone() {
    m_name = "";
    m_id = 0;
    m_tran.set(0.0f, 0.0f, 0.0f);
    m_scale.set(1.0f, 1.0f, 1.0f);
    m_rot.set(0.0f, 0.0f, 0.0f,1.0f);
    m_pParent = nullptr;
    m_children.clear();
}

void SVBone::clear() {
    m_pParent = nullptr;
    for(s32 i=0;i<m_children.size();i++) {
        m_children[i]->clear();
    }
    m_children.destroy();
}

void SVBone::update() {
    
}

//
SVSkeleton::SVSkeleton(){
    m_name = "ske";
    m_root = nullptr;
    m_boneArray.clear();
}


void SVSkeleton::addBone(SVBonePtr _bone) {
    m_boneArray.append(_bone);
}

SVBonePtr SVSkeleton::getBone(s32 _index) {
    for(s32 i=0;i<m_boneArray.size();i++) {
        if( m_boneArray[i]->m_id == _index ) {
            return m_boneArray[i];
        }
    }
    return nullptr;
}

void SVSkeleton::destroy() {
    //
    m_boneArray.clear();
    //
    if(m_root) {
        m_root->clear();
        m_root = nullptr;
    }
}

//
SVAnimateSkin::SVAnimateSkin(SVInst* _app,cptr8 _name)
:SVGBase(_app)
,m_name(_name){
    m_accTime = 0;
    m_totalTime = 10.0f;
    m_pSke = nullptr;
}

SVAnimateSkin::~SVAnimateSkin() {
    m_pSke = nullptr;
}

void SVAnimateSkin::bind(SVSkeletonPtr _ske){
    m_pSke = _ske;
}

void SVAnimateSkin::unbind(){
    m_pSke = nullptr;
}

void SVAnimateSkin::update(f32 _dt) {
    //计算时间
    m_accTime += _dt;
    //
    bool t_end = false;;
    if(m_accTime>m_totalTime) {
        m_accTime = m_totalTime;
        t_end = true;
    }
    if(!m_pSke){
        //没有骨架就不用跑动画了
        return ;
    }
    //
    for(s32 i=0;i<m_chnPool.size();i++) {
        SVChannelPtr t_chan = m_chnPool[i];
        //
        SVBonePtr t_bone = m_pSke->getBone(t_chan->m_target);
        if(!t_bone){
            continue;
        }
        //更新轨道部分
        s32 t_prekey = 0;
        s32 t_nxtkey = 0;
        f32 t_pretim = 0.0f;
        f32 t_nxttim = 0.0f;
        //根据输入确定key
        SVSkinAniDataPtr t_time = m_dataPool[t_chan->m_input];
        SVSkinAniDataPtr t_value = m_dataPool[t_chan->m_output];
        //key这块可以优化
        for(s32 j=0;j<t_time->m_datavec.size();j++) {
            f32 t_keytime = t_time->m_datavec[j];
            if(t_keytime>=m_accTime){
                //找到目标key了
                t_nxtkey = j;
                t_prekey = j-1;
                if(t_prekey<0){
                    t_prekey = 0;
                }
                t_pretim = t_time->m_datavec[t_prekey];
                t_nxttim = t_time->m_datavec[t_nxtkey];
                //做数据插值
                if( t_chan->m_type == E_CN_T_TRANS) {
                    //trans
                    FVec3 p1(t_value->m_datavec[3*t_prekey],
                             t_value->m_datavec[3*t_prekey+1],
                             t_value->m_datavec[3*t_prekey+2]);
                    FVec3 p2(t_value->m_datavec[3*t_nxtkey],
                             t_value->m_datavec[3*t_nxtkey+1],
                             t_value->m_datavec[3*t_nxtkey+2]);
                    FVec3 t_result = _lerp_trans(0,t_pretim,t_nxttim,m_accTime,p1,p2);
                    t_bone->m_tran = t_result;
                }else if( t_chan->m_type == E_CN_T_SCALE) {
                    //scale
                    FVec3 s1(t_value->m_datavec[3*t_prekey],
                             t_value->m_datavec[3*t_prekey+1],
                             t_value->m_datavec[3*t_prekey+2]);
                    FVec3 s2(t_value->m_datavec[3*t_nxtkey],
                             t_value->m_datavec[3*t_nxtkey+1],
                             t_value->m_datavec[3*t_nxtkey+2]);
                    FVec3 t_result = _lerp_scale(0,t_pretim,t_nxttim,m_accTime,s1,s2);
                    t_bone->m_scale = t_result;
                }else if( t_chan->m_type == E_CN_T_ROT) {
                    //rot
                    FVec4 r1(t_value->m_datavec[4*t_prekey],
                             t_value->m_datavec[4*t_prekey+1],
                             t_value->m_datavec[4*t_prekey+2],
                             t_value->m_datavec[4*t_prekey+3]);
                    FVec4 r2(t_value->m_datavec[4*t_nxtkey],
                             t_value->m_datavec[4*t_nxtkey+1],
                             t_value->m_datavec[4*t_nxtkey+2],
                             t_value->m_datavec[4*t_nxtkey+3]);
                    FVec4 t_result = _lerp_rot(0,t_pretim,t_nxttim,m_accTime,r1,r2);
                    t_bone->m_rot = t_result;
                }else if( t_chan->m_type == E_CN_T_WEIGHT) {
                    //weights
                    f32 t_value = _lerp_weights();
                }
            }
        }
    }
    //
    if(t_end) {
        m_accTime = 0;
    }
}

//平移插值
FVec3 SVAnimateSkin::_lerp_trans(s32 _mod,f32 _timepre,f32 _timenxt,f32 _timecur,FVec3 _pos1,FVec3 _pos2) {
    f32 t_dert = (_timecur - _timepre)/(_timenxt - _timepre);
    FVec3 t_result = _pos1 *(1.0f-t_dert) + _pos2*t_dert;
    return t_result;
}

//缩放插值
FVec3 SVAnimateSkin::_lerp_scale(s32 _mod,f32 _timepre,f32 _timenxt,f32 _timecur,FVec3 _scale1,FVec3 _scale2) {
    f32 t_dert = (_timecur - _timepre)/(_timenxt - _timepre);
    FVec3 t_result = _scale1 *(1.0f-t_dert) + _scale2*t_dert;
    return t_result;
}

//旋转插值
FVec4 SVAnimateSkin::_lerp_rot(s32 _mod,f32 _timepre,f32 _timenxt,f32 _timecur,FVec4 _rot1,FVec4 _rot2) {
    //四元数差值
    f32 t_dert = (_timecur - _timepre)/(_timenxt - _timepre);
    SVQuat q1(_rot1);
    SVQuat q2(_rot2);
    SVQuat t_quat= slerp(q1,q2,t_dert);
    FVec4 t_value(t_quat);
    return t_value;
}

//权重插值
f32 SVAnimateSkin::_lerp_weights() {
    return 0.0f;
}

cptr8 SVAnimateSkin::getName() {
    return m_name.c_str();
}

void SVAnimateSkin::addChannel(SVChannelPtr _chan) {
    m_chnPool.append(_chan);
}

void SVAnimateSkin::clearChannel() {
    m_chnPool.destroy();
}

void SVAnimateSkin::addSkinAniData(s32 _index,SVSkinAniDataPtr _obj) {
    m_dataPool.append(_index,_obj);
}

bool SVAnimateSkin::hadSkinAniData(s32 _index) {
    DATAPOOL::Iterator it = m_dataPool.find(_index);
    if(it != m_dataPool.end() ) {
        return true;
    }
    return false;
}

//
SVSkinMgr::SVSkinMgr(SVInst* _app)
:SVGBase(_app){
}

SVSkinMgr::~SVSkinMgr() {
    
}

void SVSkinMgr::addSkin(SVAnimateSkinPtr _skin) {
    //m_skinPool
}

void SVSkinMgr::remvoeSkin(cptr8 _name) {
    
}
