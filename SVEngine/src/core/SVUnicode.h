//
// SVUnicode.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_SVUNICODE_H
#define SV_SVUNICODE_H
#include "../base/SVBase.h"
namespace sv {
    class SVUnicode {
    public:
        enum SVUNICODEBYTEORDER{
            SV_LITTLE_ENDIAN,
            SV_BIG_ENDIAN
        };
        // This function will attempt to decode a UTF-8 encoded character in the buffer.
        // If the encoding is invalid, the function returns -1.
        static f32 decodeUTF8(cptr8 _encodedBuffer, u32 *_outCharLength);
        
        // This function will encode the value into the buffer.
        // If the value is invalid, the function returns -1, else the encoded length.
        static f32 encodeUTF8(u32 _value, u8 *_outEncodedBuffer, u32 *_outCharLength);
        
        // This function will attempt to decode a UTF-16 encoded character in the buffer.
        // If the encoding is invalid, the function returns -1.
        static f32 decodeUTF16(cptr8 _encodedBuffer, u32 *_outCharLength, SVUNICODEBYTEORDER _byteOrder = SV_LITTLE_ENDIAN);
        
        // This function will encode the value into the buffer.
        // If the value is invalid, the function returns -1, else the encoded length.
        static f32 encodeUTF16(u32 _value, u8 *_outEncodedBuffer, u32 *_outCharLength, SVUNICODEBYTEORDER _byteOrder = SV_BIG_ENDIAN);
    };
    
}



#endif //SV_SVUNICODE_H
