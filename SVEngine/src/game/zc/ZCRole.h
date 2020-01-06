//
//  ZCRole.hpp
//

#ifndef SV_ZC_ROLE_H
#define SV_ZC_ROLE_H

#include "../../base/SVGBase.h"
#include "ZCDef.h"

namespace sv {
    
namespace zc{
    
    //角色

    class ZCRole: public SVGBase {
    public:
        ZCRole(SVInst *_app);
        
        ~ZCRole();
        
        void unlock();
        
        bool islock();
        
    private:
        //境界
        ZCLEVEL m_level;
        //功法
        
        //友好度
        s32 m_friend;
        
        bool m_isLock;  //是否解锁
        
    };

}//!namespace zc

}//!namespace sv

#endif /* SV_ZC_ROLE_H */
