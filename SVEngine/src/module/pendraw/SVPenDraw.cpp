//
// SVPenDraw.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVPenDraw.h"
#include "../SVGameReady.h"
#include "../SVGameRun.h"
#include "../SVGameEnd.h"
#include "../../app/SVInst.h"
#include "../../app/SVGlobalMgr.h"
#include "../../base/SVDataSwap.h"
#include "../../mtl/SVMtlCore.h"
#include "../../mtl/SVTexMgr.h"
#include "../../mtl/SVTexture.h"
#include "../../event/SVOpEvent.h"
#include "../../rendercore/SVRenderTexture.h"
#include "../../rendercore/SVRenderMgr.h"
#include "../../rendercore/SVRenderCmd.h"
#include "../../rendercore/SVRenderScene.h"
#include "../../rendercore/SVRenderObject.h"
#include "../../rendercore/SVRendererBase.h"
#include "../../basesys/SVBasicSys.h"
#include "../../basesys/filter/SVFilterGlow.h"
#include "../../basesys/filter/SVFilterBlur.h"
#include "../../basesys/filter/SVFilterBase.h"
#include "../../basesys/SVStaticData.h"
#include "../../basesys/SVConfig.h"
#include "../../basesys/SVSensorProcess.h"
#include "../../node/SVMultPassNode.h"
#include "../../detect/SVDetectMgr.h"
#include "../../detect/SVPersonTracker.h"
#include "../../file/SVFileMgr.h"
#include "../../file/SVDataBase.h"
#include "SVPenPackData.h"
SVPenDraw::SVPenDraw(SVInst *_app)
:SVGameBase(_app)
,m_curStroke(nullptr){
    m_lock = MakeSharedPtr<SVLock>();
    m_strokeWidth = mApp->m_pGlobalMgr->m_pConfig->m_strokeWidth;
    m_strokeColor = mApp->m_pGlobalMgr->m_pConfig->m_strokeColor;
    m_glowWidth = mApp->m_pGlobalMgr->m_pConfig->m_strokeGlowWidth;
    m_glowColor = mApp->m_pGlobalMgr->m_pConfig->m_strokeGlowColor;
    m_faceRot.set(0.0f, 0.0f, 0.0f);
    m_noseCenter.set(0.0f, 0.0f, 0.0f);
    m_faceEyeDis = 1.0f;
    m_mode = SV_ARMODE;
}

SVPenDraw::~SVPenDraw() {
    m_curStroke = nullptr;
    m_lock = nullptr;
    if(m_pRenderObj){
        m_pRenderObj->clearMesh();
        m_pRenderObj = nullptr;
    }
    m_fbo1 = nullptr;
    m_fbo2 = nullptr;
    m_pTex1 = nullptr;
    m_pTex2 = nullptr;
    m_mtl1 = nullptr;
    m_mesh1 = nullptr;
    m_mtl2 = nullptr;
    m_mesh2 = nullptr;
    m_glowFilter = nullptr;
    m_blurFilter = nullptr;
    m_strokes.destroy();
    m_strokesCache.destroy();
}

