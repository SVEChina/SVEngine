//
// SVUnicode.cpp
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#include "SVUnicode.h"

f32 SVUnicode::decodeUTF8(cptr8 _encodedBuffer, u32 *_outCharLength){
    cptr8 buf = (cptr8)_encodedBuffer;
    s32 value = 0;
    s32 length = -1;
    u8 byte = buf[0];
    if( (byte & 0x80) == 0 )
    {
        // This is the only byte
        if( _outCharLength ) *_outCharLength = 1;
        return byte;
    }
    else if( (byte & 0xE0) == 0xC0 )
    {
        // There is one more byte
        value = s32(byte & 0x1F);
        length = 2;
        
        // The value at this moment must not be less than 2, because
        // that should have been encoded with one byte only.
        if( value < 2 )
            length = -1;
    }
    else if( (byte & 0xF0) == 0xE0 )
    {
        // There are two more bytes
        value = s32(byte & 0x0F);
        length = 3;
    }
    else if( (byte & 0xF8) == 0xF0 )
    {
        // There are three more bytes
        value = s32(byte & 0x07);
        length = 4;
    }
    
    s32 n = 1;
    for( ; n < length; n++ )
    {
        byte = buf[n];
        if( (byte & 0xC0) == 0x80 )
            value = (value << 6) + s32(byte & 0x3F);
        else
            break;
    }
    
    if( n == length )
    {
        if( _outCharLength ) *_outCharLength = (unsigned)length;
        return value;
    }
    
    // The byte sequence isn't a valid UTF-8 byte sequence.
    return -1;
}

f32 SVUnicode::encodeUTF8(u32 _value, u8 *_outEncodedBuffer, u32 *_outCharLength){
    u8 *buf = (u8*)_outEncodedBuffer;
    
    int length = -1;
    
    if( _value <= 0x7F )
    {
        buf[0] = _value;
        if( _outCharLength ) *_outCharLength = 1;
        return 1;
    }
    else if( _value >= 0x80 && _value <= 0x7FF )
    {
        // Encode it with 2 characters
        buf[0] = 0xC0 + (_value >> 6);
        length = 2;
    }
    else if( (_value >= 0x800 && _value <= 0xD7FF) || (_value >= 0xE000 && _value <= 0xFFFF) )
    {
        // Note: Values 0xD800 to 0xDFFF are not valid unicode characters
        buf[0] = 0xE0 + (_value >> 12);
        length = 3;
    }
    else if( _value >= 0x10000 && _value <= 0x10FFFF )
    {
        buf[0] = 0xF0 + (_value >> 18);
        length = 4;
    }
    
    int n = length-1;
    for( ; n > 0; n-- )
    {
        buf[n] = 0x80 + (_value & 0x3F);
        _value >>= 6;
    }
    
    if( _outCharLength ) *_outCharLength = length;
    return length;
}

f32 SVUnicode::decodeUTF16(cptr8 _encodedBuffer, u32 *_outCharLength, SVUNICODEBYTEORDER _byteOrder){
    cptr8 buf = (cptr8)_encodedBuffer;
    int value = 0;
    if( _byteOrder == SV_LITTLE_ENDIAN )
    {
        value += buf[0];
        value += (s32)(buf[1]) << 8;
    }
    else
    {
        value += buf[1];
        value += (u32)(buf[0]) << 8;
    }
    
    if( value < 0xD800 || value > 0xDFFF )
    {
        if( _outCharLength ) *_outCharLength = 2;
        return value;
    }
    else if( value < 0xDC00 )
    {
        // We've found the first surrogate word
        value = ((value & 0x3FF)<<10);
        
        // Read the second surrogate word
        int value2 = 0;
        if( _byteOrder == SV_LITTLE_ENDIAN )
        {
            value2 += buf[2];
            value2 += (u32)(buf[3]) << 8;
        }
        else
        {
            value2 += buf[3];
            value2 += (u32)(buf[2]) << 8;
        }
        
        // The second surrogate word must be in the 0xDC00 - 0xDFFF range
        if( value2 < 0xDC00 || value2 > 0xDFFF )
            return -1;
        
        value = value + (value2 & 0x3FF) + 0x10000;
        if( _outCharLength ) *_outCharLength = 4;
        return value;
    }
    
    // It is an illegal sequence if a character in the 0xDC00-0xDFFF range comes first
    return -1;
}

f32 SVUnicode::encodeUTF16(u32 _value, u8 *_outEncodedBuffer, u32 *_outCharLength, SVUNICODEBYTEORDER _byteOrder){
    if( _value < 0x10000 )
    {
        if( _byteOrder == SV_LITTLE_ENDIAN )
        {
            _outEncodedBuffer[0] = (_value & 0xFF);
            _outEncodedBuffer[1] = ((_value >> 8) & 0xFF);
        }
        else
        {
            _outEncodedBuffer[1] = (_value & 0xFF);
            _outEncodedBuffer[2] = ((_value >> 8) & 0xFF);
        }
        
        if( _outCharLength ) *_outCharLength = 2;
        return 2;
    }
    else
    {
        _value -= 0x10000;
        int surrogate1 = ((_value >> 10) & 0x3FF) + 0xD800;
        int surrogate2 = (_value & 0x3FF) + 0xDC00;
        
        if( _byteOrder == SV_LITTLE_ENDIAN )
        {
            _outEncodedBuffer[0] = (surrogate1 & 0xFF);
            _outEncodedBuffer[1] = ((surrogate1 >> 8) & 0xFF);
            _outEncodedBuffer[2] = (surrogate2 & 0xFF);
            _outEncodedBuffer[3] = ((surrogate2 >> 8) & 0xFF);
        }
        else
        {
            _outEncodedBuffer[1] = (surrogate1 & 0xFF);
            _outEncodedBuffer[0] = ((surrogate1 >> 8) & 0xFF);
            _outEncodedBuffer[3] = (surrogate2 & 0xFF);
            _outEncodedBuffer[2] = ((surrogate2 >> 8) & 0xFF);
        }
        
        if( _outCharLength ) *_outCharLength = 4;
        return 4;
    }
}
