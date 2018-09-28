//
// SVSpineListen.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_SPINELISTEN_H
#define SV_SPINELISTEN_H

#include "../third/spine/include/spine/AnimationState.h"
#include "../third/spine/include/spine//Event.h"
#include <functional>

typedef std::function<void(int trackIndex)> spStartListener;
 
typedef std::function<void(int trackIndex)> spEndListener;
 
typedef std::function<void(int trackIndex, int loopCount)> spCompleteListener;
 
typedef std::function<void(int trackIndex, spEvent *event)> spEventListener;

typedef struct _TrackEntryListeners {
    spStartListener m_cb_startListener;
    spEndListener m_cb_endListener;
    spCompleteListener m_cb_completeListener;
    spEventListener m_cb_eventListener;
} _TrackEntryListeners;


void animationCallback(spAnimationState* state, spEventType type, spTrackEntry* entry, spEvent* event);

void trackEntryCallback(spAnimationState* state, spEventType type, spTrackEntry* entry, spEvent* event);

//void disposeTrackEntry(spTrackEntry *entry);

_TrackEntryListeners *getListeners(spTrackEntry *entry);

#endif //SV_SPINELISTEN_H
