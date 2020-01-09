//
//  ZCRoleMgr.hpp
//

#ifndef SV_ZC_ROLEMGR_H
#define SV_ZC_ROLEMGR_H

#include "../../base/SVGBase.h"
#include "../../base/SVTable.h"
#include "ZCDef.h"

namespace sv {
    
namespace zc{
    
    class ZCRoleMgr: public SVGBase {
    public:
        ZCRoleMgr(SVInst *_app);
        
        ~ZCRoleMgr();
        
        //加载角色表
        void load(cptr8 _name) ;
        
    protected:
        SVTable m_tbl;
        SVArray<ZCRolePtr> m_roleVec;
    };

}   //!namespace zc
        
}   //!namespace sv

#endif /* SV_ZC_ROLEMGR_H */
