//
//  SVPythonMgr.hpp
//  SVEngine
//
//  Created by 付一洲 on 2019/2/28.
//  Copyright © 2019 李晓帆. All rights reserved.
//

#ifndef SV_PYTHONSYS_H
#define SV_PYTHONSYS_H

#include "../SVSysBase.h"
//#include "python/Python.h"

namespace sv{
    
    class SVPythonSys : public SVSysBase {
    public:
        SVPythonSys(SVInst* _app);
        
        ~SVPythonSys();
        
        void init();
        
        void destroy();
        
        void update(f32 dt);
        
        void output();
        
    protected:
        

    };
}//!namespace sv

#endif /* SV_PYTHONSYS_H */
