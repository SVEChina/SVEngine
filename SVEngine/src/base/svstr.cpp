#include "svstr.h"

namespace sv {
    
    namespace util {
        
        const SVString SVString::null;

        SVString::SVString()
        : length(0)
        , capacity(CAPACITY)
        , data(stack_data) {
            data[length] = '\0';
        }
        
        SVString::SVString(cptr8 s)
        : length(0)
        , capacity(CAPACITY)
        , data(stack_data) {
            copy(s);
        }
        
        SVString::SVString(const SVString &s)
        : length(0)
        , capacity(CAPACITY)
        , data(stack_data) {
            copy(s);
        }
        
        SVString::~SVString() {
            destroy();
        }
        
        SVString &SVString::operator=(cptr8 s) {
            if(data == s) {
                return *this;
            }
            if(s == nullptr) {
                clear();
            } else {
                do_copy(s,(s32)strlen(s));
            }
            return *this;
        }
        
        SVString &SVString::operator=(const SVString &s) {
            if(this == &s) {
                return *this;
            }
            do_copy(s,s.length);
            return *this;
        }
        
        SVString &SVString::operator+=(c8 c) {
            do_append(length,c);
            return *this;
        }
        
        SVString &SVString::operator+=(cptr8 s) {
            if(s == nullptr){
                return *this;
            }
            do_append(length,s,(s32)strlen(s));
            return *this;
        }
        
        SVString &SVString::operator+=(const SVString &s) {
            do_append(length,s.data,s.length);
            return *this;
        }
        
        void SVString::resize(s32 size) {
            allocate(size);
            length = size;
            data[length] = '\0';
        }
        
        void SVString::allocate(s32 size) {
            if(size + 1 <= (capacity & CAPACITY_MASK)) return;
            s32 dynamic = (capacity & DYNAMIC_FLAG);
            capacity = (size + 1) | DYNAMIC_FLAG;
            c8 *new_data = new c8[size + 1];
            do_memcpy(new_data,data,length);
            if(dynamic) {
                delete [] data;
            }
            data = new_data;
            data[length] = '\0';
        }
        
        void SVString::reserve(s32 size) {
            if(size + 1 <= (capacity & CAPACITY_MASK)) return;
            s32 dynamic = (capacity & DYNAMIC_FLAG);
            capacity = (size * 2 + 1) | DYNAMIC_FLAG;
            c8 *new_data = new c8[size * 2 + 1];
            do_memcpy(new_data,data,length);
            if(dynamic) {
                delete [] data;
            }
            data = new_data;
            data[length] = '\0';
        }
    
        void SVString::clear() {
            length = 0;
            data[length] = '\0';
        }
        
        void SVString::destroy() {
            s32 dynamic = (capacity & DYNAMIC_FLAG);
            length = 0;
            capacity = CAPACITY;
            if(dynamic){
                delete [] data;
            }
            data = stack_data;
            data[length] = '\0';
        }
        
        s32 SVString::find(c8 c) const {
            for(s32 i = 0; i < length; i++){
                if(data[i] == c)
                    return i;
            }
            return -1;
        }
        
        s32 SVString::find(cptr8 s) const {
            if(s == nullptr)
                return -1;
            s32 len = (s32)strlen(s);
            for(s32 i = 0; i < length; i++) {
                if(!strncmp(data + i,s,len))
                    return i;
            }
            return -1;
        }
        
        s32 SVString::find(const SVString &s) const {
            for(s32 i = 0; i < length; i++) {
                if(!strncmp(data + i,s.data,s.length))
                    return i;
            }
            return -1;
        }
        
        s32 SVString::rfind(c8 c) const {
            for(s32 i = length - 1; i >= 0; i--){
                if(data[i] == c)
                    return i;
            }
            return -1;
        }
        
        s32 SVString::rfind(cptr8 s) const {
            if(s == nullptr)
                return -1;
            s32 len = (s32)strlen(s);
            for(s32 i = length - 1; i >= 0; i--) {
                if(!strncmp(data + i,s,len))
                    return i;
            }
            return -1;
        }
        
        s32 SVString::rfind(const SVString &s) const {
            for(s32 i = length - 1; i >= 0; i--){
                if(!strncmp(data + i,s.data,s.length))
                    return i;
            }
            return -1;
        }
        
        void SVString::copy(cptr8 s,s32 len) {
            if(s == nullptr) {
                clear();
            } else {
                if(len == -1) {
                    len = (s32)strlen(s);
                }
                do_copy(s,len);
            }
        }
        
        void SVString::copy(const SVString &s,s32 len) {
            if(len == -1){
                len = s.length;
            }
            do_copy(s.data,len);
        }
        
        void SVString::append(c8 c) {
            do_append(length,c);
        }
        
        void SVString::append(cptr8 s,s32 len) {
            if(s == nullptr)
                return;
            if(len == -1) {
                len = (s32)strlen(s);
            }
            do_append(length,s,len);
        }
        
