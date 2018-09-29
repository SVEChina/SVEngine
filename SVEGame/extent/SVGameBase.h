//
// Created by lidm on 17/9/5.
//

#ifndef SV_GAMEBASE_H
#define SV_GAMEBASE_H

#include "../event/SVEventProc.h"
class SVGameUIMgr;
class SVGameBase : public SVEventProc  {
public:
    SVGameBase(SVInst* _app);

    ~SVGameBase();
    
    virtual void init();
    
    virtual void destroy();
    
    virtual void update(f32 _dt);
    
    void setCallback(cb_func_game _cb);

    cb_func_game getCallback();
    
    virtual void pushData(cptr8 _data){};
    
    virtual void setHead(s32 _playercode,s32 _width,s32 _height,void* _pData){}
    
    SVGameUIMgr* getUIMgr();
    
private:
    SVGameUIMgr* m_pUIMgr;
protected:
    cb_func_game m_pCB;
};


#endif //SV_GAMEBASE_H