void SVPenDraw::init(SVGameReadyPtr _ready,SVGameRunPtr _run,SVGameEndPtr _end) {
    SVGameBase::init(_ready,_run,_end);
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    if (t_renderer) {
        SVTexturePtr t_tex = t_renderer->getSVTex(E_TEX_MAIN);
        s32 t_w = t_tex->getwidth();
        s32 t_h = t_tex->getheight();
        if (t_renderer->hasSVTex(E_TEX_HELP0)) {
            m_pTex1 = t_renderer->getSVTex(E_TEX_HELP0);
        }else{
            m_pTex1 = t_renderer->createSVTex(E_TEX_HELP0, t_w, t_h, GL_RGBA);
        }
        
        if (t_renderer->hasSVTex(E_TEX_HELP1)) {
            m_pTex2 = t_renderer->getSVTex(E_TEX_HELP1);
        }else{
            m_pTex2 = t_renderer->createSVTex(E_TEX_HELP1, t_w, t_h, GL_RGBA);
        }
    }
    m_fbo1 = MakeSharedPtr<SVRenderTexture>(mApp,m_pTex1,true,true);
    mApp->getRenderMgr()->pushRCmdCreate(m_fbo1);
    m_fbo2 = MakeSharedPtr<SVRenderTexture>(mApp,m_pTex2,true,true);
    mApp->getRenderMgr()->pushRCmdCreate(m_fbo2);
    m_pRenderObj = MakeSharedPtr<SVMultMeshMtlRenderObject>();
    m_mtl1 = MakeSharedPtr<SVMtlCore>(mApp,"screennor");
    m_mtl1->setTexcoordFlip(1.0f, 1.0f);
    m_mtl1->setTexture(0, E_TEX_HELP0);
    m_mtl1->setDepthEnable(false);
    m_mtl1->setBlendEnable(true);
    m_mtl1->setBlendState(GL_SRC_ALPHA, GL_ONE);
    m_mesh1 = mApp->getDataMgr()->m_screenMesh;
    m_mtl2 = MakeSharedPtr<SVMtlCore>(mApp,"screennor");
    m_mtl2->setTexcoordFlip(1.0f, 1.0f);
    m_mtl2->setTexture(0, E_TEX_HELP1);
    m_mtl2->setDepthEnable(false);
    m_mtl2->setBlendEnable(true);
    m_mtl2->setBlendState(GL_SRC_ALPHA, GL_ONE);
    m_mesh2 = mApp->getDataMgr()->m_screenMesh;
    //做辉光效果处理
    m_glowFilter = MakeSharedPtr<SVFilterGlow>(mApp);
    m_glowFilter->setRSType(RST_AR);
    m_glowFilter->create(E_TEX_HELP0, E_TEX_HELP0);
    //模糊效果处理
    m_blurFilter = MakeSharedPtr<SVFilterBlur>(mApp);
    m_blurFilter->setRSType(RST_AR);
    m_blurFilter->setSmooth(1.5);
    m_blurFilter->create(E_TEX_HELP1, E_TEX_HELP1);
    
}

void SVPenDraw::destroy() {
    SVGameBase::destroy();
}

void SVPenDraw::update(f32 _dt) {
    m_lock->lock();
    SVGameBase::update(_dt);
    if (m_mode == SV_ARMODE) {
        for (s32 i =0; i<m_strokes.size(); i++) {
            SVPenStrokePtr stroke = m_strokes[i];
            stroke->update(0.0f);
        }
    }else if (m_mode == SV_FACEMODE) {
        _updateFaceParam();
        for (s32 i =0; i<m_strokes.size(); i++) {
            SVPenStrokePtr stroke = m_strokes[i];
            stroke->setFaceParam(m_noseCenter, m_faceRot, m_faceEyeDis);
            stroke->update(0.0f);
        }
    }
    if (m_strokes.size() > 0) {
        _drawGlow();
        _drawStroke();
        _drawReback();
    }
    m_lock->unlock();
}

