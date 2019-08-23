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
    //
    m_absoluteMat.setIdentity();
}

void SVBone::clear() {
    m_pParent = nullptr;
    for(s32 i=0;i<m_children.size();i++) {
        m_children[i]->clear();
    }
    m_children.destroy();
}

void SVBone::update() {
    FMat4 t_transMat;
    t_transMat.setIdentity();
    t_transMat.setTranslate(m_tran);
    
    FMat4 t_scaleMat;
    t_scaleMat.setIdentity();
    t_scaleMat.setScale(m_scale);
    
    FMat4 t_rotMat;
    t_rotMat.setIdentity();
    t_rotMat.set(SVQuat(m_rot));
    
    m_relaMat = t_transMat*t_rotMat*t_scaleMat;
    if(m_pParent) {
        m_absoluteMat = m_pParent->m_absoluteMat*m_relaMat;
    }else{
        m_absoluteMat = m_relaMat;
    }
    //计算父子关系的逆矩阵
    m_resultMat = m_absoluteMat*m_invertBindMat;
    //
    for(s32 i=0;i<m_children.size();i++) {
        m_children[i]->update();
    }
}

//
SVSkeleton::SVSkeleton(){
    m_name = "ske";
    m_root = nullptr;
    m_boneArray.clear();
}

void SVSkeleton::refresh() {
    //骨架刷新 计算各自的绝对矩阵
    if(m_root) {
        m_root->update();
    }
    //传递数值
    
}

void SVSkeleton::addBone(SVBonePtr _bone) {
    m_boneArray.append(_bone);
}

s32 SVSkeleton::getBoneNum() {
    return m_boneArray.size();
}

SVBonePtr SVSkeleton::getBoneByID(s32 _id) {
    for(s32 i=0;i<m_boneArray.size();i++) {
        if( m_boneArray[i]->m_id == _id ) {
            return m_boneArray[i];
        }
    }
    return nullptr;
}

