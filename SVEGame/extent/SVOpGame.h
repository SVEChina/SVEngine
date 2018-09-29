//
// Created by 付一洲 on 2017/5/5.
//

#ifndef SV_OPERATEGAME_H
#define SV_OPERATEGAME_H

#include "SVOpBase.h"

namespace sv {
    
    class SVOpLoadGame : public SVOpBase {
    public:
        SVOpLoadGame(SVInst *_app,cptr8 _packname,cb_func_game _cb);
        cb_func_game m_pCB;
        stringc m_packName;
    protected:
        void _process(f32 dt);
    };
    
    //
    class SVOpStartGame : public SVOpBase {
    public:
        SVOpStartGame(SVInst *_app);
        
    protected:
        void _process(f32 dt);
    };
    
    class SVOpCloseGame : public SVOpBase {
    public:
        SVOpCloseGame(SVInst *_app);
        
    protected:
        void _process(f32 dt);
    };
    
    class SVOpSetGameData : public SVOpBase {
    public:
        SVOpSetGameData(SVInst *_app,cptr8 _info);
        stringc m_info;
    protected:
        void _process(f32 dt);
    };
    
    class SVOpSetGameHead : public SVOpBase {
    public:
        SVOpSetGameHead(SVInst *_app,s32 _playercode,s32 _w,s32 _h,void* _data);
        ~SVOpSetGameHead();
        
        s32 m_playercode;
        s32 m_width;
        s32 m_height;
        void* m_pData;
        
    protected:
        void _process(f32 dt);
    };
    
}//!namespace sv





#endif //SV_OPERATEGAME_H
