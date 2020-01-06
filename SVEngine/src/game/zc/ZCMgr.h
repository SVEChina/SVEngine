//
//  SVLTMgr.hpp
//

#ifndef SV_ZC_MGR_H
#define SV_ZC_MGR_H

#include "../../module/SVModuleBase.h"

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
        
    };


}   //!namespace zc
    
}   //!namespace sv

#endif /* SV_ZC_MGR_H */