        void SVString::append(const SVString &s,s32 len) {
            if(len == -1) {
                len = s.length;
            }
            do_append(length,s.data,len);
        }
        
        void SVString::append(s32 pos,c8 c) {
            do_append(pos,c);
        }
        
        void SVString::append(s32 pos,cptr8 s,s32 len) {
            if(s == nullptr)
                return;
            if(len == -1) {
                len = (s32)strlen(s);
            }
            do_append(pos,s,len);
        }
        
        void SVString::append(s32 pos,const SVString &s,s32 len) {
            if(len == -1) {
                len = s.length;
            }
            do_append(pos,s.data,len);
        }
        
        void SVString::remove() {
            assert(length > 0 && "SVString::remove(): bad length");
            data[--length] = '\0';
        }
        
        void SVString::remove(s32 pos,s32 len) {
            assert(pos >= 0 && pos <= length && "SVString::remove(): bad position");
            assert(len >= 0 && pos + len <= length && "SVString::remove(): bad length");
            do_memcpy(data + pos,data + pos + len,length - pos - len);
            length -= len;
            data[length] = '\0';
        }
        
        void SVString::do_copy(cptr8 s,s32 len) {
            length = len;
            if(length < (capacity & CAPACITY_MASK)) {
                do_memcpy(data,s,length);
            } else {
                s32 dynamic = (capacity & DYNAMIC_FLAG);
                capacity = (length + 1) | DYNAMIC_FLAG;
                c8 *new_data = new c8[length + 1];
                do_memcpy(new_data,s,length);
                if(dynamic){
                    delete [] data;
                }
                data = new_data;
            }
            data[length] = '\0';
        }
        
        void SVString::do_append(s32 pos,c8 c) {
            assert(pos >= 0 && pos <= length && "SVString::do_append(): bad position");
            s32 new_length = length + 1;
            if(new_length < (capacity & CAPACITY_MASK)) {
                do_memcpy(data + pos + 1,data + pos,length - pos);
                data[pos] = c;
            } else {
                s32 dynamic = (capacity & DYNAMIC_FLAG);
                capacity = (new_length * 2 + 1) | DYNAMIC_FLAG;
                c8 *new_data = new c8[new_length * 2 + 1];
                do_memcpy(new_data,data,pos);
                new_data[pos] = c;
                do_memcpy(new_data + pos + 1,data + pos,length - pos);
                if(dynamic){
                    delete [] data;
                }
                data = new_data;
            }
            length = new_length;
            data[length] = '\0';
        }
        
        void SVString::do_append(s32 pos,cptr8 s,s32 len) {
            assert(pos >= 0 && pos <= length && "SVString::do_append(): bad position");
            s32 new_length = length + len;
            if(new_length < (capacity & CAPACITY_MASK)) {
                do_memcpy(data + pos + len,data + pos,length - pos);
                do_memcpy(data + pos,s,len);
            } else {
                s32 dynamic = (capacity & DYNAMIC_FLAG);
                capacity = (new_length * 2 + 1) | DYNAMIC_FLAG;
                c8 *new_data = new c8[new_length * 2 + 1];
                do_memcpy(new_data,data,pos);
                do_memcpy(new_data + pos,s,len);
                do_memcpy(new_data + pos + len,data + pos,length - pos);
                if(dynamic){
                    delete [] data;
                }
                data = new_data;
            }
            length = new_length;
            data[length] = '\0';
        }
        
        void SVString::do_memcpy(c8 *dest,cptr8 src,s32 size) {
            if(dest > src && (s32)(dest - src) < size) {
                dest += size - 1;
                src += size - 1;
                for(s32 i = size; i > 0; i--) {
                    *dest-- = *src--;
                }
            } else {
#ifdef _WEBGL
                memcpy(dest,src,size);
#else
                if(size & ~15) {
                    for(size_t i = size >> 4; i > 0; i--) {
                        *(u32*)(dest + 0) = *(const u32*)(src + 0);
                        *(u32*)(dest + 4) = *(const u32*)(src + 4);
                        *(u32*)(dest + 8) = *(const u32*)(src + 8);
                        *(u32*)(dest + 12) = *(const u32*)(src + 12);
                        dest += 16;
                        src += 16;
                    }
                    size &= 15;
                }
                if(size & ~3) {
                    for(size_t i = size >> 2; i > 0; i--) {
                        *(u32*)dest = *(const u32*)src;
                        dest += 4;
                        src += 4;
                    }
                    size &= 3;
                }
                if(size) {
                    for(s32 i = size; i > 0; i--) {
                        *dest++ = *src++;
                    }
                }
#endif
            }
        }
        
        void SVString::lower() {
            c8 *s = data;
            while(*s) {
                c8 c = *s;
                if(c >= 'A' && c <= 'Z') {
                    *s += ('a' - 'A');
                }
                s++;
            }
        }
        
        void SVString::upper() {
            c8 *s = data;
            while(*s) {
                c8 c = *s;
                if(c >= 'a' && c <= 'z') {
                    *s -= ('a' - 'A');
                }
                s++;
            }
        }
        
