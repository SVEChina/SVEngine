//
// Created by 徐子昱 on 2018/10/15.
//

#ifndef SVEDEMO_ANDROID_SVWATERMARK_H
#define SVEDEMO_ANDROID_SVWATERMARK_H


#include <mtl/SVMtlDeclare.h>
#include "base/SVGBase.h"
#include "node/SVNodeDeclare.h"
class SVWatermark : public SVGBase {
public:
    SVWatermark(SVInst *_app);
    ~SVWatermark();
    void createNode(void* _rgba,int _width,int _height);
    void clearNode();
protected:
    SVSpriteNodePtr m_sprite;
    SVTexturePtr m_tex;
};


#endif //SVEDEMO_ANDROID_SVWATERMARK_H
