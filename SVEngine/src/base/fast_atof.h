//
// fast_atof.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
// copy from irr
//

#ifndef FAST_ATOF_H
#define FAST_ATOF_H

#include "SVCoreMath.h"

namespace sv {
    //! Selection of characters which count as decimal point in fast_atof
    // TODO: This should probably also be used in irr::core::string, but
    // the float-to-string code used there has to be rewritten first.
    SV_API extern sv::SVString LOCALE_DECIMAL_POINTS;

// we write [17] here instead of [] to work around a swig bug
    const float fast_atof_table[17] = {
            0.f,
            0.1f,
            0.01f,
            0.001f,
            0.0001f,
            0.00001f,
            0.000001f,
            0.0000001f,
            0.00000001f,
            0.000000001f,
            0.0000000001f,
            0.00000000001f,
            0.000000000001f,
            0.0000000000001f,
            0.00000000000001f,
            0.000000000000001f,
            0.0000000000000001f
    };

    //
    inline u32 strtoul10(const char *in, const char **out = 0) {
        if (!in) {
            if (out)
                *out = in;
            return 0;
        }

        bool overflow = false;
        u32 unsignedValue = 0;
        while ((*in >= '0') && (*in <= '9')) {
            const u32 tmp = (unsignedValue * 10) + (*in - '0');
            if (tmp < unsignedValue) {
                unsignedValue = (u32) 0xffffffff;
                overflow = true;
            }
            if (!overflow)
                unsignedValue = tmp;
            ++in;
        }

        if (out)
            *out = in;

        return unsignedValue;
    }

    //nline s32 strtol10(const char *in, const char **out = 0) {
        if (!in) {
            if (out)
                *out = in;
            return 0;
        }

        const bool negative = ('-' == *in);
        if (negative || ('+' == *in))
            ++in;

        const u32 unsignedValue = strtoul10(in, out);
        if (unsignedValue > (u32) INT_MAX) {
            if (negative)
                return (s32) INT_MIN;
            else
                return (s32) INT_MAX;
        } else {
            if (negative)
                return -((s32) unsignedValue);
            else
                return (s32) unsignedValue;
        }
    }

    //
    inline u32 ctoul16(char in) {
        if (in >= '0' && in <= '9')
            return in - '0';
        else if (in >= 'a' && in <= 'f')
            return 10u + in - 'a';
        else if (in >= 'A' && in <= 'F')
            return 10u + in - 'A';
        else
            return 0xffffffff;
    }

    //
    inline u32 strtoul16(const char *in, const char **out = 0) {
        if (!in) {
            if (out)
                *out = in;
            return 0;
        }

        bool overflow = false;
        u32 unsignedValue = 0;
        while (true) {
            u32 tmp = 0;
            if ((*in >= '0') && (*in <= '9'))
                tmp = (unsignedValue << 4u) + (*in - '0');
            else if ((*in >= 'A') && (*in <= 'F'))
                tmp = (unsignedValue << 4u) + (*in - 'A') + 10;
            else if ((*in >= 'a') && (*in <= 'f'))
                tmp = (unsignedValue << 4u) + (*in - 'a') + 10;
            else
                break;
            if (tmp < unsignedValue) {
                unsignedValue = (u32) INT_MAX;
                overflow = true;
            }
            if (!overflow)
                unsignedValue = tmp;
            ++in;
        }

        if (out)
            *out = in;

        return unsignedValue;
    }

    //
    inline u32 strtoul8(const char *in, const char **out = 0) {
        if (!in) {
            if (out)
                *out = in;
            return 0;
        }

        bool overflow = false;
        u32 unsignedValue = 0;
        while (true) {
            u32 tmp = 0;
            if ((*in >= '0') && (*in <= '7'))
                tmp = (unsignedValue << 3u) + (*in - '0');
            else
                break;
            if (tmp < unsignedValue) {
                unsignedValue = (u32) INT_MAX;
                overflow = true;
            }
            if (!overflow)
                unsignedValue = tmp;
            ++in;
        }

        if (out)
            *out = in;

        return unsignedValue;
    }

    //
    inline u32 strtoul_prefix(const char *in, const char **out = 0) {
        if (!in) {
            if (out)
                *out = in;
            return 0;
        }
        if ('0' == in[0])
            return ('x' == in[1] ? strtoul16(in + 2, out) : strtoul8(in + 1, out));
        return strtoul10(in, out);
    }

    //
    inline f32 strtof10(const char *in, const char **out = 0) {
        if (!in) {
            if (out)
                *out = in;
            return 0.f;
        }

        const u32 MAX_SAFE_U32_VALUE = UINT_MAX / 10 - 10;
        u32 intValue = 0;

        // Use integer arithmetic for as long as possible, for speed
        // and precision.
        while ((*in >= '0') && (*in <= '9')) {
            // If it looks like we're going to overflow, bail out
            // now and start using floating point.
            if (intValue >= MAX_SAFE_U32_VALUE)
                break;

            intValue = (intValue * 10) + (*in - '0');
            ++in;
        }

        f32 floatValue = (f32) intValue;

        // If there are any digits left to parse, then we need to use
        // floating point arithmetic from here.
        while ((*in >= '0') && (*in <= '9')) {
            floatValue = (floatValue * 10.f) + (f32) (*in - '0');
            ++in;
            if (floatValue > FLT_MAX) // Just give up.
                break;
        }

        if (out)
            *out = in;

        return floatValue;
    }
    
    //
    inline const char *fast_atof_move(const char *in, f32 &result) {
        // Please run the regression test when making any modifications to this function.

        result = 0.f;
        if (!in)
            return 0;

        const bool negative = ('-' == *in);
        if (negative || ('+' == *in))
            ++in;

        f32 value = strtof10(in, &in);

        if (LOCALE_DECIMAL_POINTS.findFirst(*in) >= 0) {
            const char *afterDecimal = ++in;
            const f32 decimal = strtof10(in, &afterDecimal);
            value += decimal * fast_atof_table[afterDecimal - in];
            in = afterDecimal;
        }

        if ('e' == *in || 'E' == *in) {
            ++in;
            // Assume that the exponent is a whole number.
            // strtol10() will deal with both + and - signs,
            // but calculate as f32 to prevent overflow at FLT_MAX
            value *= powf(10.f, (f32) strtol10(in, &in));
        }

        result = negative ? -value : value;
        return in;
    }
    
    //
    inline float fast_atof(const char *floatAsString, const char **out = 0) {
        float ret;
        if (out)
            *out = fast_atof_move(floatAsString, ret);
        else
            fast_atof_move(floatAsString, ret);
        return ret;
    }

}//!namespace sv

#endif

