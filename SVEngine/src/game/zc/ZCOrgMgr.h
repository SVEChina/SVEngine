//
//  ZCOrgMgr.hpp
//

#ifndef SV_ZC_ORGMGR_H
#define SV_ZC_ORGMGR_H

#include "../../base/SVGBase.h"
#include "../../base/SVTable.h"

namespace sv {
    
namespace zc{
    
    //组织
    class ZCOrg: public SVGBase {
    public:
        ZCOrg(SVInst *_app);
        
        ~ZCOrg();
        
        SVString m_name;    //组织名称
        SVString m_dsp;     //组织描述
        bool m_islock;
    };
    
    //组织相关

    class ZCOrgMgr: public SVGBase {
    public:
        ZCOrgMgr(SVInst *_app);
        
        ~ZCOrgMgr();
        
        //加载组织表
        void load(cptr8 _name) ;
        
    protected:
        SVTable m_tbl;
    };

}   //!namespace zc
        
}   //!namespace sv

#endif /* SV_ZC_ORGMGR_H */
