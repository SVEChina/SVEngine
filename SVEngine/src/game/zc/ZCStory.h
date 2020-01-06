//
//  ZCStory.hpp
//

#ifndef SV_ZC_STORY_H
#define SV_ZC_STORY_H

#include "../../base/SVGBase.h"
#include "ZCDef.h"
#include "../../base/SVArray.h"

namespace sv {
    
namespace zc{

    //内容
    class ZCContext : public SVObject {
    public:
        ZCContext();
       
        ~ZCContext();
        
        //内容
        SVString m_context;
        
        //内容id
        s32 m_id;
    };

    //带选择的内容
    class ZCContextSelect : public ZCContext {
    public:
        ZCContextSelect();
       
        ~ZCContextSelect();
        
        SVArray<SVString> m_contexts;   //内容列表;
        SVArray<s32>    m_jumpID;       //跳转内容的id；
    };


    //章节
    class ZCChapter : public SVGBase {
    public:
        ZCChapter(SVInst *_app);
       
        ~ZCChapter();
        
        //解锁
        void unlock();
        
        //是否锁定
        bool isLock();
        
    protected:
        //锁定
        bool m_lock;
        //id
        s8 m_id;
    };


    //故事
    class ZCStory : public SVGBase {
    public:
        ZCStory(SVInst *_app);
        
        ~ZCStory();
        
        //加载故事表
        void load(cptr8 _tbl);
        
        //跳转到目标章节的，目标行数
        void jump(s32 _chapter,s32 _code);
        
        //执行下一句
        void nxt();
        
    protected:
        ZCChapterPtr m_curChapter;
    };


}   //!namespace zc
    
}   //!namespace sv

#endif /* SV_ZC_STORY_H */
