//
// Created by lidm on 17/11/15.
//

#ifndef SV_GAME_GETMAPLOCUTIL_H
#define SV_GAME_GETMAPLOCUTIL_H


#include "../core/SVVertDef.h"
#include "../base/SVGBase.h"
//#include "../GameFaceFlappy/MaFaceFlappyDef.h"

class SVGameGetMapLocUtil {

public:
    static void getLocForRandom(int _tarWidth, int _tarHeight, int _size, array<V2> *_resultArray);

};


#endif //SV_GAME_GETMAPLOCUTIL_H
