//
// Created by lidm on 17/9/5.
//

#ifndef SV_QCLICK_BATUI_H
#define SV_QCLICK_BATUI_H

#include "../event/SVEventProc.h"
#include "../base/SVVector3d.h"
#include "../base/SVPreDeclare.h"

//战斗主UI
class SVNode;
class SVSpineNode;
class SVSpriteNode;
class SVUIParamBase;
class SVFaceDanceHelpSys;
class SVFreeTypeNode;
class SVBitFontNode;
class SVTexture;
class SVQClickMain;

struct PlayerUI{
    PlayerUI(){
        m_exist = false;
        m_name = "";
        m_score = 0;
        m_uiBg = nullptr;
        m_uiHead = nullptr;
        m_uiScore = nullptr;
        m_uiName = nullptr;
        m_pHeadTex = nullptr;
    };
    
    bool m_exist;
    stringc m_name;             //名称
    s32 m_score;                //分数
    stringc m_bgtexname;        //背景纹理名称
    SVSpriteNode* m_uiBg;       //背景
    SVSpriteNode* m_uiHead;     //头像
    SVBitFontNode *m_uiScore;   //游戏分数
    SVFreeTypeNode *m_uiName;   //人物名称
    SVTexturePtr m_pHeadTex;      //头像纹理
};

//
class SVQClickBatUI : public SVEventProc {
public:
    SVQClickBatUI(SVInst *_app,SVQClickMain* _gamemain);

    ~SVQClickBatUI();
    
    virtual void init();
    
    virtual void destroy();
    
    virtual void enter();
    
    virtual void exit();
    
    virtual void update(f32 _dt);
    
    virtual bool procEvent(SVEvent *_event);
    
    void setPersonData(s32 _code,cptr8 _name);
    
    void setHead(s32 _code,s32 _headw,s32 _headh,void* _headata);
    
    void setResultAni(cptr8 _spinename);
    
protected:
    void _gamestart();
    void _gameover(s32 _num);
    //
    bool m_exe; //是否UI在执行状态
    PlayerUI m_player[5];
    SVQClickMain* m_pGameMain;
    //
    SVNode* m_pRowNode;             //横向结果
    SVSpineNode* m_uiResult;     //结果
    //
    stringc m_spine_mingci;
    stringc m_spine_result;
};


#endif //SV_QCLICK_BATUI_H
