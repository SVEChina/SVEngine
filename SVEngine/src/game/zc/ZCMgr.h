//
//  SVLTMgr.hpp
//

#ifndef SV_ZC_MGR_H
#define SV_ZC_MGR_H

#include "../../module/SVModuleBase.h"
#include "ZCDef.h"

namespace sv {
    
namespace zc{

    class ZCMgr : public SVModuleBase {
    public:
        ZCMgr(SVInst *_app);
        
        ~ZCMgr();
        
        virtual void init();
        
        virtual void destroy();
        
        virtual void update(f32 _dt);
        
        virtual void reset();
        

    protected:
        //组织
        ZCOrgMgrPtr m_pOrgMgr;
        //故事
        ZCStoryPtr m_pStory;
        //角色
        ZCRoleMgrPtr m_pRoleMgr;
        //控制
        ZCCtrlPtr m_pCtrl;
    };


}   //!namespace zc
    
}   //!namespace sv

#endif /* SV_ZC_MGR_H */
