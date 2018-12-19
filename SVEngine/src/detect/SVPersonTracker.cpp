//
// SVPersonTracker.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVPersonTracker.h"
#include "../basesys/SVConfig.h"
#include "../event/SVEvent.h"
#include "../event/SVEventMgr.h"
#include "../app/SVInst.h"

#define STANDARD_EYE_STD 190
#define STANDARD_NOISE_STD 160  //从眼角中心点到鼻尖

SVPersonTracker::SVPersonTracker(SVInst* _app)
:SVGBase(_app) {
    m_img_w = 1024;
    m_img_h = 1024;
    m_personID = 0;
    m_scale.set(1.0f, 1.0f, 1.0f);
    m_standardEyeDis = STANDARD_EYE_STD;
}

SVPersonTracker::~SVPersonTracker() {

}

void SVPersonTracker::track_st(void *_data, s32 _ptnum, SVRect *_rect, f32 yaw, f32 pitch, f32 roll, s32 personID) {
    f32 *pdata = (f32 *) _data;
    m_personID = personID;
    m_img_w = mApp->m_pGlobalParam->m_inner_width;
    m_img_h = mApp->m_pGlobalParam->m_inner_height;
    //面部区域
    m_faceRect = *_rect;
    {
        //双眼正中位置
        m_eyecenter.x = pdata[43 * 2];
        m_eyecenter.y = pdata[43 * 2 + 1];
        //面部中心位置
        m_facecenter.x = pdata[45 * 2];
        m_facecenter.y = pdata[45 * 2 + 1];
        //鼻子上
        m_noiseup.x = pdata[44 * 2];
        m_noiseup.y = pdata[44 * 2 + 1];
        //鼻子中心
        m_noisecenter.x = pdata[46 * 2];
        m_noisecenter.y = pdata[46 * 2 + 1];
        //鼻子下
        m_noisedown.x = pdata[49 * 2];
        m_noisedown.y = pdata[49 * 2 + 1];
        //下巴最下方位置
        m_jawbottompos.x = pdata[16 * 2];
        m_jawbottompos.y = pdata[16 * 2 + 1];

        m_eyeDistance = FVec3(pdata[74 * 2] - pdata[77 * 2], pdata[74 * 2 + 1] - pdata[77 * 2 + 1], 0).length() / cosf(fabs(yaw)*3.14/180);
        //横向缩放
        m_eyestd_scale = m_eyeDistance / m_standardEyeDis;
    }
}
