//
// SVPipleBatUI.cpp
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//

#include "SVPipleBatUI.h"
#include "SVGameDefine.h"
#include "SVGameCommonEvent.h"
#include "SVQClickEvent.h"
#include "SVQClickMain.h"
#include "../app/SVInst.h"
#include "../node/SVBitFontNode.h"
#include "../node/SVSpriteNode.h"
#include "../node/SVFreeTypeNode.h"
#include "../node/SVSpineNode.h"
#include "../node/SVScene.h"
#include "../logicsys/SVSceneMgr.h"
#include "../core/SVSpine.h"
#include "../event/SVEventMgr.h"
#include "../texture/SVTexMgr.h"
#include "../mtl/SVMtlCore.h"
#include "../basesys/SVConfig.h"
#include "../sdata/SVDataSwap.h"
#include "../rendercore/SVRenderObject.h"
#include "../third/rapidjson/document.h"
#include "../third/rapidjson/prettywriter.h"
#include "../third/rapidjson/writer.h"
#include "../third/rapidjson/stringbuffer.h"

using namespace sv;

SVPipleBatUI::SVPipleBatUI(SVInst *_app,SVPipleMain* _gamemain)
:SVEventProc(_app)
,m_pGameMain(_gamemain)
,m_pBGMap(nullptr){
}

SVPipleBatUI::~SVPipleBatUI() {
    m_pGameMain = nullptr;
}

void SVPipleBatUI::init(){
    int as = 0;
//    SVNodeParam t_nodeparam;
//    t_nodeparam.m_pos.set(0.0f,0.0f,0.0f);
//    m_pRowNode = t_nodeparam.genNode(mApp);
//    m_pRowNode->setvisible(false);
//    //
//    stringc t_texresname = stringc(m_pGameMain->getResPath())+"hong.png";
//    SVTexture* tTmpTex = mApp->m_pGlobalMgr->m_pTexMgr->getTextureSync(t_texresname.c_str(),true);
//    SVSpriteNodeParam t_sp_param;
//    t_sp_param.m_width = 120;
//    t_sp_param.m_height = 106;
//    SVBitFontNodeParam t_bmp_param;
//    t_bmp_param.m_fonttex = stringc(m_pGameMain->getResPath()) + "font_score.png";
//    t_bmp_param.m_content = "0";
//    t_bmp_param.m_fontw = 30;
//    t_bmp_param.m_fonth = 40;
//    t_bmp_param.m_pos.set(0.0f,-25.0f,0.0f);
//    SVSpriteNodeParam t_head_param;
//    t_head_param.m_width = 40;
//    t_head_param.m_height = 40;
//    t_head_param.m_pos.set(-30.0f,25.0f,0.0f);
//    t_head_param.m_texname = stringc(m_pGameMain->getResPath()) + "hong.png";
//    SVFreetypeNodeParam t_name_param;
//    t_name_param.m_pos.set(25.0,25.0f,0.0f);
//    t_name_param.m_scale.set(0.5f,0.5f,0.5f);
//    //
//    for(s32 i=0;i<5;i++){
//        t_sp_param.m_pos.set((120.0f+20.0f)*i+60.0f,0.0,0.0f);
//        if(i == 0){
//            t_sp_param.m_texname = stringc(m_pGameMain->getResPath())+"bg1.png";
//            t_bmp_param.m_content = "0";
//            t_name_param.m_text = "CR";
//        }else if(i == 1){
//            t_sp_param.m_texname = stringc(m_pGameMain->getResPath())+"bg2.png";
//            t_bmp_param.m_content = "0";
//            t_name_param.m_text = "热狗";
//        }else if(i == 2){
//            t_sp_param.m_texname = stringc(m_pGameMain->getResPath())+"bg3.png";
//            t_bmp_param.m_content = "0";
//            t_name_param.m_text = "崔哥";
//        }else if(i == 3){
//            t_sp_param.m_texname = stringc(m_pGameMain->getResPath())+"bg4.png";
//            t_bmp_param.m_content = "0";
//            t_name_param.m_text = "狗仔";
//        }else if(i == 4){
//            t_sp_param.m_texname = stringc(m_pGameMain->getResPath())+"bg5.png";
//            t_bmp_param.m_content = "0";
//            t_name_param.m_text = "翠嫂";
//        }
//        //
//        m_player[i].m_exist = false;
//        m_player[i].m_name = t_name_param.m_text;
//        m_player[i].m_uiScore = (SVBitFontNode*)(t_bmp_param.genNode(mApp));
//        m_player[i].m_uiHead = (SVSpriteNode*)(t_head_param.genNode(mApp));
//        m_player[i].m_uiName = (SVFreeTypeNode*)(t_name_param.genNode(mApp));
//        m_player[i].m_uiBg = (SVSpriteNode*)(t_sp_param.genNode(mApp));
//        m_player[i].m_pHeadTex = tTmpTex;
//        if(m_player[i].m_pHeadTex){
//            m_player[i].m_pHeadTex->retain();
//        }
//    }
}

void SVPipleBatUI::destroy(){
//    SAFE_DELETE(m_pRowNode);
//    SAFE_DELETE(m_uiResult);
//    for(s32 i=0;i<5;i++){
//        SAFE_DELETE(m_player[i].m_pHeadTex);
//        SAFE_DELETE(m_player[i].m_uiScore);
//        SAFE_DELETE(m_player[i].m_uiHead);
//        SAFE_DELETE(m_player[i].m_uiName);
//        SAFE_DELETE(m_player[i].m_uiBg);
//    }
}

