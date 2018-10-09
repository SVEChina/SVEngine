//
//  SVTest.m
//  SVESample2
//
//  Created by 付一洲 on 2018/10/9.
//  Copyright © 2018年 付一洲. All rights reserved.
//

#import "SVTest.h"
#import "SDLogicSys.h"
#include "src/basesys/SVSceneMgr.h"
#include "src/node/SVScene.h"
#include "src/node/SVSpriteNode.h"
#include "src/mtl/SVTexMgr.h"

@implementation SVTest

-(void)testSprite {
    SVInst* pSVE = [[SDLogicSys getInst] getSVE];
    if( pSVE ) {
        SVScenePtr t_sc = pSVE->getSceneMgr()->getScene();
        if(t_sc) {
            SVSpriteNodePtr t_sp = MakeSharedPtr<SVSpriteNode>(pSVE,300,400);
            t_sp->setTexture(pSVE->getTexMgr()->getSVETexture());
            t_sc->addNode(t_sp);
        }
    }
}

@end
