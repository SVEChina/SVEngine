//
//  ZCStory.hpp
//

#ifndef SV_ZC_STORY_JC_H
#define SV_ZC_STORY_JC_H

#include "../../base/SVGBase.h"
#include "ZCDef.h"
#include "../../base/SVArray.h"
#include "../../base/SVTable.h"

namespace sv {
    
namespace zc{


    //jcjh故事类型

    class ZCStoryJC : public ZCStory {
    public:
        ZCStoryJC(SVInst *_app);
        
        ~ZCStoryJC();
        
    };


}   //!namespace zc
    
}   //!namespace sv

#endif /* SV_ZC_STORY_JC_H */