void SVPenDraw::_drawStroke(){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
    if (t_rs && t_renderer && m_pRenderObj && m_fbo2) {
        SVRenderCmdFboBindPtr t_fbo_bind = MakeSharedPtr<SVRenderCmdFboBind>(m_fbo2);
        t_fbo_bind->mTag = "pen_draw_fbo2_bind";
        t_rs->pushRenderCmd(RST_AR, t_fbo_bind);
        
        SVRenderCmdClearPtr t_clear = MakeSharedPtr<SVRenderCmdClear>();
        t_clear->mTag = "pen_draw_fbo2_clear";
        t_clear->setRenderer(t_renderer);
        t_clear->setClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        t_rs->pushRenderCmd(RST_AR, t_clear);
        {
            ////画笔触
            for (s32 i =0; i<m_strokes.size(); i++) {
                SVPenStrokePtr stroke = m_strokes[i];
                stroke->updateStroke(0.0f);
                stroke->renderStroke();
            }
            //做模糊处理
            if (m_blurFilter) {
                SVNodePtr t_node = m_blurFilter->getNode();
                SVMultPassNodePtr t_passNode = DYN_TO_SHAREPTR(SVMultPassNode, t_node)
                if (t_passNode) {
                    t_passNode->update(0.0f);
                    t_passNode->render();
                }
            }
        }
        SVRenderCmdFboUnbindPtr t_fbo_unbind = MakeSharedPtr<SVRenderCmdFboUnbind>(m_fbo2);
        t_fbo_unbind->mTag = "pen_draw_fbo2_unbind";
        t_rs->pushRenderCmd(RST_AR, t_fbo_unbind);
    }
}
void SVPenDraw::_drawGlow(){
    SVRendererBasePtr t_renderer = mApp->getRenderer();
    SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
    if (t_rs && t_renderer && m_pRenderObj && m_fbo1) {
        SVRenderCmdFboBindPtr t_fbo_bind = MakeSharedPtr<SVRenderCmdFboBind>(m_fbo1);
        t_fbo_bind->mTag = "pen_draw_fbo1_bind";
        t_rs->pushRenderCmd(RST_AR, t_fbo_bind);
        
        SVRenderCmdClearPtr t_clear = MakeSharedPtr<SVRenderCmdClear>();
        t_clear->mTag = "pen_draw_fbo1_clear";
        t_clear->setRenderer(t_renderer);
        t_clear->setClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        t_rs->pushRenderCmd(RST_AR, t_clear);
        
        {
            ////画荧光背景
            for (s32 i =0; i<m_strokes.size(); i++) {
                SVPenStrokePtr stroke = m_strokes[i];
                stroke->updateGlow(0.0f);
                stroke->renderGlow();
            }
            //做荧光模糊处理
            if (m_glowFilter) {
                SVNodePtr t_node = m_glowFilter->getNode();
                SVMultPassNodePtr t_passNode = DYN_TO_SHAREPTR(SVMultPassNode, t_node)
                if (t_passNode) {
                    t_passNode->update(0.0f);
                    t_passNode->render();
                }
            }
        }
        SVRenderCmdFboUnbindPtr t_fbo_unbind = MakeSharedPtr<SVRenderCmdFboUnbind>(m_fbo1);
        t_fbo_unbind->mTag = "pen_draw_fbo1_unbind";
        t_rs->pushRenderCmd(RST_AR, t_fbo_unbind);
        
        
    }
}
void SVPenDraw::_drawReback(){
    //再画回主纹理
    SVRenderScenePtr t_rs = mApp->getRenderMgr()->getRenderScene();
    if (m_mtl1 && m_mesh1 && m_mtl2 && m_mesh2 && m_pRenderObj) {
        m_pRenderObj->clearMesh();
        m_pRenderObj->addRenderObj(m_mesh1, m_mtl1);
        m_pRenderObj->addRenderObj(m_mesh2, m_mtl2);
        m_pRenderObj->pushCmd(t_rs, RST_AR_END, "SVPenStrokeRenderReback");
    }
}

void SVPenDraw::open() {
    SVGameBase::open();
}

void SVPenDraw::close() {
    SVGameBase::close();
}

void SVPenDraw::setStrokeWidth(f32 _width){
    m_strokeWidth = _width;
}

void SVPenDraw::setStrokeColor(FVec4 &_color){
    m_strokeColor = _color;
}

void SVPenDraw::setGlowWidth(f32 _width){
    m_glowWidth = _width;
}

void SVPenDraw::setGlowColor(FVec4 &_color){
    m_glowColor = _color;
}

void SVPenDraw::setPenMode(SVPENMODE _mode){
    m_mode = _mode;
}

void SVPenDraw::clear(){
    m_lock->lock();
    m_curStroke = nullptr;
    m_strokes.destroy();
    m_strokesCache.destroy();
    m_lock->unlock();
}

void SVPenDraw::redo(){
    m_lock->lock();
    if (m_strokesCache.size() > 0) {
        SVPenStrokePtr lastStroke = m_strokesCache.get(m_strokesCache.size() - 1);
        m_strokes.append(lastStroke);
        m_strokesCache.remove(m_strokesCache.size() - 1);
    }
    m_lock->unlock();
}

void SVPenDraw::undo(){
    m_lock->lock();
    if (m_strokes.size() > 0) {
        SVPenStrokePtr lastStroke = m_strokes.get(m_strokes.size() - 1);
        m_strokesCache.append(lastStroke);
        m_strokes.remove(m_strokes.size() - 1);
    }
    m_lock->unlock();
}

