//
//  SVFaceDeform.hpp
//  SVEngineShow
//
//  Created by 徐子昱 on 2018/10/31.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#ifndef SV_FACEDEFORM_H
#define SV_FACEDEFORM_H

#include "base/SVGBase.h"
#include "node/SVNodeDeclare.h"
#include "SVFaceDeform.h"

namespace sv{
    
    class SVFaceDeform : public SVGBase {
    public:
        SVFaceDeform(SVInst *_app);
        
        ~SVFaceDeform();
        
        SVNodePtr createDeform();
        
        SVDeformImageMovePtr getDeform();
    protected:
        SVBackGroundNodePtr m_node;
        SVDeformImageMovePtr m_deform;
    
    };

}

#endif /* SV_FACEDEFORM_H */
