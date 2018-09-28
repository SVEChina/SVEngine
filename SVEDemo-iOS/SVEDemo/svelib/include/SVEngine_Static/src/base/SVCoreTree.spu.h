#ifndef __TREE_SPU_H__
#define __TREE_SPU_H__

#include "SVBase.spu.h"

namespace sv {
    
    namespace util {
        
        template <class Key,class Data,u16 Capacity> class SVCoreTree {
            
        public:
            
            // SVCoreTree::Iterator
            class Iterator;
            
            // SVCoreTree::Node
            class Node : public Data {
                
            private:
                
                friend class SVCoreTree<Key,Data,Capacity>;
                friend class SVCoreTree<Key,Data,Capacity>::Iterator;
                
                Node() { }
                ~Node() { }
                
                Node *parent;
                Node *left;
                Node *right;
                short balance;
                
            public:
                
                Key key;
            };
            
            // Tree::Iterator
            class Iterator {
                
            private:
                
                friend class SVCoreTree<Key,Data,Capacity>;
                
                Iterator(Node *node)
                : node(node) {

                }
                
            public:
                
                Iterator()
                : node(0) {
                    
                }
                
                Iterator(const Iterator &it)
                : node(it.node) {
                    
                }
                
                ~Iterator() {
                    
                }
                
                Iterator &operator=(const Iterator &it) {
                    node = it.node;
                    return *this;
                }
                
                sv_inline int operator==(const Iterator &it) const { return node == it.node; }
                sv_inline int operator!=(const Iterator &it) const { return node != it.node; }
                
                sv_inline Node &operator*() { return *node; }
                sv_inline const Node &operator*() const { return *node; }
                
                sv_inline Node *operator->() { return node; }
                sv_inline const Node *operator->() const { return node; }
                
                sv_inline Node *get() { return node; }
                sv_inline const Node *get() const { return node; }
                
                sv_inline Iterator &operator++() {
                    next();
                    return *this;
                }
                sv_inline Iterator operator++(int) {
                    Iterator temp = *this;
                    next();
                    return temp;
                }
                
            private:
                
                void next() {
                    if(node->right != 0) {
                        node = node->right;
                        while(node->left != 0) node = node->left;
                    } else {
                        Node *p = node->parent;
                        while(p != 0 && node == p->right) { node = p; p = p->parent; }
                        if(p == 0 || node->right != p) node = p;
                    }
                }
                
                Node *node;
            };
            
            // Tree
            SVCoreTree() {
                clear();
            }
            
            ~SVCoreTree() {
                deallocate(root);
            }
            
            sv_inline Iterator begin() const {
                if(root) {
                    Node *node = root;
                    while(node->left) node = node->left;
                    return Iterator(node);
                }
                return Iterator(0);
            }
            
            sv_inline Iterator end() const {
                return Iterator(0);
            }
            
            void clear() {
                root = 0;
                num_free_nodes = Capacity;
                for(u16 i = 0; i < num_free_nodes; i++) {
                    free_nodes[i] = num_free_nodes - i - 1;
                }
            }
            
            template <class T> Iterator find(const T &key) const {
                Node *node = root;
                while(node && (node->key == key) == 0) {
                    node = (key < node->key) ? node->left : node->right;
                }
                return Iterator(node);
            }
            
            sv_inline void remove(const Key &key) {
                short change = 0;
                Node *node = remove_proc(key,root,change);
                deallocate(node);
            }
            sv_inline void remove(const Iterator &it) {
                remove(it->key);
            }
            
        protected:
            
            Node *allocate(const Key &key) {
                assert(num_free_nodes > 0 && "Tree::allocate(): can't allocate node");
                Node *node = &nodes[free_nodes[--num_free_nodes]];
                node->parent = 0;
                node->left = 0;
                node->right = 0;
                node->balance = 0;
                node->key = key;
                return node;
            }
            
            void deallocate(Node *node) {
                if(node == 0) return;
                deallocate(node->left);
                deallocate(node->right);
                u16 index = (u16)(nodes - node);
                assert(index < Capacity && "Tree::deallocate(): bad node pointer");
                free_nodes[num_free_nodes++] = index;
            }
            
            Node *append_proc(const Key &key,Node *&root,Node *&parent,short &change) {
                if(root == 0) {
                    root = allocate(key);
                    root->parent = parent;
                    change = 1;
                    return root;
                }
                Node *ret = 0;
                short increase = 0;
                if(key < root->key) {
                    ret = append_proc(key,root->left,root,change);
                    increase = -change;
                } else if(root->key < key) {
                    ret = append_proc(key,root->right,root,change);
                    increase = change;
                } else {
                    return root;
                }
                root->balance += increase;
                if(increase && root->balance) change = 1 - balance(root);
                else change = 0;
                return ret;
            }
            
            Node *remove_proc(const Key &key,Node *&root,short &change) {
                if(root == 0) {
                    change = 0;
                    return 0;
                }
                Node *ret = 0;
                short decrease = 0;
                if(key < root->key) {
                    ret = remove_proc(key,root->left,change);
                    if(ret == 0) return ret;
                    decrease = -change;
                } else if(root->key < key) {
                    ret = remove_proc(key,root->right,change);
                    if(ret == 0) return ret;
                    decrease = change;
                } else {
                    ret = root;
                    if(root->left == 0 && root->right == 0) {
                        change = 1;
                        root = 0;
                        return ret;
                    } else if(root->left == 0 || root->right == 0) {
                        change = 1;
                        if(root->left) root = root->left;
                        else root = root->right;
                        root->parent = ret->parent;
                        ret->left = 0;
                        ret->right = 0;
                        return ret;
                    } else {
                        ret = remove_proc(root->right,decrease);
                        root->key = ret->key;
                        const Data *src = static_cast<const Data*>(ret);
                        Data *dest = static_cast<Data*>(root);
                        *dest = *src;
                    }
                }
                root->balance -= decrease;
                if(decrease && root->balance) change = balance(root);
                else if(decrease) change = 1;
                else change = 0;
                return ret;
            }
            
            Node *remove_proc(Node *&root,short &change) {
                Node *ret = 0;
                short decrease = 0;
                if(root->left != 0) {
                    ret = remove_proc(root->left,change);
                    if(ret == 0) return ret;
                    decrease = -change;
                } else {
                    ret = root;
                    if(root->right == 0) {
                        change = 1;
                        root = 0;
                        return ret;
                    } else {
                        change = 1;
                        root = root->right;
                        root->parent = ret->parent;
                        ret->left = 0;
                        ret->right = 0;
                        return ret;
                    }
                }
                root->balance -= decrease;
                if(decrease && root->balance) change = balance(root);
                else if(decrease) change = 1;
                else change = 0;
                return ret;
            }
            
            short rotate_left_once(Node *&root) {
                short change = (root->right->balance != 0);
                Node *old_root = root;
                root = old_root->right;
                old_root->right = root->left;
                root->left = old_root;
                root->parent = old_root->parent;
                old_root->parent = root;
                if(old_root->right) {
                    old_root->right->parent = old_root;
                }
                old_root->balance = -(--root->balance);
                return change;
            }
            
            short rotate_right_once(Node *&root) {
                short change = (root->left->balance != 0);
                Node *old_root = root;
                root = old_root->left;
                old_root->left = root->right;
                root->right = old_root;
                root->parent = old_root->parent;
                old_root->parent = root;
                if(old_root->left) {
                    old_root->left->parent = old_root;
                }
                old_root->balance = -(++root->balance);
                return change;
            }
            
            short rotate_left_twice(Node *&root) {
                Node *old_root = root;
                Node *old_root_right = root->right;
                root = old_root->right->left;
                old_root->right = root->left;
                root->left = old_root;
                old_root_right->left = root->right;
                root->right = old_root_right;
                root->parent = old_root->parent;
                old_root->parent = root;
                old_root_right->parent = root;
                if(old_root->right) {
                    old_root->right->parent = old_root;
                }
                if(old_root_right->left) {
                    old_root_right->left->parent = old_root_right;
                }
                root->left->balance = -((root->balance > 0) ? root->balance : 0);
                root->right->balance = -((root->balance < 0) ? root->balance : 0);
                root->balance = 0;
                return 1;
            }
            
            short rotate_right_twice(Node *&root) {
                Node *old_root = root;
                Node *old_root_left = root->left;
                root = old_root->left->right;
                old_root->left = root->right;
                root->right = old_root;
                old_root_left->right = root->left;
                root->left = old_root_left;
                root->parent = old_root->parent;
                old_root->parent = root;
                old_root_left->parent = root;
                if(old_root->left) {
                    old_root->left->parent = old_root;
                }
                if(old_root_left->right) {
                    old_root_left->right->parent = old_root_left;
                }
                root->left->balance = -((root->balance > 0) ? root->balance : 0);
                root->right->balance = -((root->balance < 0) ? root->balance : 0);
                root->balance = 0;
                return 1;
            }
            
            short balance(Node *&root) {
                if(root->balance < -1) {
                    if(root->left->balance == 1)
                        return rotate_right_twice(root);
                    return rotate_right_once(root);
                } else if(root->balance > 1) {
                    if(root->right->balance == -1)
                        return rotate_left_twice(root);
                    return rotate_left_once(root);
                }
                return 0;
            }
            
            Node *root;
            Node nodes[Capacity];
            u16 num_free_nodes;
            u16 free_nodes[Capacity];
        };

        
    }//!namespace util
    
}//!namespace sv


#endif /* __TREE_SPU_H__ */
