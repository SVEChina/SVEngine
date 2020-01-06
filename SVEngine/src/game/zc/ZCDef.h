//
//  ZCDef.h
//  SVEShowGift
//
//  Created by 付一洲 on 2019/09/15.
//  Copyright © 2019 付一洲. All rights reserved.
//

#ifndef SV_ZC_DEF_H
#define SV_ZC_DEF_H

#include "../../base/SVPreDeclare.h"
#include "../../node/SVNodeDeclare.h"

namespace sv {
    
namespace zc {
        
DECLARE_SHAREPTR(SVLTMgr);
DECLARE_SHAREPTR(SVLTCtrl);

//内容剧情相关
DECLARE_SHAREPTR(ZCContext);
DECLARE_SHAREPTR(ZCContextSelect);
DECLARE_SHAREPTR(ZCChapter);
DECLARE_SHAREPTR(ZCStory);
//角色相关
DECLARE_SHAREPTR(ZCRole);
DECLARE_SHAREPTR(ZCRoleMgr);
//组织相关
DECLARE_SHAREPTR(ZCOrg);
DECLARE_SHAREPTR(ZCOrgMgr);



    
enum ZCLEVEL {
    E_LEVEL_BASE_1,     //初窥门径
    E_LEVEL_BASE_2,     //登堂入室
    E_LEVEL_BASE_3,     //融会贯通
    E_LEVEL_UP_1,       //尊者，提升段1
    E_LEVEL_UP_2,
    E_LEVEL_UP_3,
    E_LEVEL_SHI_1,      //师境
    E_LEVEL_SHI_2,
    E_LEVEL_SHI_3,
    E_LEVEL_DAO_1,      //道境
    E_LEVEL_DAO_2,
    E_LEVEL_DAO_3,
    E_LEVEL_YIN_1,      //隐境
    E_LEVEL_YIN_2,
    E_LEVEL_YIN_3,
    E_LEVEL_KING        //皇境
};

}   //!namespace zc
    
}   //!namespace sv

#endif /* SV_ZC_DEF_H */
