//
// SVPictureProcess.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_PICPROC_H
#define SV_PICPROC_H

#include "SVProcess.h"
#include "filter/SVFilterDef.h"
#include "../mtl/SVMtlDeclare.h"
#include "../node/SVNodeDeclare.h"

namespace sv {

    namespace logic {
        
        //图片处理
        class SVPictureProcess : public SVProcess {
        public:
            SVPictureProcess(SVInst *_app);
            
            ~SVPictureProcess();
            //
            void init();
            
            void update(f32 _dt);
            //
            void addFilter(SVFilterBasePtr _filter);
  
            void openFilter(SVFilterBasePtr _filter);
            
            void clearFilter(SVFilterBasePtr _filter);
            
            void clearFilter(SVString t_name);
            
            void clearFilter(SVFILTERFUNCTYPE t_type);
            //
            void destroy();
            
            void setFilterParam(f32 _smooth,SVFILTERITEMTYPE _type);
            
            f32  getFilterParam(SVFILTERITEMTYPE _type);
            
            SVFilterBasePtr getFilter(SVString t_name);
        protected:
            typedef SVArray<SVFilterBasePtr> FilterArray;
            FilterArray m_filterArray;
        };
        
    }//!namespace logic

}//!namespace sv



#endif //SV_PICPROC_H
