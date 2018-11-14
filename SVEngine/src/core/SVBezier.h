//
//  SVBezier.h
//  SVEngine
//
//  Created by 徐子昱 on 2018/11/12.
//  Copyright © 2018 李晓帆. All rights reserved.
//

#ifndef SV_BEZIER_H
#define SV_BEZIER_H

#include "../base/SVGBase.h"
#include "../base/SVMap.h"
#include "../core/SVImageUsingMove.h"
#include "../mtl/SVTexture.h"
#include "../base/SVArray.h"

namespace sv {
    
    namespace util{
        
        //Bezier曲线绘制
        
        class SVBezier : public SVGBase {
        public:
            SVDeformImageMove(SVInst *_app);
            
            ~SVDeformImageMove();
            
            void init();
            
            void update(f32 dt);
            
            void render();
            
        };
        
    }//!namespace util
    
}//!namespace sv

#endif /* SV_BEZIER_H */
