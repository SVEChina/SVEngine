//
//  SVMtlSmooth.h
//  SVEngine
//
//  Created by 徐子昱 on 2019/3/26.
//  Copyright © 2019 李晓帆. All rights reserved.
//

#ifndef SV_MTL_SMOOTH_H
#define SV_MTL_SMOOTH_H

#include "SVMtlCore.h"

namespace sv {
    
    namespace mtl{
        
        class SVMtlSmooth : public SVMtlCore {
        public:
            
            SVMtlSmooth(SVInst *_app , SVString _name);
            
            SVMtlSmooth(SVMtlSmooth *_mtl);
            
            ~SVMtlSmooth();
            
            virtual SVMtlCorePtr clone();
            
            
            inline void setSmooth(f32 _smooth){
                m_smooth = _smooth;
            }
            
            inline void setImgWH(f32 _w , f32 _h){
                m_w = _w;
                m_h = _h;
            }
        protected:
            virtual void _submitMtl(SVRendererPtr _render);
            
            f32 m_smooth;
            f32 m_w;
            f32 m_h;
        };
        
    }//!namespace mtl
    
}//!namespace sv


#endif /* SV_MTL_SMOOTH_H */
