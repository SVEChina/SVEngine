//
//  ZCRoleMgr.hpp
//

#ifndef SV_ZC_ROLEMGR_H
#define SV_ZC_ROLEMGR_H

#include "../../base/SVGBase.h"

namespace sv {
    
namespace zc{
    
    class ZCRoleMgr: public SVGBase {
    public:
        ZCRoleMgr(SVInst *_app);
        
        ~ZCRoleMgr();
        
        //加载角色表
        void load(cptr8 _tbl) ;
        
    protected:
        
    };

}   //!namespace zc
        
}   //!namespace sv

#endif /* SV_ZC_ROLEMGR_H */