        void SVString::vprintf(cptr8 format,va_list argptr) {
            clear();
            c8 buf[1024];
            SVStringStack<> token;
            cptr8 f = format;
            while(*f) {
                if(*f == '%') {
                    token.clear();
                    token.append(*f++);
                    s32 is_long = 0;
                    while(*f && strchr("0123456789.-+#lL ",*f)) {
                        if(*f == 'l' || *f == 'L') {
                            is_long = 1;
                        }
                        token.append(*f++);
                    }
                    token.append(*f);
                    buf[0] = '\0';
                    if(strchr("cdiouxX",*f)) {
                        if(is_long)
                            sprintf(buf,token,va_arg(argptr,s64));
                        else
                            sprintf(buf,token,va_arg(argptr,s32));
                    } else if(strchr("fgGeE",*f)) {
                        sprintf(buf,token,va_arg(argptr,f64));
                    } else if(strchr("s",*f)) {
                        append(va_arg(argptr,cptr8));
                    } else if(strchr("p",*f)) {
                        sprintf(buf,token,va_arg(argptr,const void*));
                    } else if(strchr("%",*f)) {
                        strcpy(buf,"%");
                    } else {
                        break;
                    }
                    append(buf);
                    if(*f) {
                        f++;
                    }
                } else {
                    append(*f++);
                }
            }
        }
        
        void SVString::printf(cptr8 format,...) {
            va_list argptr;
            va_start(argptr,format);
            vprintf(format,argptr);
            va_end(argptr);
        }
        
        s32 SVString::vscanf(cptr8 format,va_list argptr) {
            s32 ret = 0;
            SVStringStack<> token;
            cptr8 s = data;
            cptr8 f = format;
            cptr8 delimiters = " \t\n\r";
            while(*s && *f) {
                while(*f && strchr(delimiters,*f)) f++;
                if(*f == '%') {
                    while(*s && strchr(delimiters,*s)) s++;
                    if(*s == '\0')
                        break;
                    f++;
                    token.clear();
                    s32 is_long = 0;
                    while(*f && strchr("0123456789.lL ",*f)) {
                        if(*f == 'l' || *f == 'L'){
                            is_long = 1;
                        }
                        token.append(*f++);
                    }
                    s32 length = atoi(token);
                    token.clear();
                    if(strchr("dDiI",*f)) {
                        if(length == 0){
                            while(*s && strchr(delimiters,*s) == nullptr && *s != *(f + 1)) {
                                token.append(*s++);
                            }
                        } else {
                            while(length-- && *s && strchr(delimiters,*s) == nullptr && *s != *(f + 1)) {
                                token.append(*s++);
                            }
                        }
                        if(is_long) {
                            *((s64*)va_arg(argptr,s64*)) = atoi(token);
                        } else {
                            *((s32*)va_arg(argptr,s32*)) = atoi(token);
                        }
                        
                        ret++;
                    } else if(strchr("xX",*f)) {
                        if(length == 0) {
                            while(*s && strchr(delimiters,*s) == nullptr && *s != *(f + 1)) {
                                token.append(*s++);
                            }
                        } else {
                            while(length-- && *s && strchr(delimiters,*s) == nullptr && *s != *(f + 1)) {
                                token.append(*s++);
                            }
                        }
                        if(is_long) {
                            *((s64*)va_arg(argptr,s64*)) = atol("0x" + token);
                        } else {
                            *((s32*)va_arg(argptr,s32*)) = atoi("0x" + token);
                        }
                        ret++;
                    } else if(strchr("fFgGeE",*f)) {
                        if(length == 0) {
                            while(*s && strchr(delimiters,*s) == nullptr && *s != *(f + 1)) {
                                token.append(*s++);
                            }
                        } else {
                            while(length-- && *s && strchr(delimiters,*s) == nullptr && *s != *(f + 1)) {
                                token.append(*s++);
                            }
                        }
                        if(is_long) {
                            *((f64*)va_arg(argptr,f64*)) = atod(token);
                        } else {
                            *((f32*)va_arg(argptr,f32*)) = atof(token);
                        }
                        ret++;
                    } else if(strchr("sS",*f)) {
                        c8 *d = (c8*)va_arg(argptr,c8*);
                        if(length == 0) {
                            while(*s && strchr(delimiters,*s) == nullptr && *s != *(f + 1)) {
                                token.append(*s++);
                            }
                        } else {
                            while(length-- && *s && strchr(delimiters,*s) == nullptr && *s != *(f + 1)) {
                                token.append(*s++);
                            }
                        }
                        *d = '\0';
                        ret++;
                    } else if(strchr("cC",*f)) {
                        *((c8*)va_arg(argptr,c8*)) = *s++;
                        ret++;
                    } else {
                        break;
                    }
                    f++;
                } else {
                    while(*s && strchr(delimiters,*s)){
                        s++;
                    }
                    while(*f && *s && *f == *s) {
                        if(strchr(delimiters,*f))
                            break;
                        f++;
                        s++;
                    }
                    if(*f != '%' && *f != *s) {
                        break;
                    }
                }
            }
            return ret;
        }
        
