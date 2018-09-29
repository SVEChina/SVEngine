//
// Created by yinlong on 17/10/28.
//

#ifndef SV_GAMEUISHOW_H
#define SV_GAMEUISHOW_H


#include "../base/SVGBase.h"

class SVNode;
class SVUIParamBase;
class SVGameUIShow : public SVGBase {
public:
    SVGameUIShow(SVInst*_app);

    ~SVGameUIShow();
   
    virtual void init(const char* _strName);
    
    virtual void destroy();
    
    virtual void enter();
    
    virtual void exit();
    
    virtual void update(f32 _dt);
    
    void createUI(SVUIParamBase* _param);
    
    SVNode* getUIWidgetByName(const char* _strName);

    const char* getName();
private:
    typedef map<stringc , SVNode*> UIELEMENTMAP;
    UIELEMENTMAP m_mapUIShow;

    stringc m_strName;
};

#endif //SV_GAMEUISHOW_H
