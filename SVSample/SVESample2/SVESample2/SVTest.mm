//
//  SVTest.m
//  SVESample2
//
//  Created by 付一洲 on 2018/10/9.
//  Copyright © 2018年 付一洲. All rights reserved.
//

#import "SVTest.h"
#import "SDLogicSys.h"
#include "basesys/SVSceneMgr.h"
#include "node/SVScene.h"
#include "node/SVSpriteNode.h"
#include "mtl/SVTexMgr.h"

@implementation SVTest

-(void)testSprite {
    SVInst* pSVE = [[SDLogicSys getInst] getSVE];
    if( pSVE ) {
        SVScenePtr t_sc = pSVE->getSceneMgr()->getScene();
        if(t_sc) {
            SVSpriteNodePtr t_sp = MakeSharedPtr<SVSpriteNode>(pSVE,400,600);
            t_sp->setTexture(pSVE->getTexMgr()->getSVETexture());
            t_sc->addNode(t_sp);
            //
            t_sp->setScale(0.2, 0.5, 1.0f);
            t_sp->setPosition(100, 100, -100);
            t_sp->setRotation(0.0f, 0.0f, 45.0f);
        }
    }
}

@end
