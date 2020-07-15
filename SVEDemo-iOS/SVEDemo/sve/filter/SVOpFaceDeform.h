//
//  SVOpFaceDeform.hpp
//  SVEngineShow
//
//  Created by 徐子昱 on 2018/10/31.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#ifndef SV_OPFACEDEFORM_H
#define SV_OPFACEDEFORM_H

#include "operate/SVOpBase.h"
#include "SVFilterExtDef.h"
#include "base/SVMap.h"
#include "core/SVVertDef.h"

namespace sv {
    class SVOpFaceDeform : public SVOpBase {
    public:
        SVOpFaceDeform(SVInst* _app,SVFaceDeformPtr _deform);
        ~SVOpFaceDeform();
    protected:
        void _process(f32 _dt);
        SVFaceDeformPtr m_deform;
    };
    
    class SVOpFaceDeformSmooth : public SVOpBase {
    public:
        SVOpFaceDeformSmooth(SVInst* _app,SVFaceDeformPtr _deform, SVMap<u32,V2> _point,SVMap<u32,V2> _area);
        ~SVOpFaceDeformSmooth();
    protected:
        void _process(f32 _dt);
        SVMap<u32,V2> m_point;
        SVMap<u32,V2> m_area;
        SVFaceDeformPtr m_deform;
    };
    
    class SVOpFaceDeformEye : public SVOpBase {
    public:
        SVOpFaceDeformEye(SVInst* _app,SVFaceDeformPtr _deform,f32 _smooth);
        ~SVOpFaceDeformEye();
    protected:
        void _process(f32 _dt);
        f32 m_smooth;
        SVFaceDeformPtr m_deform;
    };
    
    class SVOpDeformSmooth : public SVOpBase {
    public:
        SVOpDeformSmooth(SVInst* _app,SVFaceDeformPtr _deform,f32 _smooth);
        ~SVOpDeformSmooth();
    protected:
        void _process(f32 _dt);
        f32 m_smooth;
        SVFaceDeformPtr m_deform;
    };
    
    class SVOpFaceDeformParss : public SVOpBase {
    public:
        SVOpFaceDeformParss(SVInst* _app,SVFaceDeformPtr _deform,cptr8 path);
        ~SVOpFaceDeformParss();
    protected:
        void _process(f32 _dt);
        SVString m_path;
        SVFaceDeformPtr m_deform;
    };
}

#endif /* SV_OPFACEDEFORM_H */
