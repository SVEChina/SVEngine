//
// Created by lidm on 17/9/5.
//

#include "SVQClickBatUI.h"
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
#include "../base/SVUtil.h"
#include "../third/rapidjson/document.h"
#include "../third/rapidjson/prettywriter.h"
#include "../third/rapidjson/writer.h"
#include "../third/rapidjson/stringbuffer.h"

SVQClickBatUI::SVQClickBatUI(SVInst *_app,SVQClickMain* _gamemain)
:SVEventProc(_app)
,m_pGameMain(_gamemain){
    m_uiResult = nullptr;
    m_pRowNode = nullptr;
    m_exe = false;
}

SVQClickBatUI::~SVQClickBatUI() {
    m_pGameMain = nullptr;
}

void SVQClickBatUI::init(){
    //
    SVNodeParam t_nodeparam;
    t_nodeparam.m_pos.set(0.0f,0.0f,0.0f);
    m_pRowNode = t_nodeparam.genNode(mApp);
    m_pRowNode->setvisible(false);
    //
    stringc t_texresname = stringc(m_pGameMain->getResPath())+"hong.png";
    SVTexturePtr tTmpTex = mApp->m_pGlobalMgr->m_pTexMgr->getTextureSync(t_texresname.c_str(),true);
    SVSpriteNodeParam t_sp_param;
    t_sp_param.m_width = 120;
    t_sp_param.m_height = 106;
    SVBitFontNodeParam t_bmp_param;
    t_bmp_param.m_fonttex = stringc(m_pGameMain->getResPath()) + "font_score.png";
    t_bmp_param.m_content = "0";
    t_bmp_param.m_fontw = 30;
    t_bmp_param.m_fonth = 40;
    t_bmp_param.m_pos.set(0.0f,-25.0f,0.0f);
    SVSpriteNodeParam t_head_param;
    t_head_param.m_width = 40;
    t_head_param.m_height = 40;
    t_head_param.m_pos.set(-30.0f,25.0f,0.0f);
    t_head_param.m_texname = stringc(m_pGameMain->getResPath()) + "hong.png";
    SVFreetypeNodeParam t_name_param;
    t_name_param.m_pos.set(25.0,25.0f,0.0f);
    t_name_param.m_scale.set(0.5f,0.5f,0.5f);
    //
    for(s32 i=0;i<5;i++){
        t_sp_param.m_pos.set((120.0f+20.0f)*i+60.0f,0.0,0.0f);
        if(i == 0){
            t_sp_param.m_texname = stringc(m_pGameMain->getResPath())+"bg1.png";
            t_bmp_param.m_content = "0";
            t_name_param.m_text = "CR";
        }else if(i == 1){
            t_sp_param.m_texname = stringc(m_pGameMain->getResPath())+"bg2.png";
            t_bmp_param.m_content = "0";
            t_name_param.m_text = "热狗";
        }else if(i == 2){
            t_sp_param.m_texname = stringc(m_pGameMain->getResPath())+"bg3.png";
            t_bmp_param.m_content = "0";
            t_name_param.m_text = "崔哥";
        }else if(i == 3){
            t_sp_param.m_texname = stringc(m_pGameMain->getResPath())+"bg4.png";
            t_bmp_param.m_content = "0";
            t_name_param.m_text = "狗仔";
        }else if(i == 4){
            t_sp_param.m_texname = stringc(m_pGameMain->getResPath())+"bg5.png";
            t_bmp_param.m_content = "0";
            t_name_param.m_text = "翠嫂";
        }
        //
        m_player[i].m_exist = false;
        m_player[i].m_name = t_name_param.m_text;
        m_player[i].m_uiScore = (SVBitFontNode*)(t_bmp_param.genNode(mApp));
        m_player[i].m_uiHead = (SVSpriteNode*)(t_head_param.genNode(mApp));
        m_player[i].m_uiName = (SVFreeTypeNode*)(t_name_param.genNode(mApp));
        m_player[i].m_uiBg = (SVSpriteNode*)(t_sp_param.genNode(mApp));
        m_player[i].m_pHeadTex = tTmpTex;
    }
}

void SVQClickBatUI::destroy(){
    SAFE_DELETE(m_pRowNode);
    SAFE_DELETE(m_uiResult);
    for(s32 i=0;i<5;i++){
        SAFE_DELETE(m_player[i].m_pHeadTex);
        SAFE_DELETE(m_player[i].m_uiScore);
        SAFE_DELETE(m_player[i].m_uiHead);
        SAFE_DELETE(m_player[i].m_uiName);
        SAFE_DELETE(m_player[i].m_uiBg);
    }
}

