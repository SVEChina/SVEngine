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
        
        class SVTableCell {
        public:
            SVTableCell();
            
            SVTableCell &operator=(const SVTableCell &s);
            
            void setValue(cptr8 _value);
            
            void setValue(f32 _value);
            
            void setValue(s32 _value);
            
            cptr8 getStrValue(){ return m_str_value.c_str(); }
            
            s32 getSValue() { return m_s_value; }
            
            f32 getFValue() { return m_f_value; }
            
            SVString m_str_value;
            f32 m_f_value;
            s32 m_s_value;
        };
    
        //
        class SVTableLine {
        public:
            SVTableLine();

            ~SVTableLine();
            
            SVTableLine &operator=(const SVTableLine &s);
            
            cptr8 getStrValue(s32 _index);
            
            SVArray<SVTableCell> m_cells;  //表头
        };
    
        //
        class SVTable{
        public:
            SVTable();

            ~SVTable();
            
            void reset();
            
            void setHead(SVStringArray<> _head);
            
            void pushCxt(SVStringArray<> _value);
            
            void removeCxt(s32 _key);
            
            void clearCxt();
            
            //获取内容
            s32 getCtxNum();
            
            SVString getCtx(s32 _index,cptr8 _name);
            
            f32 getCtxF(s32 _index,cptr8 _name);
            
            s32 getCtxI(s32 _index,cptr8 _name);
            
        private:
            bool _hasHead;
            SVArray<SVString> m_tblHead;  //表头
            SVArray<SVTableLine> m_tblCtx;  //内容
        };
        
        
    }//!namespace util
    
}//!namespace sv

#endif /* __SV_TABLE_H__ */
