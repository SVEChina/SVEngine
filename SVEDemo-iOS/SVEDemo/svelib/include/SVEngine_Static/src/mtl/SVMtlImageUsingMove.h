//
//  SVMtlImageUsingMove.hpp
//  SVEngine
//
//  Created by 徐子昱 on 2018/8/6.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#ifndef SV_MTLIMAGEUSINGMOVE_H
#define SV_MTLIMAGEUSINGMOVE_H

#include "SVMtlCore.h"
#include "../base/SVVec2.h"




namespace sv {
    
    namespace mtl{
        
        class SVMtlImageUsingMove : public SVMtlCore {
        public:
            SVMtlImageUsingMove(SVInst *_app);
            
            SVMtlImageUsingMove(SVMtlImageUsingMove *_mtl);
            
            ~SVMtlImageUsingMove();
            
            virtual SVMtlCorePtr clone();
            
            virtual void update(f32 dt);
            
        protected:
            void _submitMtl();
            void _bindUniform();
        };
        
    }//!namespace mtl
    
}//!namespace sv
#endif /* SV_MTLIMAGEUSINGMOVE_H */
