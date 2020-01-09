//
//  ZCStory.hpp
//

#ifndef SV_ZC_STORY_JC_H
#define SV_ZC_STORY_JC_H

#include "ZCStory.h"
#include "ZCDef.h"

namespace sv {
    
namespace zc{


    //jcjh故事类型

    class ZCStoryJC : public ZCStory {
    public:
        ZCStoryJC(SVInst *_app);
        
        ~ZCStoryJC();
        
        void load();
        
    private:
        //组织
        ZCOrgMgrPtr m_pOrgMgr;
        //角色
        ZCRoleMgrPtr m_pRoleMgr;
    };


}   //!namespace zc
    
}   //!namespace sv

#endif /* SV_ZC_STORY_JC_H */
