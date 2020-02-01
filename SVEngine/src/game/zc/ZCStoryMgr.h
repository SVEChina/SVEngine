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
        //
        void loadStory(cptr8 _name);
        
        bool activeStory(cptr8 _name);
        
        ZCStoryPtr getActiveStory();
        
        void getAllStory(SVArray<ZCStoryPtr> &_storyVec);
    protected:
        SVTable m_tbl;
        
        SVArray<ZCStoryPtr> m_storyVec;
        
        ZCStoryPtr m_activeStory;
    };

}   //!namespace zc
        
}   //!namespace sv

#endif /* SV_ZC_STORYMGR_H */