void SVQClickBatUI::enter(){
    m_exe = true;
    SVScene* t_sc = mApp->m_pGlobalMgr->m_pSceneMgr->getMainScene();
    if( !t_sc )
        return;
    for(s32 i=0;i<5;i++){
        if(m_player[i].m_uiBg){
            m_player[i].m_uiBg->addChild(m_player[i].m_uiScore);
            m_player[i].m_uiBg->addChild(m_player[i].m_uiHead);
            m_player[i].m_uiBg->addChild(m_player[i].m_uiName);
            if(m_player[i].m_uiHead){
                SVMtlCorePtr t_mtl = MakeSharedPtr<SVMtlCore>(mApp, "normal2d");
                t_mtl->init();
                t_mtl->setTexture0(m_player[i].m_pHeadTex);
                m_player[i].m_uiHead->setMtl(t_mtl);
            }
            m_player[i].m_uiBg->setvisible( false );
        }
        m_pRowNode->addChild(m_player[i].m_uiBg);
    }
    //设置了头像node所在场景的顺序
    t_sc->addNode(m_pRowNode, 100);
//    m_pRowNode->setvisible(true);
    mApp->m_pGlobalMgr->m_pEventMgr->registProcer(this);
    
//    for (s32 i = 0; i< 5; i++) {
//        setPersonData(i, "sve001");
//    }
}

void SVQClickBatUI::exit(){
    m_exe = false;
    SVScene* t_sc = mApp->m_pGlobalMgr->m_pSceneMgr->getMainScene();
    if( !t_sc )
        return;
    mApp->m_pGlobalMgr->m_pEventMgr->unregistProcer(this);
    if(m_uiResult){
        t_sc->removeNode(m_uiResult);
    }
    if(m_pRowNode){
        t_sc->removeNode(m_pRowNode);
    }
    for(s32 i=0;i<5;i++){
        if(m_player[i].m_exist){
            if(m_player[i].m_uiScore){
                m_player[i].m_uiScore->removeFromParent();
            }
            if(m_player[i].m_uiHead){
                m_player[i].m_uiHead->removeFromParent();
            }
            if(m_player[i].m_uiName){
                m_player[i].m_uiName->removeFromParent();
            }
            if(m_player[i].m_uiBg){
                m_player[i].m_uiBg->removeFromParent();
            }
        }
    }
}

void SVQClickBatUI::update(f32 _dt){
    for(s32 i=0;i<5;i++){
        if(m_player[i].m_exist){
            if(m_player[i].m_uiScore){
                m_player[i].m_uiScore->setText(m_player[i].m_score);
            }
            if(m_player[i].m_uiName){
                m_player[i].m_uiName->setText(m_player[i].m_name.c_str());
            }
        }
    }
}

void SVQClickBatUI::setPersonData(s32 _code,cptr8 _name){
    if(_code<0 || _code>4)
        return ;
    m_player[_code].m_exist = true;
    m_player[_code].m_name = _name;
    m_player[_code].m_score = 0;
    if(m_player[_code].m_uiBg){
        m_player[_code].m_uiBg->setvisible(true);
    }
}

void SVQClickBatUI::setHead(s32 _code,s32 _headw,s32 _headh,void* _headata){
    if(_code<0 || _code>4)
        return ;
    bool t_head_refresh = false;
    if(_headata){
        t_head_refresh = true;
        SAFE_DELETE(m_player[_code].m_pHeadTex);
        m_player[_code].m_pHeadTex = mApp->m_pGlobalMgr->m_pTexMgr->createUnctrlTextureWithData(_headw,_headh,GL_RGBA,(void*)_headata);
    }
    //
    if(m_player[_code].m_uiBg && t_head_refresh){
//        m_player[_code].m_exist = true;
//        m_player[_code].m_uiBg->setvisible(true);
        if(m_player[_code].m_uiHead){
            SVMtlCorePtr t_mtl = MakeSharedPtr<SVMtlCore>(mApp, "normal2d");
            t_mtl->init();
            t_mtl->setTexture0(m_player[_code].m_pHeadTex);
            m_player[_code].m_uiHead->setMtl(t_mtl);
        }
    }
}

