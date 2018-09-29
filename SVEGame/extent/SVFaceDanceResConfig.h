//
// Created by lidm on 17/9/5.
//

#ifndef SV_FACEDANCE_RESCONFIG_H
#define SV_FACEDANCE_RESCONFIG_H

#include "../base/SVGBase.h"

//游戏配置
class SVFaceDanceResConfig : public SVGBase {
public:
    SVFaceDanceResConfig(SVInst*_app);

    ~SVFaceDanceResConfig();
    
    virtual void init();
    
    virtual void destroy();
};


#endif //SV_FACEDANCE_RESCONFIG_H