        s32 SVString::scanf(cptr8 format,...) {
            va_list argptr;
            va_start(argptr,format);
            s32 ret = vscanf(format,argptr);
            va_end(argptr);
            return ret;
        }
        
        //
        //operator ==
        s32 operator ==(const SVString &s0,const SVString &s1) {
            return (strcmp(s0.get(),s1.get()) == 0);
        }
        
        s32 operator ==(const SVString &s0,cptr8 s1) {
            if(s1 == nullptr) return 0;
            return (strcmp(s0.get(),s1) == 0);
        }
        
        s32 operator ==(cptr8 s0,const SVString &s1) {
            if(s0 == nullptr) return 0;
            return (strcmp(s0,s1.get()) == 0);
        }
        
        //operator !=
        s32 operator !=(const SVString &s0,const SVString &s1) {
            return (strcmp(s0.get(),s1.get()) != 0);
        }
        
        s32 operator !=(const SVString &s0,cptr8 s1) {
            if(s1 == nullptr)
                return 0;
            return (strcmp(s0.get(),s1) != 0);
        }
        
        s32 operator !=(cptr8 s0,const SVString &s1) {
            if(s0 == nullptr)
                return 0;
            return (strcmp(s0,s1.get()) != 0);
        }
        
        //operator >
        s32 operator >(const SVString &s0,const SVString &s1) {
            SVString t_low_s0 = s0;
            t_low_s0.lower();
            SVString t_low_s1 = s1;
            t_low_s1.lower();
            //字典排序
            s32 t_len0 = t_low_s0.size();
            s32 t_len1 = t_low_s1.size();
            s32 t_size = t_len0>t_len1?t_len1:t_len0;
            for(s32 i=0;i<t_size;i++) {
                if( t_low_s0[i]==t_low_s1[i]){
                    continue;
                }
                if( t_low_s0[i]>t_low_s1[i]){
                    return 1;
                }else{
                    return 0;
                }
            }
            //比较长度
            if(t_len0>t_len1)
                return 1;
            return 0;
        }
        
        s32 operator >(const SVString &s0,cptr8 s1) {
            if(s1 == nullptr)
                return 0;
            SVString t_s1(s1);
            return s0>t_s1;
        }
        
        s32 operator >(cptr8 s0,const SVString &s1) {
            if(s0 == nullptr)
                return 0;
            SVString t_s0(s0);
            return t_s0>s1;
        }
        
        //operator <
        s32 operator <(const SVString &s0,const SVString &s1) {
            if(s0 == s1)
                return 0;
            s32 t_value = s0>s1;
            if(t_value == 1)
                return 0;
            return 1;
        }
        
        s32 operator <(const SVString &s0,cptr8 s1) {
            if(s0 == s1)
                return 0;
            s32 t_value = s0>s1;
            if(t_value == 1)
                return 0;
            return 1;
        }
        
        s32 operator <(cptr8 s0,const SVString &s1) {
            if(s0 == s1)
                return 0;
            s32 t_value = s0>s1;
            if(t_value == 1)
                return 0;
            return 1;
        }
        
        //operator >=
        s32 operator >=(const SVString &s0,const SVString &s1) {
            if(s0<s1)
                return 0;
            return 1;
        }
        
        s32 operator >=(const SVString &s0,cptr8 s1) {
            if(s1 == nullptr)
                return 0;
            if(s0<s1)
                return 0;
            return 1;
        }
        
        s32 operator >=(cptr8 s0,const SVString &s1) {
            if(s0 == nullptr)
                return 0;
            if(s0<s1)
                return 0;
            return 1;
        }
        
        //operator <=
        s32 operator <=(const SVString &s0,const SVString &s1) {
            if(s0>s1)
                return 0;
            return 1;
        }
        
        s32 operator <=(const SVString &s0,cptr8 s1) {
            if(s0>s1)
                return 0;
            return 1;
        }
        
        s32 operator <=(cptr8 s0,const SVString &s1) {
            if(s0>s1)
                return 0;
            return 1;
        }
        
        /************************* SVString functions ****************************/
        
        SVStringStack<> operator+(const SVString &s0,const SVString &s1) {
            SVStringStack<> ret = s0;
            ret.append(s1);
            return ret;
        }
        
        SVStringStack<> operator+(const SVString &s0,cptr8 s1) {
            SVStringStack<> ret = s0;
            ret.append(s1);
            return ret;
        }
        
        SVStringStack<> operator+(cptr8 s0,const SVString &s1) {
            SVStringStack<> ret = SVStringStack<>(s0);
            ret.append(s1);
            return ret;
        }
        
        SVStringStack<> operator+(const SVString &s0,char c1) {
            SVStringStack<> ret = s0;
            ret.append(c1);
            return ret;
        }
        