void SVQClickBatUI::setResultAni(cptr8 _spinename){
    m_spine_result = _spinename;
}

void SVQClickBatUI::_gamestart(){
    s32 t_personnum = 0;
    for(s32 i=0;i<5;i++){
        if(m_player[i].m_exist){
            t_personnum++;
        }
    }
    if(m_pRowNode){
        f32 t_seg_len = 120.0f*t_personnum + (t_personnum-1)*20.0f;
        f32 t_win_w = 720;
        f32 t_win_h = 1280;
        f32 t_off_x = t_win_w*0.5f;
        f32 t_off_y = t_win_h*0.5f;
        //
        f32 t_x = -0.5f*t_seg_len;
        f32 t_y = t_win_h-73.0f-t_off_y;
        f32 t_z = 0.0f;
        m_pRowNode->setPosition( t_x,t_y,t_z);
        m_pRowNode->setvisible(true);
    }
}

void SVQClickBatUI::_gameover(s32 _num){
    if(!m_exe)
        return;
    SVSpineNodeParam spineparam;
    spineparam.m_spinename = m_spine_result;
    spineparam.m_aniname = stringc("flyscreen") + stringc(_num);
    spineparam.m_changeAni = true;
    m_uiResult = (SVSpineNode*)spineparam.genNode(mApp);
    SVScene* t_sc = mApp->m_pGlobalMgr->m_pSceneMgr->getMainScene();
    if(m_uiResult && t_sc){
        t_sc->addNode(m_uiResult);
        m_uiResult->setPosition(0.0f, -640.0f, 0.0f);
        m_uiResult->setloop(false);
        m_uiResult->play(spineparam.m_aniname.c_str());
    }
}

bool SVQClickBatUI::procEvent(SVEvent *_event){
    //增加分数
    SVEventAddScore* t_event_addscore = dynamic_cast<SVEventAddScore*>(_event);
    if(t_event_addscore){
        s32 t_code = t_event_addscore->m_playerCode;
        if(t_code<0 || t_code>4)
            return true;
        if(m_player[t_code].m_uiScore){
            m_player[t_code].m_score += s32(t_event_addscore->m_score);
            //m_player[t_code].m_uiScore->setText(m_player[t_code].m_score);
            SV_LOG_INFO("add score num %d \n",m_player[t_code].m_score);
            if(m_pGameMain && m_pGameMain->getCallback() ){
                //write json
                RAPIDJSON_NAMESPACE::Document document;
                document.SetObject();
                RAPIDJSON_NAMESPACE::Document::AllocatorType& allocator = document.GetAllocator();
                RAPIDJSON_NAMESPACE::Value vPlayerName;
                vPlayerName.SetString(m_player[t_code].m_name.c_str(), m_player[t_code].m_name.size());
                document.AddMember("name", vPlayerName, allocator);
                document.AddMember("code", m_pGameMain->getPlayerCode(), allocator);
                document.AddMember("score", m_player[t_code].m_score, allocator);
                RAPIDJSON_NAMESPACE::StringBuffer buffer;
                RAPIDJSON_NAMESPACE::Writer<RAPIDJSON_NAMESPACE::StringBuffer> writer(buffer);
                document.Accept(writer);
                SV_LOG_INFO("out: %s", buffer.GetString());
                m_pGameMain->getCallback()(1111,buffer.GetString());
            }
        }
    }
    //设置分数
    SVEventSetScore* t_event_setscore = dynamic_cast<SVEventSetScore*>(_event);
    if(t_event_setscore){
        stringc t_name = t_event_setscore->m_playername;
        for(s32 i=0;i<5;i++){
            if(m_player[i].m_exist && (m_player[i].m_name == t_name) ){
                m_player[i].m_score = s32(t_event_setscore->m_score);
                m_player[i].m_uiScore->setText(m_player[i].m_score);
                SV_LOG_INFO("set score num %d \n",m_player[i].m_score);
            }
        }
    }
    //游戏开始
    SVEventGameStart* t_event_game_start = dynamic_cast<SVEventGameStart*>(_event);
    if(t_event_game_start){
        ///m_state = 1;    //游戏进行状态
        _gamestart();
    }
    //游戏结束
    SVGameOver* t_event_game_over = dynamic_cast<SVGameOver*>(_event);
    if(t_event_game_over){
        _gameover( m_pGameMain->getMingci() );
    }
    return true;
}