bool SVPenDraw::isRedoEnable(){
    return m_strokesCache.size();
}

bool SVPenDraw::isUndoEnable(){
    return m_strokes.size();
}

bool SVPenDraw::procEvent(SVEventPtr _event){
    if(_event->eventType == SV_EVENT_TYPE::EVN_T_TOUCH_BEGIN){
        SVTouchEventPtr t_touch = DYN_TO_SHAREPTR(SVTouchEvent,_event);
        if (!m_curStroke) {
            m_curStroke = MakeSharedPtr<SVPenStroke>(mApp, m_mode, m_strokeWidth, m_strokeColor, m_glowWidth, m_glowColor);
            m_strokes.append(m_curStroke);
        }
        m_curStroke->begin(t_touch->x,t_touch->y,0.0);
    }else if(_event->eventType == SV_EVENT_TYPE::EVN_T_TOUCH_END){
        SVTouchEventPtr t_touch = DYN_TO_SHAREPTR(SVTouchEvent,_event);
        if (t_touch && m_curStroke) {
            m_curStroke->end(t_touch->x,t_touch->y,0.0f);
            if (m_mode == SV_ARMODE) {
                
            }else if (m_mode == SV_FACEMODE) {
                m_curStroke->genFaceRawParam(m_noseCenter, m_faceRot, m_faceEyeDis);
            }
        }
        m_curStroke = nullptr;
    }else if(_event->eventType == SV_EVENT_TYPE::EVN_T_TOUCH_MOVE){
        SVTouchEventPtr t_touch = DYN_TO_SHAREPTR(SVTouchEvent,_event);
        if (t_touch && m_curStroke) {
            m_curStroke->draw(t_touch->x,t_touch->y,0.0f);
        }
    }
    return true;
}

void SVPenDraw::_updateFaceParam(){
    SVPersonPtr t_person = mApp->getDetectMgr()->getPersonModule()->getPerson(1);
    if (t_person && t_person->getExist()) {
        SVPersonTrackerPtr t_personTracker = t_person->getTracker();
        f32 m_screenH = mApp->m_pGlobalParam->m_inner_height;
        f32 t_pt_x = t_person->getFaceDataOriginalX(46);
        f32 t_pt_y = m_screenH - t_person->getFaceDataOriginalY(46);
        f32 t_yaw = t_person->getFaceRot().y;
        f32 t_roll = t_person->getFaceRot().z;
        f32 t_pitch = t_person->getFaceRot().x;
        m_noseCenter.set(t_pt_x, t_pt_y, 0.0f);
        m_faceEyeDis = t_personTracker->m_eyeDistance;
        m_faceRot.set(t_pitch, -t_yaw, t_roll);
    }
}

bool SVPenDraw::save(cptr8 _path){
    if (m_strokes.size() == 0 || m_mode == SV_ARMODE) {
        return false;
    }
    //生成Json串
    RAPIDJSON_NAMESPACE::Document jsonDoc;    //生成一个dom元素Document
    RAPIDJSON_NAMESPACE::Document::AllocatorType &allocator = jsonDoc.GetAllocator(); //获取分配器
    jsonDoc.SetObject();    //将当前的Document设置为一个object，也就是说，整个Document是一个Object类型的dom元素
    SVString t_path_pen_data = SVString(_path) + SVString("/pen.bin");
    toJSON(allocator, jsonDoc, t_path_pen_data);
    RAPIDJSON_NAMESPACE::StringBuffer buffer;
    RAPIDJSON_NAMESPACE::Writer<RAPIDJSON_NAMESPACE::StringBuffer> writer(buffer);
    jsonDoc.Accept(writer);
    SVString strJson = buffer.GetString();
    SVString new_strJson(SVDataBase::jsonFormat(strJson));
    SVDataSwapPtr t_jsonData = MakeSharedPtr<SVDataSwap>();
    t_jsonData->writeData((void *)new_strJson.c_str(), new_strJson.size());
    SVString t_path_pen_json = SVString(_path) + SVString("/config.json");
    if (!SVPenPackData::writePenData(mApp, t_jsonData, t_path_pen_json)) {
        SV_LOG_ERROR("SVPenDraw::Save Pen Json Error %s\n", t_path_pen_json.c_str());
        return false;
    }
    return true;
}

