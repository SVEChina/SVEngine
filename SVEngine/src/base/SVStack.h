// SVStack.h
// SVEngine
// Copyright 2017-2020
// yizhou Fu,long Yin,longfei Lin,ziyu Xu,xiaofan Li,daming Li
//

#ifndef SV_STACK_H
#define SV_STACK_H

#include "SVBase.h"

namespace sv {
    
    namespace util {
        
        template <class Type,s32 Capacity> class SVStack {
        public:
            SVStack()
            :m_depth(0) {
                m_stack = new Type[Capacity];
            }
            
            ~SVStack() {
                destroy();
            }
            
            sv_inline Type &get(s32 index) {
                assert((u32)index < (u32)m_depth && "Stack::get(): bad index");
                return m_stack[m_depth - 1 - index];
            }
            
            sv_inline const Type &get(s32 index) const {
                assert((u32)index < (u32)m_depth && "Stack::get(): bad index");
                return m_stack[m_depth - 1 - index];
            }
            
            sv_inline s32 size() const {
                return m_depth;
            }
            
            sv_inline s32 empty() const {
                return (m_depth == 0);
            }
            
            sv_inline s32 capacity() const {
                return Capacity;
            }
            
            sv_inline void append(s32 num) {
                assert(m_depth + num < Capacity && "Stack::append(): stack overflow");
                m_depth += num;
            }
            
            sv_inline void remove(s32 num) {
                assert(m_depth - num >= 0 && "Stack::remove(): stack underflow");
                m_depth -= num;
            }
            
            sv_inline void clear() {
                m_depth = 0;
            }
            
            void destroy() {
                m_depth = 0;
                delete [] m_stack;
                m_stack = 0;
            }
            
            sv_inline Type &top() {
                assert(m_depth > 0 && "Stack::top(): stack underflow");
                return m_stack[m_depth - 1];
            }
            
            sv_inline const Type &top() const {
                assert(m_depth > 0 && "Stack::top(): stack underflow");
                return m_stack[m_depth - 1];
            }
            
            sv_inline Type &pop() {
                assert(m_depth > 0 && "Stack::pop(): stack underflow");
                return m_stack[--m_depth];
            }
            
            sv_inline void push(const Type &t) {
                assert(m_depth < Capacity && "Stack::push(): stack overflow");
                m_stack[m_depth++] = t;
            }
            
            sv_inline Type &push() {
                assert(m_depth < Capacity && "Stack::push(): stack overflow");
                return m_stack[m_depth++];
            }
            
        private:
            s32 m_depth;
            Type *m_stack;
        };
        
    }//!namespace util
    
}//!namespace sv



#endif /* SV_STACK_H */
