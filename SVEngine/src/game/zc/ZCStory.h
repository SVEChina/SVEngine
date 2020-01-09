//
//  ZCStory.hpp
//

#ifndef SV_ZC_STORY_H
#define SV_ZC_STORY_H

#include "../../base/SVGBase.h"
#include "ZCDef.h"
#include "../../base/SVArray.h"
#include "../../base/SVTable.h"

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
        
        void setID(s32 _id);
        
        void setName(cptr8 _name);
        
        void setRes(cptr8 _res);
        
        //解锁
        void unlock();
        
        //是否锁定
        bool isLock();
        
        void load();
        
        ZCContextPtr getCtx();
        
        ZCContextPtr nxt();
        
        cptr8 getName();
        
    protected:
        //锁定
        bool m_lock;
        //id
        s8 m_id;
        //章节的名字
        SVString m_name;
        //内容路径
        SVString m_ctxpath;
        //
        SVArray<ZCContextPtr> m_ctxs;   //内容
        //
        s32 m_curCtxCode;   //当前内容序号
        //
        f32 m_percent;      //进度
    };


    //故事
    class ZCStory : public SVGBase {
    public:
        ZCStory(SVInst *_app);
        
        ~ZCStory();
        
        void setAuthor(cptr8 _name);
        
        cptr8 getAuthor();
        
        void setList(cptr8 _name);
        
        //
        void setName(cptr8 _name);
        
        cptr8 getName();
        
        void setID(s32 _id);
        
        s32 getID();
        
        //加载故事表
        void load();
        
        //跳转到目标章节的，目标行数
        void jump(s32 _chapter,s32 _code);
        
        //执行下一句
        void nxt();

    protected:
        //故事id
        s32 m_id;
        //故事名称
        SVString m_name;
        //
        SVString m_author;
        //
        SVString m_list;
        //当前章节
        ZCChapterPtr m_curChapter;
        //章节集合
        SVArray<ZCChapterPtr> m_chapters;
        //
        SVTable m_tbl;
    };


}   //!namespace zc
    
}   //!namespace sv

#endif /* SV_ZC_STORY_H */
