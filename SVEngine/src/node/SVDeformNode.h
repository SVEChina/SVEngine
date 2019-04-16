//
//  SVDeformNode.h
//  SVEngine
//
//  Created by 徐子昱 on 2019/4/13.
//  Copyright © 2019 李晓帆. All rights reserved.
//

#ifndef SV_DEFORM_NODE_H
#define SV_DEFORM_NODE_H

#include "SVNode.h"

namespace sv {
    
    namespace node{
        
        class SVDeformNode : public SVNode {
            public :
            SVDeformNode(SVInst *_app);
            
            ~SVDeformNode();
            
            void init();
            
            void destroy();
            
            void update(f32 _dt);
            
            void render();
            
            bool procEvent(SVEventPtr _event);
            
            void pushDeform(SVArray<SVDeformImageMovePtr> _deformArray);
            
        protected:
            typedef SVArray<SVDeformImageMovePtr> DEFORMPOOL;
            DEFORMPOOL m_deformPool;
      
        };
    }
}

#endif /* SV_DEFORM_NODE_H */
