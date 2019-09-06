//
// SVParseData.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PARSEDATA_H
#define SV_PARSEDATA_H

#include "../base/SVGBase.h"
#include "../node/SVNodeDef.h"

namespace sv {
    
    namespace logic{
        
        class SVParseData : public SVGBase {
        public:
            SVParseData(SVInst *_app);
            
            ~SVParseData();
            
        public:
            SVString version;
            SVString name;
            SVString type;
            SVString subType;
            SVString resourceType;
            s32 personid;
            s32 loop;
            
            virtual void generate(s32 resid);
            
            virtual void generate(s32 resid, cptr8 path);
            
            void suspend();
            
        protected:
            void _addNode(SVNodePtr _node);
            
            void _clearNode();
            
            SVArray<SVNodePtr> m_Nodes;
        };
        
    }//!namespace logic
    
}//!namespace sv



//
#endif //SV_PARSEDATA_H
