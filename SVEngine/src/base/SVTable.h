//
// SVTable.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef __SV_TABLE_H__
#define __SV_TABLE_H__

#include "../base/SVGBase.h"
#include "svstr.h"
#include "SVArray.h"
#include "SVMap.h"

/*
    表
 */
namespace sv {
    
    namespace util {
        
        class SVTable{
        public:
            SVTable();

            ~SVTable();
            
            void reset();
            
            void setHead(SVStringArray<>& _head);
            
            void pushCxt(s32 _key,SVStringArray<>& _value);
            
            void removeCxt(s32 _key);
            
            void clearCxt();
            
            //获取内容
            SVString getCtx(s32 _key,cptr8 _name);
            
            f32 getCtxF(s32 _key,cptr8 _name);
            
            s32 getCtxI(s32 _key,cptr8 _name);
            
        private:
            bool _hasHead;
            SVArray<SVString> m_tblHead;  //表头
            SVMap<s32,SVArray<SVString>> m_tblCtx;  //内容
        };
        
        
    }//!namespace util
    
}//!namespace sv

#endif /* __SV_TABLE_H__ */
