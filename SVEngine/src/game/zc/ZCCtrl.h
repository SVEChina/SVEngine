//
//  ZCCtrl.hpp
//

#ifndef SV_ZC_CTRL_H
#define SV_ZC_CTRL_H

#include "../../base/SVGBase.h"

namespace sv {
    
namespace zc{
    
    class ZCCtrl: public SVGBase {
    public:
        ZCCtrl(SVInst *_app);
        
        ~ZCCtrl();
        
        //播放音乐
        void playMusic(s32 _code);
        
        //播放特效
        void playEffect(s32 _effect);
        
        //播放背景
        void playBG(s32 _bg);
        
    };

}   //!namespace zc
        
}   //!namespace sv

#endif /* SV_ZC_CTRL_H */