SVBonePtr SVSkeleton::getBoneByNodeID(s32 _id) {
    for(s32 i=0;i<m_boneArray.size();i++) {
        if( m_boneArray[i]->m_nodeid == _id ) {
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

//专门用于骨骼动画的时间轴
SVChannel::SVChannel() {
    m_target = 0;
    m_intertype_trans = -1;    //插值方式 linear
    m_intertype_rot = -1;    //插值方式 linear
    m_intertype_scale = -1;    //插值方式 linear
    m_intertype_weight = -1;    //插值方式 linear
    m_maxTime = 0.0f;
    m_minTime = 0.0f;
}

SVChannel::~SVChannel() {
    m_keyPool.destroy();
}

void SVChannel::update(f32 _dt,f32 _acct,s32 _rate,SVSkeletonPtr _ske) {
    SVBonePtr t_bone = _ske->getBoneByNodeID(m_target);
    if(!t_bone)
        return ;
    //二分法取key
    s32 t_preKey = findPreKey(_acct,_rate);
    s32 t_aftKey = t_preKey+1;
    if(t_aftKey>=m_keyPool.size()) {
        t_aftKey = t_preKey;
    }
    //取key测试
    if(m_target == 8) {
        f32 t_pt = m_keyPool[t_preKey]->m_time;
        f32 t_at = m_keyPool[t_aftKey]->m_time;
        if(_acct<t_pt){
            SV_LOG_INFO("astime pt error \n");
            return ;
        }
        
        if(_acct>t_at) {
            if(t_preKey!=t_aftKey) {
                SV_LOG_INFO("astime at error \n");
            }
            return ;
        }
        SV_LOG_INFO("astime %f frame %d time(%f,%f) \n",_acct,t_preKey,t_pt,t_at);
    }
    //插值
    SVASKeyPtr pKey = m_keyPool[t_preKey];
    SVASKeyPtr aKey = m_keyPool[t_aftKey];
    f32 t_pre_t = m_keyPool[t_preKey]->m_time;
    f32 t_aft_t = m_keyPool[t_aftKey]->m_time;
    //
    FVec3 t_pos = _lerp_trans(0,t_pre_t,t_aft_t,_acct,pKey->m_pos,aKey->m_pos);
    //
    FVec3 t_sc = _lerp_scale(0,t_pre_t,t_aft_t,_acct,pKey->m_scale,aKey->m_scale);
    //
    FVec4 t_rot = _lerp_rot(0,t_pre_t,t_aft_t,_acct,pKey->m_rot,aKey->m_rot);
    //
    t_bone->m_tran = t_pos;
    t_bone->m_scale = t_sc;
    t_bone->m_rot = t_rot;
    if(m_target == 8) {
        SV_LOG_INFO("aim 8 pos (%f,%f,%f) \n",t_pos.x,t_pos.y,t_pos.z);
    }
}

//平移插值
FVec3 SVChannel::_lerp_trans(s32 _mod,f32 _timepre,f32 _timenxt,f32 _timecur,FVec3 _pos1,FVec3 _pos2) {
    f32 t_dert = (_timecur - _timepre)/(_timenxt - _timepre);
    FVec3 t_result = _pos1 *(1.0f-t_dert) + _pos2*t_dert;
    return t_result;
}

//缩放插值
FVec3 SVChannel::_lerp_scale(s32 _mod,f32 _timepre,f32 _timenxt,f32 _timecur,FVec3 _scale1,FVec3 _scale2) {
    f32 t_dert = (_timecur - _timepre)/(_timenxt - _timepre);
    FVec3 t_result = _scale1 *(1.0f-t_dert) + _scale2*t_dert;
    return t_result;
}

//旋转插值
FVec4 SVChannel::_lerp_rot(s32 _mod,f32 _timepre,f32 _timenxt,f32 _timecur,FVec4 _rot1,FVec4 _rot2) {
    //四元数差值
    f32 t_dert = (_timecur - _timepre)/(_timenxt - _timepre);
    SVQuat q1(_rot1);
    SVQuat q2(_rot2);
    SVQuat t_quat= slerp(q1,q2,t_dert);
    FVec4 t_value(t_quat);
    return t_value;
}

//权重插值
f32 SVChannel::_lerp_weights() {
    return 0.0f;
}

//
s32 SVChannel::findPreKey(f32 _acct,s32 _rate) {
    //卡帧
    //修正时间
    s32 length = m_keyPool.size();
    //error
    if(length == 0 ) {
        return 0;
    }
    //最后一帧
    if( m_keyPool[length - 1]->m_time < _acct ) {
        return length - 1;
    }
    //第一帧
    if(_acct <m_keyPool[0]->m_time) {
        return 0;
    }
    s32 left = 0;
    s32 right = length - 1;
    s32 middle = 0;
    f32 t_tt = 0.0f;
    while(left <= right) {
        middle = (left + right) >> 1;
        t_tt = m_keyPool[middle]->m_time;
        if( _acct>t_tt) {
            if(right-left == 1) {
                return left;    //中间没有帧了
            }
            if(right == left) {
                return left;
            }
            //
            left = middle;
        } else if(_acct < t_tt ) {
            if(right-left == 1) {
                return left;    //中间没有帧了
            }
            if(right == left) {
                return left-1;
            }
            //
            right = middle;
        } else {
            return middle;
        }
    }
    if(right<left) {
        return middle;
    }
    if(right == left) {
        return middle;
    }
    return middle;
}

//
SVAnimateSkin::SVAnimateSkin(SVInst* _app,cptr8 _name)
:SVGBase(_app)
,m_name(_name){
    m_accTime = 0;
    m_totalTime = 7.0f;
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
    //每个轨道都走
    for(s32 i=0;i<m_chnPool.size();i++) {
        SVChannelPtr t_chan = m_chnPool[i];
        t_chan->update(_dt,m_accTime,30,m_pSke);    //24帧率
    }
    if(t_end) {
        m_accTime = 0;
    }
}



cptr8 SVAnimateSkin::getName() {
    return m_name.c_str();
}

//
SVChannelPtr SVAnimateSkin::getChannel(s32 _target){
    for(s32 i=0;i<m_chnPool.size();i++) {
        if( m_chnPool[i]->m_target == _target ) {
            return m_chnPool[i];
        }
    }
    return nullptr;
}

void SVAnimateSkin::addChannel(SVChannelPtr _chan) {
    m_chnPool.append(_chan);
}

void SVAnimateSkin::clearChannel() {
    m_chnPool.destroy();
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