//序列化接口
void SVPenDraw::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue, cptr8 _path){
    RAPIDJSON_NAMESPACE::Value t_array(RAPIDJSON_NAMESPACE::kArrayType);
    for (s32 i = 0; i<m_strokes.size(); i++) {
        SVPenStrokePtr stroke = m_strokes[i];
        RAPIDJSON_NAMESPACE::Value t_stroke(RAPIDJSON_NAMESPACE::kObjectType);
        stroke->toJSON(_allocator, t_stroke, _path);
        t_array.PushBack(t_stroke, _allocator);
    }
    _objValue.AddMember("SVPen",t_array,_allocator);
}

void SVPenDraw::_fromJSONBase(RAPIDJSON_NAMESPACE::Value &_item){
    f32 t_strokeWidth = 0.005;
    if (_item.HasMember("stroke_width") && _item["stroke_width"].IsFloat()) {
        RAPIDJSON_NAMESPACE::Value &t_value = _item["stroke_width"];
        t_strokeWidth = t_value.GetFloat();
    }
    FVec4 t_strokeColor;
    t_strokeColor.set(255.0f, 255.0f, 255.0f, 255.0f);
    if (_item.HasMember("stroke_color") && _item["stroke_color"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &t_values = _item["stroke_color"];
        if (t_values.Size() > 3) {
            t_strokeColor.x = t_values[0].GetFloat();
            t_strokeColor.y = t_values[1].GetFloat();
            t_strokeColor.z = t_values[2].GetFloat();
            t_strokeColor.w = t_values[3].GetFloat();
        }
    }
    f32 t_strokeGlowWidth = 0.1;
    if (_item.HasMember("stroke_glowidth") && _item["stroke_glowidth"].IsFloat()) {
        RAPIDJSON_NAMESPACE::Value &t_value = _item["stroke_glowidth"];
        t_strokeGlowWidth = t_value.GetFloat();
    }
    FVec4 t_strokeGlowColor;
    t_strokeGlowColor.set(0.0f, 255.0f, 127.0f, 255.0f);
    if (_item.HasMember("stroke_glowcolor") && _item["stroke_glowcolor"].IsArray()) {
        RAPIDJSON_NAMESPACE::Value &t_values = _item["stroke_glowcolor"];
        if (t_values.Size() > 3) {
            t_strokeGlowColor.x = t_values[0].GetFloat();
            t_strokeGlowColor.y = t_values[1].GetFloat();
            t_strokeGlowColor.z = t_values[2].GetFloat();
            t_strokeGlowColor.w = t_values[3].GetFloat();
        }
    }
    m_strokeWidth = t_strokeWidth;
    m_strokeColor = t_strokeColor;
    m_glowWidth = t_strokeGlowWidth;
    m_glowColor = t_strokeGlowColor;
}

void SVPenDraw::fromJSON(RAPIDJSON_NAMESPACE::Value &_item, cptr8 _path){
    if (_item.IsArray()) {
        for(s32 i = 0; i<_item.Size(); i++){
            RAPIDJSON_NAMESPACE::Value &t_strokeValue = _item[i];
            for(auto iter = t_strokeValue.MemberBegin(); iter != t_strokeValue.MemberEnd(); ++iter){
                cptr8 key = (iter->name).GetString();
                if (t_strokeValue.HasMember(key)) {
                    RAPIDJSON_NAMESPACE::Value &t_strokeObj = iter->value;
                    _fromJSONBase(t_strokeObj);
                    m_curStroke = MakeSharedPtr<SVPenStroke>(mApp, m_mode, m_strokeWidth, m_strokeColor, m_glowWidth, m_glowColor);
                    m_curStroke->fromJSON(t_strokeObj, _path);
                    m_strokes.append(m_curStroke);
                    
                }
            }
        }
    }else if (_item.IsObject()){
        _fromJSONBase(_item);
    }
    
}
