//
// Created by yinlong on 17/10/20.
//

#ifndef SV_GAMEUIBASE_H
#define SV_GAMEUIBASE_H

#include "../event/SVEventProc.h"
#include "../base/SVVector3d.h"

//UI基类
class SVGameUIMgr;
class SVGameUIShow;
class SVGameUIBase : public SVEventProc {
public:
    SVGameUIBase(SVInst*_app);

    ~SVGameUIBase();
    
    virtual void init(SVGameUIMgr* pUIMgr);
    
    virtual void destroy();
    
    virtual void enter();
    
    virtual void exit();
    
    virtual void update(float _dt);
    
    void setUIName(const char* pUiName);
    
protected:
    stringc m_strUIName;
    SVGameUIMgr* m_pUIMgr;
    SVGameUIShow* m_pUIShow;
};


#endif //SV_GAMEUIBASE_H
