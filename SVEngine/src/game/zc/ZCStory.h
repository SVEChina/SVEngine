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
        //
        SVString m_player;
        s32 m_music;
        s32 m_bg;
        s32 m_effect;
    };

    //带选择的内容
    class ZCContextSelect : public ZCContext {
    public:
        ZCContextSelect();
       
        ~ZCContextSelect();
        
        SVString m_context1;
        SVString m_context2;
        SVString m_context3;
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
        void setRoleTbl(cptr8 _roletbl);
        //
        void setName(cptr8 _name);
        
        cptr8 getName();
        
        void setID(s32 _id);
        
        s32 getID();
        
        //加载故事表
        virtual void load();
        
        void loadChapter(s32 _index);
        //
        bool activeChapter(s32 _index);
        
        //跳转到目标章节的，目标行数
        void jump(s32 _index,s32 _code);
        
        //执行下一句
        void nxt();

    protected:
        //故事id
        s32 m_id;
        //故事名称
        SVString m_name;
        //作者
        SVString m_author;
        //目录
        SVString m_list;
        //角色表
        SVString m_roletbl;
        //当前章节
        ZCChapterPtr m_curChapter;
        //章节集合
        SVArray<ZCChapterPtr> m_chapters;
        //
        SVTable m_tbl;
        //角色
        ZCRoleMgrPtr m_pRoleMgr;
    };


}   //!namespace zc
    
}   //!namespace sv

#endif /* SV_ZC_STORY_H */
