//
// SVStringHelper.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVStringHelper.h"

SVStringHelper::SVStringHelper()
: SVObject(){
 
}

SVStringHelper::~SVStringHelper(){
    
}

void SVStringHelper::encrypt(c8 *_str, s32 *_keys, u32 _keyCount){
    u32 len= (u32)strlen(_str);//获取长度
    for(u32 i=0;i<len;i++){
        s32 key = _keys[i%_keyCount]%9;
        *(_str+i) = *(_str+i) ^ key;
    }
}

void SVStringHelper::decrypt(c8 *_str, s32 *_keys, u32 _keyCount){
    u32 len= (u32)strlen(_str);//获取长度
    for(u32 i=0;i<len;i++){
        s32 key = _keys[i%_keyCount]%9;
        *(_str+i) = *(_str+i) ^ key;
    }
}

SVString SVStringHelper::jsonFormat(const SVString inString){
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