        SVStringStack<> operator+(char c0,const SVString &s1) {
            SVStringStack<> ret;
            ret.append(c0);
            ret.append(s1);
            return ret;
        }
        
        /***************SVString static functions **********************/
        s32 SVString::isdigit(s32 code) {
            return (code >= '0' && code <= '9');
        }
        
        s32 SVString::islower(s32 code) {
            return (code >= 'a' && code <= 'z');
        }
        
        s32 SVString::isupper(s32 code) {
            return (code >= 'A' && code <= 'Z');
        }
        
        s32 SVString::isalpha(s32 code) {
            return ((code >= 'a' && code <= 'z') || (code >= 'A' && code <= 'Z'));
        }
        
        s32 SVString::atoi(cptr8 str) {
            s32 ret = 0;
            s32 negative = 0;
            cptr8 s = str;
            while(*s == ' ' || *s == '\t') {
                s++;
            }
            if(*s == '-' || *s == '+') {
                if(*s == '-'){
                    negative = 1;
                }
                s++;
            }
            if(*s == '0' && (*(s + 1) == 'x' || *(s + 1) == 'X')) {
                s += 2;
                while(*s) {
                    c8 c = *s++;
                    if(c >= '0' && c <= '9'){
                        ret = (ret << 4) + (c - '0');
                    } else if(c >= 'a' && c <= 'f') {
                        ret = (ret << 4) + (c - 'a' + 10);
                    } else if(c >= 'A' && c <= 'F') {
                        ret = (ret << 4) + (c - 'A' + 10);
                    } else {
                        break;
                    }
                }
            } else {
                while(*s) {
                    c8 c = *s++;
                    if(c < '0' || c > '9')
                        break;
                    ret = ret * 10 + (c - '0');
                }
            }
            if(negative)
                return -ret;
            return ret;
        }
        
        s64 SVString::atol(cptr8 str) {
            s64 ret = 0;
            s32 negative = 0;
            cptr8 s = str;
            while(*s == ' ' || *s == '\t') {
                s++;
            }
            if(*s == '-' || *s == '+') {
                if(*s == '-')
                    negative = 1;
                s++;
            }
            if(*s == '0' && (*(s + 1) == 'x' || *(s + 1) == 'X')) {
                s += 2;
                while(*s) {
                    c8 c = *s++;
                    if(c >= '0' && c <= '9') {
                        ret = (ret << 4) + (c - '0');
                    } else if(c >= 'a' && c <= 'f') {
                        ret = (ret << 4) + (c - 'a' + 10);
                    } else if(c >= 'A' && c <= 'F') {
                        ret = (ret << 4) + (c - 'A' + 10);
                    } else {
                        break;
                    }
                }
            } else {
                while(*s) {
                    c8 c = *s++;
                    if(c < '0' || c > '9')
                        break;
                    ret = ret * 10 + (c - '0');
                }
            }
            if(negative) {
                return -ret;
            }
            return ret;
        }
        
        f32 SVString::atof(cptr8 str) {
            return (f32)atod(str);
        }
        
        f64 SVString::atod(cptr8 str) {
            f64 ret = 0.0;
            s32 negative = 0;
            cptr8 s = str;
            while(*s == ' ' || *s == '\t') {
                s++;
            }
            if(*s == '-' || *s == '+') {
                if(*s == '-')
                    negative = 1;
                s++;
            }
            if(*s >= '0' && *s <= '9') {
                while(*s) {
                    c8 c = *s;
                    if(c < '0' || c > '9')
                        break;
                    ret = ret * 10.0 + (c - '0');
                    s++;
                }
            }
            if(*s == '.') {
                s++;
                if(*s >= '0' && *s <= '9') {
                    f64 frac = 0.0;
                    f64 scale = 0.1;
                    while(*s) {
                        c8 c = *s;
                        if(c < '0' || c > '9')
                            break;
                        frac += scale * (c - '0');
                        scale *= 0.1;
                        s++;
                    }
                    ret += frac;
                }
            }
            if(*s == 'e' || *s == 'E') {
                s++;
                f64 exp = 0.0;
                s32 negative = 0;
                if(*s == '-' || *s == '+') {
                    if(*s == '-')
                        negative = 1;
                    s++;
                }
                if(*s >= '0' && *s <= '9') {
                    while(*s) {
                        c8 c = *s++;
                        if(c < '0' || c > '9')
                            break;
                        exp = exp * 10.0 + (c - '0');
                    }
                }
                if(negative)
                    exp = -exp;
                ret *= pow(10.0,exp);
            }
            if(negative)
                return -ret;
            return ret;
        }
        
        SVStringStack<> SVString::memory(size_t size) {
            SVStringStack<> ret;
            if(size < 1024) {
                ret.printf("%dB",size);
            } else if(size < 1024 * 1024) {
                ret.printf("%.1fKB",size / 1024.0);
            } else if(size < 1024 * 1024 * 1024) {
                ret.printf("%.1fMB",size / (1024.0 * 1024.0));
            } else {
                ret.printf("%.1fGB",size / (1024.0 * 1024.0 * 1024.0));
            }
            return ret;
        }
        
