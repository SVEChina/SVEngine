//
// SVParseMain.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PARSEMAIN_H
#define SV_PARSEMAIN_H

#include "../base/SVGBase.h"
#include "SVParseDef.h"
#include "../base/SVMap.h"

namespace sv {
    
    namespace logic{
        
        class SVParseMain : public SVGBase {
        public:
            SVParseMain(SVInst *_app);
            
            virtual ~SVParseMain();
            
            SVModuleBasePtr parse(cptr8 _path, s32 resid);

        protected:
            void _registDictionary(cptr8 _type, prase_content_fun fun);
            
            void _unregistDictionary(cptr8 _type);
            
            bool _hasRegist(cptr8 _type );
            
            SVNodePtr _callTypeParse(cptr8 _type, RAPIDJSON_NAMESPACE::Value &item, s32 resid, cptr8 _path);
            
        protected:
            typedef SVMap<SVString, prase_content_fun> PARSEPOOL;
            PARSEPOOL parse_dictionary;
            SVInst *m_app;
        };

        
    }//!namespace logic
    
}//!namespace sv



#endif //SV_PARSEMAIN_H
