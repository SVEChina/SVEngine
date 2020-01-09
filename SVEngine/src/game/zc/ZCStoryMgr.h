//
//  ZCOrgMgr.hpp
//

#ifndef SV_ZC_STORYMGR_H
#define SV_ZC_STORYMGR_H

#include "../../base/SVGBase.h"
#include "../../base/SVTable.h"
#include "ZCDef.h"

namespace sv {
    
namespace zc{
    
    
    //组织相关

    class ZCStoryMgr: public SVGBase {
    public:
        ZCStoryMgr(SVInst *_app);
        
        ~ZCStoryMgr();
        
        //加载组织表
        void load(cptr8 _name) ;
        
    protected:
        SVTable m_tbl;
        //组织
        ZCOrgMgrPtr m_pOrgMgr;
        //故事
        ZCStoryPtr m_pStory;
        //角色
        ZCRoleMgrPtr m_pRoleMgr;
    };

}   //!namespace zc
        
}   //!namespace sv

#endif /* SV_ZC_STORYMGR_H */
