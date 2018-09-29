//
// SVPipleBatUI.h
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//

#ifndef SV_PIPLE_BATUI_H
#define SV_PIPLE_BATUI_H

#include "../event/SVEventProc.h"
#include "../base/SVVector3d.h"

//战斗主UI

class SVNode;
class SVSpineNode;
class SVSpriteNode;
class SVUIParamBase;
class SVFreeTypeNode;
class SVBitFontNode;
class SVTexture;

//
namespace sv {
    
    class SVPipleMain;
    
    class SVPipleBatUI : public SVEventProc {
    public:
        SVPipleBatUI(SVInst *_app,SVPipleMain* _gamemain);
        
        ~SVPipleBatUI();
        
        virtual void init();
        
        virtual void destroy();
        
        virtual void enter();
        
        virtual void exit();
        
        virtual void update(f32 _dt);
        
        virtual bool procEvent(SVEvent *_event);
        
        //void setMap();
        void setPipleBG(cptr8 _bg);
        
        void setWorld(f32 _gridsize,f32 _gridboder);
        
        void setPersonData(s32 _code,cptr8 _name);
        
        void setResultAni(cptr8 _spinename);
        
    protected:
        void _gamestart();
        
        void _gameover(s32 _num);
        //
        SVPipleMain* m_pGameMain;
        f32 m_gridsize;
        f32 m_gridborder;
        //
        SVSpriteNode* m_pBGMap;
        //
        stringc m_pipleBg;
        stringc m_spine_result;
    };
    
//!namespace
}



#endif //SV_QCLICK_BATUI_H
