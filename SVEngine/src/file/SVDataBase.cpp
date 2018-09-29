//
// SVDataBase.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVDataBase.h"

SVDataBase::SVDataBase(SVInst *_app)
: SVGBase(_app){
    m_dataType = MODELDATA_NONE;
}

SVDataBase::~SVDataBase(){
}

SVString SVDataBase::jsonFormat(const SVString inString){
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

void SVDataBase::toJSON(RAPIDJSON_NAMESPACE::Document::AllocatorType &_allocator, RAPIDJSON_NAMESPACE::Value &_objValue){
}

void SVDataBase::fromJSON(RAPIDJSON_NAMESPACE::Value &item){
}

