//
// SVPipleGrid.h
// SVEngine
// Copyright 2017-2020
// 付一洲,尹龙,林龙飞,徐子昱,李晓帆,李达明
//

#ifndef SV_PIPLE_GRID_H
#define SV_PIPLE_GRID_H

#include "../event/SVEventProc.h"
#include "../base/SVVector3d.h"
#include "../base/SVVector2d.h"

//每一个格子 都是独立的位置 包含两个头

class SVSpineNode;
class SVBitFontNode;

namespace sv {
    enum E_PIPLEGRID_TYPE{
        E_PIP_GDT_LINE,
        E_PIP_GDT_WAVE
    };
    
    enum E_PIPLEGRID_STATE{
        E_PIP_GDS_0,
        E_PIP_GDS_1,
        E_PIP_GDS_2,
        E_PIP_GDS_3,
        E_PIP_GDS_NUM = 4
    };
    
    class SVPipleGridMgr;
    
    class SVPipleGrid : public SVEventProc {
    public:
        SVPipleGrid(SVInst*_app,SVPipleGridMgr* _gridmgr);
        
        ~SVPipleGrid();
        
        virtual void init(cptr8 _spinename);
        
        virtual void destroy();
        
        void enter();
        
        void exit();
        
        void gen(E_PIPLEGRID_TYPE _type,E_PIPLEGRID_STATE _state,s32 _px,s32 _py);
        
        void locate(s32 _px,s32 _py);
        
        bool isLink(SVPipleGrid* _grid);
        
        void trans();
        
        void setLight(bool _light);
        
        virtual void update(f32 _dt,f32 _gametime);
        
        virtual bool procEvent(SVEvent *_event);
        
    public:
        vector2di m_local;
        vector2di m_side0;
        vector2di m_side1;
        
    protected:
        SVPipleGridMgr* m_pGridMgr;
        
        E_PIPLEGRID_TYPE m_gridType;
        
        E_PIPLEGRID_STATE m_gridState;
        
        SVSpineNode* m_pAni;
        
        bool m_light;
    };
    
    //
    class SVPipleGridSet{
    public:
        SVPipleGridSet();
        
        ~SVPipleGridSet();
        
        bool isClose(SVPipleGrid* _grid);
        
        void pushGrid(SVPipleGrid* _grid);
        
    protected:
        array<SVPipleGrid*> m_grids;
    };
}



#endif //SV_QCLICK_UNIT_H
