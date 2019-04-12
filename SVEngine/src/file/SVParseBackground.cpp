//
//  SVParseBackground.cpp
//  SVEngine
//
//  Created by 徐子昱 on 2018/10/29.
//  Copyright © 2018年 李晓帆. All rights reserved.
//

#include "SVParseBackground.h"
#include "../node/SVBackGroundNode.h"
#include "../core/SVDeformImageMove.h"

SVNodePtr SVParseBackground::parseDeform(SVInst *app, RAPIDJSON_NAMESPACE::Value &item, s32 _resid, cptr8 _path){
    SVBackGroundNodePtr t_node=MakeSharedPtr<SVBackGroundNode>(app);
    t_node = MakeSharedPtr<SVBackGroundNode>(app);
    t_node->setcanSelect(false);
    t_node->setRSType(RST_SKY);
    t_node->setZOrder(0);
    t_node->setTexture(E_TEX_MAIN);
    t_node->fromJSON(item);
    t_node->getDeform()->setFlip(true);
    t_node->getDeform()->setIsDetect(true);
    return t_node;
}
