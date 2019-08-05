//
// SVJsonTool.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVJsonTool.h"

SVJsonTool::SVJsonTool(SVInst *_app)
: SVGBase(_app){
 
}

SVJsonTool::~SVJsonTool(){
}

SVString SVJsonTool::jsonFormat(const SVString inString){
    SVString outString;
    outString.allocate(inString.size());
    int index = 0;
    int count = 0;
    while(index < inString.size()){
        char ch = inString.get(index);
        if(ch == '{' || ch == '['){
            outString.append(ch);
            outString.append('\n');
            count++;
            for (int i = 0; i < count; i++) {
                outString.append('\t');
            }
        }
        else if(ch == '}' || ch == ']'){
            outString.append('\n');
            count--;
            for (int i = 0; i < count; i++) {
                outString.append('\t');
            }
            outString.append(ch);
        }
        else if(ch == ','){
            outString.append(ch);
            outString.append('\n');
            for (int i = 0; i < count; i++) {
                outString.append('\t');
            }
        }
        else {
            outString.append(ch);
        }
        index ++;
    }
    return outString;
}
