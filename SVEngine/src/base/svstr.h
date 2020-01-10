#ifndef __STRING_H__
#define __STRING_H__

#include "SVBase.h"
#include "SVArray.h"

namespace sv {
    
    namespace util {
        
        class SVString;
        class SVStringArray;
        template <s32 Capacity = 256> class SVStringStack;
        
        class SVString {
        public:
            SVString();
            
            //explicit SVString(cptr8 s);
            SVString(cptr8 s);
            
            SVString(const SVString &s);
            
            virtual ~SVString();
            
            SVString &operator=(cptr8 s);
            
            SVString &operator=(const SVString &s);
            
            SVString &operator+=(c8 c);
            
            SVString &operator+=(cptr8 s);
            
            SVString &operator+=(const SVString &s);
            
            sv_inline cptr8 get() const { return data; }
            
            sv_inline cptr8 c_str() const { return data; }
            
            sv_inline operator cptr8() const { return data; }
            
            sv_inline operator const void*() const { return data; }
            
            sv_inline c8 &get(s32 index) {
                assert((u32)index < (u32)length && "SVString::get(): bad index");
                return data[index];
            }
            sv_inline c8 get(s32 index) const {
                assert((u32)index < (u32)length && "SVString::get(): bad index");
                return data[index];
            }
            
            sv_inline c8 &operator[](s32 index) {
                assert((u32)index < (u32)length && "SVString::operator[](): bad index");
                return data[index];
            }
            sv_inline c8 operator[](s32 index) const {
                assert((u32)index < (u32)length && "SVString::operator[](): bad index");
                return data[index];
            }
            
            sv_inline s32 size() const {
                return length;
            }
            
            sv_inline s32 empty() const {
                return (length == 0);
            }
            
            void resize(s32 size);
            
            void allocate(s32 size);
            
            void reserve(s32 size);
            
            void clear();
            
            void destroy();
            
            s32 find(c8 c) const;
            
            s32 find(cptr8 s) const;
            
            s32 find(const SVString &s) const;
            
            s32 rfind(c8 c) const;
            
            s32 rfind(cptr8 s) const;
            
            s32 rfind(const SVString &s) const;
            
            void copy(cptr8 s,s32 len = -1);
            
            void copy(const SVString &s,s32 len = -1);
            
            void append(c8 c);
            
            void append(cptr8 s,s32 len = -1);
            
            void append(const SVString &s,s32 len = -1);
            
            void append(s32 pos,c8 c);
            
            void append(s32 pos,cptr8 s,s32 len = -1);
            
            void append(s32 pos,const SVString &s,s32 len = -1);
            
            void remove();
            
            void remove(s32 pos,s32 len = 1);
            
            void lower();
            
            void upper();
            
            void vprintf(cptr8 format,va_list argptr);
            
            void printf(cptr8 format,...);
            
            s32 vscanf(cptr8 format,va_list argptr);
            
            s32 scanf(cptr8 format,...);
            
            static s32 isdigit(s32 code);
            static s32 islower(s32 code);
            static s32 isupper(s32 code);
            static s32 isalpha(s32 code);
            
            static s32 atoi(cptr8 str);
            static s64 atol(cptr8 str);
            static f32 atof(cptr8 str);
            static f64 atod(cptr8 str);
            
            static SVString itoa(s32 value, c8* result, s32 base);
            
            static SVStringStack<> memory(size_t memory);
            
            static SVStringStack<> lower(cptr8 str);
            static SVStringStack<> upper(cptr8 str);
            
            static SVStringStack<> format(cptr8 format,va_list argptr);
            static SVStringStack<> format(cptr8 format,...);
            
            static s32 vsscanf(cptr8 str,cptr8 format,va_list argptr);
            static s32 sscanf(cptr8 str,cptr8 format,...);
            
            static SVStringStack<> trim(cptr8 str,cptr8 symbols = 0);
            static SVStringStack<> replace(cptr8 str,cptr8 before,cptr8 after);
            
            static SVStringStack<> substr(cptr8 str,s32 pos,s32 len = -1);
            static SVStringStack<> addslashes(cptr8 str);
            static SVStringStack<> stripslashes(cptr8 str);
            
            static SVStringStack<> dirname(cptr8 str);
            static SVStringStack<> basename(cptr8 str);
            static SVStringStack<> pathname(cptr8 str);
            
            static SVStringStack<> absname(cptr8 path,cptr8 str);
            static SVStringStack<> relname(cptr8 path,cptr8 str);
            static SVStringStack<> extension(cptr8 str,cptr8 ext);
            
            static SVStringStack<> unicodeToUtf8(const wchar_t *src);
            static SVStringStack<> unicodeToUtf8(const u32 *src);
            static s32 utf8ToUnicode(cptr8 src,u32 &code);
            static s32 utf8ToUnicode(cptr8 src,wchar_t *dest,s32 size);
            static s32 utf8ToUnicode(cptr8 src,u32 *dest,s32 size);
            
            static s32 utf8strlen(cptr8 str);
            static SVStringStack<> utf8substr(cptr8 str,s32 pos,s32 len = -1);
            
            static s32 match(cptr8 pattern,cptr8 str);
            
