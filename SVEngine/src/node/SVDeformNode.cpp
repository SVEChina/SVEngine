//
//  SVDeformNode.cpp
//  SVEngine
//
//  Created by 徐子昱 on 2019/4/13.
//  Copyright © 2019 李晓帆. All rights reserved.
//

#include "SVDeformNode.h"
#include "../core/SVDeformImageMove.h"



SVDeformNode::SVDeformNode(SVInst *_app)
:SVNode(_app){
    ntype = "SVDeformNode";
    
}

SVDeformNode::~SVDeformNode(){
    
}

void SVDeformNode::init(){

}

void SVDeformNode::destroy(){
    m_deformPool.clear();
}

void SVDeformNode::update(f32 _dt){
    SVNode::update(_dt);
    for(s32 i=0;i<m_deformPool.size();i++){
        m_deformPool[i]->update(_dt);
    }
}

void SVDeformNode::render(){
    for(s32 i=0;i<m_deformPool.size();i++){
        m_deformPool[i]->render();
    }
    m_deformPool.clear();
}

void SVDeformNode::pushDeform(SVArray<SVDeformImageMovePtr> _deformArray){
    m_deformPool.append(_deformArray);
}

bool SVDeformNode::procEvent(SVEventPtr _event){
 
    return true;
}
