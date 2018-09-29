//
// SVPipleGrid.h
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//

#ifndef SV_PIPLEGRID_MGR_H
#define SV_PIPLEGRID_MGR_H

#include "../event/SVEventProc.h"
#include "../base/SVVector2d.h"

//游戏独立个体
namespace sv{
    class SVPipleGrid;
    
    class SVPipleGridSet;
    
    class SVPipleMain;
    
    class SVPipleGridMgr : public SVEventProc {
    public:
        SVPipleGridMgr(SVInst*_app,SVPipleMain* _gamemain);
        
        ~SVPipleGridMgr();
        
        virtual void init();
        
        virtual void destroy();
        
        virtual void update(f32 _dt,f32 _gametime);
        
        virtual bool procEvent(SVEvent *_event);
        //生成网络
        void genNet(s32 _w,s32 _h);
        //检测网的合理性
        void checkNet();
        //检测闭环(目标格子,从哪个格子联通过来的,闭环集合)
        bool checkClosed(SVPipleGrid* _srcGrid,SVPipleGrid* _fromGrid,SVPipleGridSet* _gridSet);
        //获取格子
        SVPipleGrid* getGrid(vector2di& _pos);
        //
        s32 roollingX(s32 _value);
        //
        s32 roollingY(s32 _value);
        //
        void setSpineRes(cptr8 _wave,cptr8 _line);
        
    protected:
        void _search(f32 _x,f32 _y);
        
        void _dead(SVPipleGridSet* _gridSet);
        
        SVPipleMain* m_pPipleMain;
        
        s32 m_netWidth;
        
        s32 m_netHeight;
        
        typedef array<SVPipleGrid*> GRIDPOOL;
        GRIDPOOL m_gridPool;
        
        stringc m_spineWave;
        
        stringc m_spineLine;
    };
}



#endif //SV_PIPLEGRID_MGR_H
