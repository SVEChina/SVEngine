//
// SVEffectMusic.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_EFFECTMUSIC_H
#define SV_EFFECTMUSIC_H
#include "SVModuleBase.h"
#include "../base/SVMap.h"
namespace sv{
    class SVEffectMusic : public SVModuleBase {
    public:
        SVEffectMusic(SVInst* _app);
        
        ~SVEffectMusic();
        
        virtual void init();
        
        virtual void destroy();
        
        virtual void update(f32 _dt);
        
        virtual void open();
        
        virtual bool procEvent(SVEventPtr _event);
        
        virtual void fromJSON(RAPIDJSON_NAMESPACE::Value &_item);
        
        void reset();
        
        void setRootPath(cptr8 _path);
    protected:
        void _addMusic(cptr8 _key, cptr8 _musicPath);
        void _clearAllMusic();
    protected:
        SVLockPtr m_lock;
        typedef SVMap<SVString,SVString> MUSICMAP;
        MUSICMAP m_musicMap;
        SVString m_rootPath;
    };
    
    
}//!namespace sv

#endif /* SV_EFFECTMUSIC_H */