        SVStringStack<> SVString::lower(cptr8 str) {
            SVStringStack<> ret = SVStringStack<>(str);
            ret.lower();
            return ret;
        }
        
        SVStringStack<> SVString::upper(cptr8 str) {
            SVStringStack<> ret = SVStringStack<>(str);
            ret.upper();
            return ret;
        }
        
        SVStringStack<> SVString::format(cptr8 format,va_list argptr) {
            SVStringStack<> ret;
            ret.vprintf(format,argptr);
            return ret;
        }
        
        SVStringStack<> SVString::format(cptr8 format,...) {
            SVStringStack<> ret;
            va_list argptr;
            va_start(argptr,format);
            ret.vprintf(format,argptr);
            va_end(argptr);
            return ret;
        }
        
        s32 SVString::vsscanf(cptr8 str,cptr8 format,va_list argptr) {
            return SVStringStack<>(str).vscanf(format,argptr);
        }
        
        s32 SVString::sscanf(cptr8 str,cptr8 format,...) {
            va_list argptr;
            va_start(argptr,format);
            s32 ret = SVStringStack<>(str).vscanf(format,argptr);
            va_end(argptr);
            return ret;
        }
        
        SVStringStack<> SVString::trim(cptr8 str,cptr8 symbols) {
            if(symbols == nullptr) {
                symbols = " ";
            }
            cptr8 s = str;
            while(*s && strchr(symbols,*s) != nullptr) {
                s++;
            }
            SVStringStack<> ret = SVStringStack<>(s);
            str = ret.get();
            s = str + ret.size() - 1;
            while(s > str && strchr(symbols,*s--) != nullptr) {
                ret.remove();
            }
            return ret;
        }
        
        SVStringStack<> SVString::replace(cptr8 str,cptr8 before,cptr8 after) {
            SVStringStack<> ret;
            cptr8 s = str;
            s32 length = (s32)strlen(before);
            while(*s) {
                if(length && !strncmp(s,before,length)) {
                    ret += after;
                    s += length;
                } else {
                    ret += *s++;
                }
            }
            return ret;
        }
        
        SVStringArray<> SVString::split(cptr8 str,cptr8 delimiters) {
            SVStringArray<> ret((s32)strlen(str));
            cptr8 s = str;
            char *d = ret.data;
            s32 index = -1;
            while(*s) {
                if(strchr(delimiters,*s)) {
                    while(*s && strchr(delimiters,*s)) {
                        *d++ = '\0';
                        s++;
                    }
                    if(index != -1) ret.append(index);
                    index = -1;
                } else {
                    if(*s == '\\' && *(s + 1) != '\0' && strchr(delimiters,*(s + 1))) s++;
                    if(index == -1) index = (s32)(d - ret.data);
                    *d++ = *s++;
                }
            }
            if(index != -1) ret.append(index);
            *d = '\0';
            return ret;
        }
        
        SVStringStack<> SVString::substr(cptr8 str,s32 pos,s32 len) {
            SVStringStack<> ret;
            s32 length = (s32)strlen(str);
            if(len == -1) len = length - pos;
            for(s32 i = 0; i < len; i++) {
                if(pos + i < 0) ret += ' ';
                else if(pos + i < length) ret += str[pos + i];
                else ret += ' ';
            }
            return ret;
        }
        
        SVStringStack<> SVString::addslashes(cptr8 str) {
            SVStringStack<> ret;
            cptr8 s = str;
            while(*s) {
                if(*s == '\t'){
                    ret += "\\t";
                } else if(*s == '\n'){
                    ret += "\\n";
                } else if(*s == '\r') {
                    ret += "\\r";
                } else if(*s == '"') {
                    ret += "\\\"";
                } else if(*s == '\\') {
                    ret += "\\\\";
                } else {
                    ret += *s;
                }
                s++;
            }
            return ret;
        }
        
        SVStringStack<> SVString::stripslashes(cptr8 str) {
            SVStringStack<> ret;
            cptr8 s = str;
            while(*s) {
                if(*s == '\\') {
                    s++;
                    if(*s == 't') {
                        ret += '\t';
                    } else if(*s == 'n') {
                        ret += '\n';
                    } else if(*s == 'r') {
                        ret += '\r';
                    } else if(*s == '"') {
                        ret += '\"';
                    } else if(*s == '\\') {
                         ret += '\\';
                    } else if(*s == '0' && (*(s + 1) >= '0' && *(s + 1) <= '7') && (*(s + 2) >= '0' && *(s + 2) <= '7')) {
                        ret += (*(s + 1) - '0') * 8 + (*(s + 2) - '0');
                        s += 2;
                    } else {
                        ret += SVString::format("\\%c",*s);
                    }
                    s++;
                } else {
                    ret += *s++;
                }
            }
            return ret;
        }
        
