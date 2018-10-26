//
//  SVPlotPoint.h
//  SVEngine
//
//  Created by 徐子昱 on 2018/10/25.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#ifndef SV_PLOTPOINT_H
#define SV_PLOTPOINT_H
#include "../base/SVGBase.h"

namespace sv {
    
    namespace util{
        
        class SVPlotPoint :public SVGBase{
        public:
            SVPlotPoint(SVInst *_app);
            
            ~SVPlotPoint();
            
            void init();
            
            void update(f32 dt);
            
            void render();
            
        };
        
    }//!namespace util
    
}//!namespace sv

#endif /* SV_PLOTPOINT_H */
