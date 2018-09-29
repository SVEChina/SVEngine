//
// Created by yinlong on 17/10/28.
//

#ifndef SV_GAMEUIMGR_H
#define SV_GAMEUIMGR_H


#include "../base/SVGBase.h"
class SVGameUIShow;
class SVUIParamUI;
class SVGameUIMgr : public SVGBase{
public:
    SVGameUIMgr(SVInst* _app);
    ~SVGameUIMgr();
    
    void init();
    
    void destory();
    
    void update(float _dt);
    
    void addUI(SVGameUIShow* _pUI);
    
    void removeUI(const char* _strUIName);
    
    void clear();
    
    SVGameUIShow* getUI(const char* _strUIName);
    
    void create(SVUIParamUI* _pUIParam);
protected:
    typedef map<stringc , SVGameUIShow*> MAPUIMGR;
    MAPUIMGR m_mapUIMgr;
};
#endif //SV_GAMEUIMGR_H
