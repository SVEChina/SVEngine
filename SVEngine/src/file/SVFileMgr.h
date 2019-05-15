//
// SVFileMgr.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_FILEMGR_H
#define SV_FILEMGR_H

#include "../basesys/SVSysBase.h"
#include "../base/SVDataChunk.h"
//文件路径管理器 增加搜索路径相关
namespace sv {
    
    class SVFileMgr : public SVSysBase {
    public:
        SVFileMgr(SVInst *_app);
        
        ~SVFileMgr();
        
        //获取搜索路径数目
        s32 getSearchPathNum();
        
        //设置c++搜索路径
        void addRespath(cptr8 _path);
        
        void delRespath(cptr8 _path);
        
        void clearRespath();
        
        //获取文件全名 包含路径
        SVString getFileFullName(cptr8 _fname);
        
        bool loadFileContent(SVDataChunk *_datachunk, cptr8 _fname);
        
        bool loadFileContentStr(SVDataChunk *_datachunk, cptr8 _fname);
        
        bool saveFileData(SVDataSwapPtr _data, cptr8 _fname);
        
    protected:
        typedef SVArray<SVString> SEARCHPATHPOOL;
        SEARCHPATHPOOL m_searchPathPool;
        
        SVLockPtr m_fileLock;
        
        bool _hasRespath(cptr8 _path);
    };
    
}//!namespace sv



#endif //SV_FILEMGR_H