        SVStringStack<> SVString::dirname(cptr8 str) {
            cptr8 s = strrchr(str,'/');
            if(s == nullptr) {
                s = strrchr(str,'\\');
            }
            if(s == nullptr) {
                return SVStringStack<>();
            }
            return substr(str,0,(s32)(s - str) + 1);
        }
        
        SVStringStack<> SVString::basename(cptr8 str) {
            cptr8 s = strrchr(str,'/');
            if(s == nullptr) {
                s = strrchr(str,'\\');
            }
            if(s == nullptr) {
                return SVStringStack<>(str);
            }
            return SVStringStack<>(s + 1);
        }
        
        SVStringStack<> SVString::pathname(cptr8 str) {
            SVStringStack<> ret;
            cptr8 s = str;
            while(1) {
                if(*s == '/' && ret.size() > 0 && ret[ret.size() - 1] == '/') {
                    s++;
                } else if(!strncmp(s,"./",2)) {
                    s += 2;
                } else if(!strncmp(s,"../",3)) {
                    s += 3;
                    if(ret.size() > 0 && ret[ret.size() - 1] == '/' && (ret.size() < 3 || strncmp(ret.get() + ret.size() - 3,"../",3))) {
#ifdef _WIN32
                        if(ret.size() != 3 || isalpha(ret[0]) == 0 || ret[1] != ':' || ret[2] != '/') {
#else
                            if(ret.size() != 1 || ret[0] != '/') {
#endif
                                ret.remove();
                                s32 pos = ret.rfind('/');
                                if(pos == -1) {
                                    ret.clear();
                                } else {
                                    ret.remove(pos + 1,ret.size() - pos - 1);
                                }
                            }
                        } else {
#ifdef _WIN32
                            if(ret.size() < 3 || isalpha(ret[0]) == 0 || ret[1] != ':' || ret[2] != '/') {
                                ret.append("../");
                            }
#else
                            if(ret.size() < 1 || ret[0] != '/') {
                                ret.append("../");
                            }
#endif
                        }
                    } else {
                        cptr8 slash = strchr(s,'/');
                        if(slash == nullptr)
                            break;
                        ret.append(s,(s32)(slash - s) + 1);
                        s = slash + 1;
                    }
                }
                return ret;
            }
            
            SVStringStack<> SVString::absname(cptr8 path,cptr8 str) {
                SVStringStack<> ret;
#ifdef _WIN32
                cptr8 s = str;
                if(*s == '\0' || *(s + 1) != ':') {
                    ret = path;
                }
#else
                ret = path;
#endif
                ret += str;
                return ret;
            }
            
            SVStringStack<> SVString::relname(cptr8 path,cptr8 str) {
                SVStringStack<> ret;
                cptr8 p = path;
                cptr8 s = str;
                while(*p && *p == *s) {
                    p++;
                    s++;
                }
                while(p > path && *p != '/') {
                    p--;
                }
                while(s > str && *s != '/') {
                    s--;
                }
                if(*p != '/' || *s != '/') {
                    return SVStringStack<>(str);
                }
                while(*p) {
                    if(*++p == '/') {
                        ret += "../";
                    }
                }
                ret += s + 1;
                return ret;
            }
            
            SVStringStack<> SVString::extension(cptr8 str,cptr8 ext) {
                SVStringStack<> ret(str);
                s32 pos = ret.rfind('.');
                if(pos != -1 && pos > ret.rfind('/') && pos > ret.rfind('\\')) {
                    if(lower(substr(ret,pos)) != ext) {
                        ret.remove(pos,ret.size() - pos);
                        ret += ext;
                    }
                } else {
                    ret += ext;
                }
                return ret;
            }
            
            /******************** SVString unicode functions ********************************/
            SVStringStack<> SVString::unicodeToUtf8(const wchar_t *src) {
                SVStringStack<> ret;
                const wchar_t *s = src;
                while(*s) {
                    u32 code = *s++;
                    if(code < 0x80) {
                        ret += code & 0x7f;
                    } else if(code < 0x800) {
                        ret += 0xc0 | (code >> 6);
                        ret += 0x80 | (code & 0x3f);
                    } else if(code < 0x10000) {
                        ret += 0xe0 | (code >> 12);
                        ret += 0x80 | ((code >> 6) & 0x3f);
                        ret += 0x80 | (code & 0x3f);
                    }
                }
                return ret;
            }
            
            SVStringStack<> SVString::unicodeToUtf8(const u32 *src) {
                SVStringStack<> ret;
                const u32 *s = src;
                while(*s) {
                    u32 code = *s++;
                    if(code < 0x80) {
                        ret += code & 0x7f;
                    } else if(code < 0x800) {
                        ret += 0xc0 | (code >> 6);
                        ret += 0x80 | (code & 0x3f);
                    } else if(code < 0x10000) {
                        ret += 0xe0 | (code >> 12);
                        ret += 0x80 | ((code >> 6) & 0x3f);
                        ret += 0x80 | (code & 0x3f);
                    } else if(code < 0x200000) {
                        ret += 0xf0 | (code >> 18);
                        ret += 0x80 | ((code >> 12) & 0x3f);
                        ret += 0x80 | ((code >> 6) & 0x3f);
                        ret += 0x80 | (code & 0x3f);
                    }
                }
                return ret;
            }
            
            s32 SVString::utf8ToUnicode(cptr8 src,u32 &code) {
                const u8 *s = (const u8*)src;
                code = *s;
                if(code > 0xf0) {
                    code = (u32)(*s++ & 0x07) << 18;
                    if(*s) {
                        code |= (u32)(*s++ & 0x3f) << 12;
                    }
                    if(*s) {
                        code |= (u32)(*s++ & 0x3f) << 6;
                    }
                    if(*s) {
                        code |= (u32)(*s++ & 0x3f);
                    }
                } else if(code > 0xe0) {
                    code = (u32)(*s++ & 0x0f) << 12;
                    if(*s) {
                        code |= (u32)(*s++ & 0x3f) << 6;
                    }
                    if(*s) {
                        code |= (u32)(*s++ & 0x3f);
                    }
                } else if(code > 0xc0) {
                    code = (u32)(*s++ & 0x1f) << 6;
                    if(*s) {
                        code |= (u32)(*s++ & 0x3f);
                    }
                } else {
                    s++;
                }
                return (s32)(s - (const u8*)src);
            }
            
            s32 SVString::utf8ToUnicode(cptr8 src,u32 *dest,s32 size) {
                cptr8 s = src;
                u32 *d = dest;
                u32 code = 0;
                while(*s && --size) {
                    s += utf8ToUnicode(s,code);
                    *d++ = code;
                }
                *d = '\0';
                return (s32)(d - dest);
            }
            
            s32 SVString::utf8ToUnicode(cptr8 src,wchar_t *dest,s32 size) {
                cptr8 s = src;
                wchar_t *d = dest;
                u32 code = 0;
                while(*s && --size) {
                    s += utf8ToUnicode(s,code);
                    if(code < 0x10000) {
                        *d++ = (wchar_t)code;
                    } else {
                        *d++ = 0x25a1;
                    }
                }
                *d = '\0';
                return (s32)(d - dest);
            }
            
            s32 SVString::utf8strlen(cptr8 str) {
                const u8 *s = (const u8*)str;
                s32 length = 0;
                while(*s) {
                    s32 code = *s++;
                    if(code > 0xf0) {
                        if(*s) s++;
                        if(*s) s++;
                        if(*s) s++;
                    } else if(code > 0xe0) {
                        if(*s) s++;
                        if(*s) s++;
                    } else if(code > 0xc0) {
                        if(*s) s++;
                    }
                    length++;
                }
                return length;
            }
            
            SVStringStack<> SVString::utf8substr(cptr8 str,s32 pos,s32 len) {
                SVStringStack<> ret;
                const u8 *s = (const u8*)str;
                s32 length = 0;
                while(*s) {
                    if(length == pos) {
                        str = (cptr8)s;
                    }
                    s32 code = *s++;
                    if(code > 0xf0) {
                        if(*s) s++;
                        if(*s) s++;
                        if(*s) s++;
                    } else if(code > 0xe0) {
                        if(*s) s++;
                        if(*s) s++;
                    } else if(code > 0xc0) {
                        if(*s) s++;
                    }
                    length++;
                }
                s = (const u8*)str;
                if(len == -1) {
                    len = length - pos;
                }
                for(s32 i = 0; i < len; i++) {
                    if(pos + i < 0) ret += ' ';
                    else if(pos + i < length) {
                        s32 code = *s++;
                        ret += (c8)code;
                        if(code > 0xf0) {
                            if(*s) ret += *s++;
                            if(*s) ret += *s++;
                            if(*s) ret += *s++;
                        } else if(code > 0xe0) {
                            if(*s) ret += *s++;
                            if(*s) ret += *s++;
                        } else if(code > 0xc0) {
                            if(*s) ret += *s++;
                        }
                    }
                    else ret += ' ';
                }
                return ret;
            }
            
            /******************** SVString compare functions ************/
            s32 SVString::match(cptr8 pattern,cptr8 str) {
                s32 ret = 1;
                cptr8 p = pattern;
                cptr8 s = str;
                while(*p) {
                    if(*p == '*' && ret) {
                        if(*(p + 1) == '\0' || *(p + 1) == '|')
                            break;
                        p++;
                        s32 len = 0;
                        while(p[len] && p[len] != '|') {
                            len++;
                        }
                        while(*s) {
                            s++;
                        }
                        if(s - str >= len) {
                            s -= len;
                        }
                    } else if(*p == '|') {
                        if(ret)
                            break;
                        p++;
                        s = str;
                        ret = 1;
                    } else {
                        if(*p != '?' && *p != *s) {
                            ret = 0;
                        }
                        if(*s) {
                            s++;
                        }
                        p++;
                    }
                }
                if(*s != '\0' && *p == '\0')
                    ret = 0;
                return ret;
            }
            
    }//!namespace util
    
}//!namespace sv