void SVPipleBatUI::enter(){
    SVScene* t_sc = mApp->m_pGlobalMgr->m_pSceneMgr->getMainScene();
    if( !t_sc )
        return;
    //地图挂到场景中
    t_sc->addNode(m_pBGMap, 100);
    enterex();
}

void SVPipleBatUI::exit(){
    exitex();
    SVScene* t_sc = mApp->m_pGlobalMgr->m_pSceneMgr->getMainScene();
    if( !t_sc )
        return;
    if(m_pBGMap){
        t_sc->removeNode(m_pBGMap);
    }
}

void SVPipleBatUI::update(f32 _dt){
    //更新新生成节点的移动
}

void SVPipleBatUI::setPipleBG(cptr8 _bg){
    m_pipleBg = _bg;
}

void SVPipleBatUI::setWorld(f32 _gridsize,f32 _gridboder){
    m_gridsize = _gridsize;
    m_gridborder = _gridboder;
}

void SVPipleBatUI::setPersonData(s32 _code,cptr8 _name){
//    if(_code<0 || _code>4)
//        return ;
//    m_player[_code].m_exist = true;
//    m_player[_code].m_name = _name;
//    m_player[_code].m_score = 0;
//    if(m_player[_code].m_uiBg){
//        m_player[_code].m_uiBg->setvisible(true);
//    }
}

void SVPipleBatUI::setResultAni(cptr8 _spinename){
    //m_spine_result = _spinename;
}

void SVPipleBatUI::_gamestart(){
}

void SVPipleBatUI::_gameover(s32 _num){
//    SVSpineNodeParam spineparam;
//    spineparam.m_spinename = m_spine_result;
//    spineparam.m_aniname = stringc("flyscreen") + stringc(_num);
//    spineparam.m_changeAni = true;
//    m_uiResult = (SVSpineNode*)spineparam.genNode(mApp);
//    SVScene* t_sc = mApp->m_pGlobalMgr->m_pSceneMgr->getMainScene();
//    if(m_uiResult && t_sc){
//        t_sc->addNode(m_uiResult);
//        m_uiResult->setPosition(0.0f, -640.0f, 0.0f);
//        m_uiResult->setloop(false);
//        m_uiResult->play(spineparam.m_aniname.c_str());
//    }
}

bool SVPipleBatUI::procEvent(SVEvent *_event){
//    //增加分数
//    SVEventAddScore* t_event_addscore = dynamic_cast<SVEventAddScore*>(_event);
//    if(t_event_addscore){
//        s32 t_code = t_event_addscore->m_playerCode;
//        if(t_code<0 || t_code>4)
//            return true;
//        if(m_player[t_code].m_uiScore){
//            m_player[t_code].m_score += s32(t_event_addscore->m_score);
//            //m_player[t_code].m_uiScore->setText(m_player[t_code].m_score);
//            SV_LOG_INFO("add score num %d \n",m_player[t_code].m_score);
//            if(m_pGameMain && m_pGameMain->getCallback() ){
//                //write json
//                RAPIDJSON_NAMESPACE::Document document;
//                document.SetObject();
//                RAPIDJSON_NAMESPACE::Document::AllocatorType& allocator = document.GetAllocator();
//                RAPIDJSON_NAMESPACE::Value vPlayerName;
//                vPlayerName.SetString(m_player[t_code].m_name.c_str(), m_player[t_code].m_name.size());
//                document.AddMember("name", vPlayerName, allocator);
//                document.AddMember("code", m_pGameMain->getPlayerCode(), allocator);
//                document.AddMember("score", m_player[t_code].m_score, allocator);
//                RAPIDJSON_NAMESPACE::StringBuffer buffer;
//                RAPIDJSON_NAMESPACE::Writer<RAPIDJSON_NAMESPACE::StringBuffer> writer(buffer);
//                document.Accept(writer);
//                SV_LOG_INFO("out: %s", buffer.GetString());
//                m_pGameMain->getCallback()(1111,buffer.GetString());
//            }
//        }
//    }
//    //设置分数
//    SVEventSetScore* t_event_setscore = dynamic_cast<SVEventSetScore*>(_event);
//    if(t_event_setscore){
//        stringc t_name = t_event_setscore->m_playername;
//        for(s32 i=0;i<5;i++){
//            if(m_player[i].m_exist && (m_player[i].m_name == t_name) ){
//                m_player[i].m_score = s32(t_event_setscore->m_score);
//                m_player[i].m_uiScore->setText(m_player[i].m_score);
//                SV_LOG_INFO("set score num %d \n",m_player[i].m_score);
//            }
//        }
//    }
//    //游戏开始
//    SVEventGameStart* t_event_game_start = dynamic_cast<SVEventGameStart*>(_event);
//    if(t_event_game_start){
//        ///m_state = 1;    //游戏进行状态
//        _gamestart();
//    }
//    //游戏结束
//    SVGameOver* t_event_game_over = dynamic_cast<SVGameOver*>(_event);
//    if(t_event_game_over){
//        _gameover( m_pGameMain->getMingci() );
//    }
    return true;
}