            static const SVString null;
            
        protected:
            
            enum {
                CAPACITY = 4,
                DYNAMIC_FLAG = (s32)(1 << (sizeof(s32) * 8 - 1)),
                CAPACITY_MASK = ~(s32)DYNAMIC_FLAG,
            };
            
            void do_copy(cptr8 s,s32 len);
            void do_append(s32 pos,c8 c);
            void do_append(s32 pos,cptr8 s,s32 len);
            void do_memcpy(c8 *dest,cptr8 src,s32 size);
            
            s32 length;
            s32 capacity;
            c8 *data;
            c8 stack_data[CAPACITY];
        };
        
        //
        template <s32 Capacity> class SVStringStack : public SVString {
        public:
            SVStringStack() {
                capacity = Capacity;
                data = stack_data;
                data[length] = '\0';
            }
            
            SVStringStack(const SVString &s) {
                capacity = Capacity;
                data = stack_data;
                copy(s);
            }
            
            SVStringStack(const SVStringStack &s) {
                capacity = Capacity;
                data = stack_data;
                copy(s);
            }
            
            explicit SVStringStack(cptr8 s) {
                capacity = Capacity;
                data = stack_data;
                copy(s);
            }
            
            ~SVStringStack() {
            }
            
            SVStringStack &operator=(cptr8 s) {
                if(data == s) {
                    return *this;
                }
                copy(s);
                return *this;
            }
            
            SVStringStack &operator=(const SVString &s) {
                if(this == &s) {
                    return *this;
                }
                copy(s);
                return *this;
            }
            
            SVStringStack &operator=(const SVStringStack &s) {
                if(this == &s) {
                    return *this;
                }
                copy(s);
                return *this;
            }
        private:
            c8 stack_data[Capacity];
        };
        
        //
        class SVStringArray {
        private:
            friend class SVString;

            void append(cptr8 _str) {
                m_strvec.append(_str);
            }
            
        public:
            SVStringArray(){}
            
            SVStringArray(const SVStringArray &s){
                for(s32 i=0;i<s.size();i++) {
                    m_strvec.append(s[i]);
                }
            }
            ~SVStringArray() {
                m_strvec.destroy();
            }
            
            void setData(cptr8 _data,char delimiters) {
                m_strvec.destroy();
                //
                cptr8 t_pData = _data;
                s32 t_num = 0;
                cptr8 t_lastPoint = nullptr;
                while(*t_pData!='\0') {
                    cptr8 t_pointer = strchr(t_pData,delimiters);
                    if(t_pointer) {
                        //找到目标字符
                        s32 t_size = t_pointer - t_pData;
                        SVString t_str;
                        t_str.append(t_pData,t_size);
                        t_pData = t_pointer;
                        t_pData += sizeof(delimiters);
                        m_strvec.append(t_str);
                    }else{
                        if(!t_lastPoint){
                            t_lastPoint = t_pData;
                        }
                        //最后一组词
                        t_pData++;
                        t_num++;
                    }
                }
                //
                SVString t_str;
                t_str.append(t_lastPoint,t_num);
                m_strvec.append(t_str);
            }
            
            sv_inline const c8 *operator[](s32 index) const {
                assert((u32)index < (u32)m_strvec.size() && "SVStringArray::operator[](): bad index");
                return m_strvec[index].c_str();
            }
            
            sv_inline s32 size() const {
                return m_strvec.size();
            }
            
            sv_inline s32 empty() const {
                return (m_strvec.size() == 0);
            }
            
        private:
            SVArray<SVString> m_strvec;
        };
    
        //
        SVStringStack<> operator+(const SVString &s0,const SVString &s1);
        SVStringStack<> operator+(const SVString &s0,cptr8 s1);
        SVStringStack<> operator+(cptr8 s0,const SVString &s1);
        SVStringStack<> operator+(const SVString &s0,c8 c1);
        SVStringStack<> operator+(c8 c0,const SVString &s1);
        
        s32 operator==(const SVString &s0,const SVString &s1);
        s32 operator==(const SVString &s0,cptr8 s1);
        s32 operator==(cptr8 s0,const SVString &s1);
        
        s32 operator!=(const SVString &s0,const SVString &s1);
        s32 operator!=(const SVString &s0,cptr8 s1);
        s32 operator!=(cptr8 s0,const SVString &s1);
        
        s32 operator<(const SVString &s0,const SVString &s1);
        s32 operator<(const SVString &s0,cptr8 s1);
        s32 operator<(cptr8 s0,const SVString &s1);
        
        s32 operator>(const SVString &s0,const SVString &s1);
        s32 operator>(const SVString &s0,cptr8 s1);
        s32 operator>(cptr8 s0,const SVString &s1);
        
        s32 operator<=(const SVString &s0,const SVString &s1);
        s32 operator<=(const SVString &s0,cptr8 s1);
        s32 operator<=(cptr8 s0,const SVString &s1);
        
        s32 operator>=(const SVString &s0,const SVString &s1);
        s32 operator>=(const SVString &s0,cptr8 s1);
        s32 operator>=(cptr8 s0,const SVString &s1);
            
    }//!namespace util
    
}//!namespace sv



#endif /* __STRING_H__ */
