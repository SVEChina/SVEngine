//
// Created by yinlong on 17/10/28.
//

#ifndef SV_GAMEBUFF_H
#define SV_GAMEBUFF_H

#include "../base/SVGBase.h"

class SVBuffBase : public SVGBase {
public:
    SVBuffBase(SVInst*_app);

    ~SVBuffBase();
    
    virtual void enter();
    
    virtual void exit();
    
    virtual bool isEnd();

    virtual bool isTrig();
    
    virtual void proc();
    
    virtual void update(f32 _dt);
    
    s32  getBufId();
    
    void setBufType(s32 iType);
    
    s32  getBufType();
private:
    s32     m_iBufType;
    s32     m_iBufId;
    bool    m_bEnd;
};


class SVBuffMgr : public SVGBase{
public:
    SVBuffMgr(SVInst* _app);
    ~SVBuffMgr();
    
    void init();
    void destory();
    void update(f32 _dt);
    
    void addBuff(SVBuffBase* pBuf);
    
    SVBuffBase* getBuff(s32 iBufId);
private:
    typedef list<SVBuffBase*> BUFCONTAIN;
    BUFCONTAIN m_bufContainer;
};
#endif //SV_GAMEBUFF_H
