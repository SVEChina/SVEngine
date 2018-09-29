//
// Created by lidm on 17/11/15.
//

#include "SVGameGetMapLocUtil.h"

void SVGameGetMapLocUtil::getLocForRandom(int _tarWidth, int _tarHeight, int _size, array<V2> *_resultArray) {
    for (s32 i = 0; i < _size; i++) {
        V2 t_v2;
        t_v2.x = rand() % _tarWidth;
        //范围在80 - 1200之间随机
        t_v2.y = 1200 - rand() % _tarHeight - 80;
        _resultArray->insert(t_v2);
    }
}

