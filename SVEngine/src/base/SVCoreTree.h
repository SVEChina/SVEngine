#ifndef __TREE_H__
#define __TREE_H__

#include "SVBase.h"

namespace sv {
    
    namespace util {
        
        template <class Key,class Data> class SVCoreTree {
        public:
            class Iterator;
            class Node : public Data, public Base {
            private:
                friend class SVCoreTree<Key,Data>;
                friend class SVCoreTree<Key,Data>::Iterator;

                Node(const Key &key)
                : parent(0)
                , left(0)
                , right(0)
                , balance(0)
                , key(key) {
                }
                
                ~Node() {
                    delete left;
                    delete right;
                }
                
                Node *parent;
                Node *left;
                Node *right;
                s32 balance;
            public:
                const Key key;
            };
            
            //SVTree Iterator
            class Iterator {
            private:
                friend class SVCoreTree<Key,Data>;
                Iterator(Node *node)
                : node(node) {
                }
                
            public:
                typedef Key KeyType;
                typedef Data DataType;
                
                Iterator()
                : node(0) {
                }
                
                Iterator(const Iterator &it)
                : node(it.node) {
                }
                
                ~Iterator() {
                }
                
                sv_inline Iterator &operator=(const Iterator &it) {
                    node = it.node;
                    return *this;
                }
                
                sv_inline s32 operator==(const Iterator &it) const {
                    return node == it.node;
                }
                
                sv_inline s32 operator!=(const Iterator &it) const {
                    return node != it.node;
                }
                
                sv_inline Node &operator*() {
                    return *node;
                }
                
                sv_inline const Node &operator*() const {
                    return *node;
                }
                
                sv_inline Node *operator->() {
                    return node;
                }
                
                sv_inline const Node *operator->() const {
                    return node;
                }
                
                sv_inline Node *get() {
                    return node;
                }
                
                sv_inline const Node *get() const {
                    return node;
                }
                
                sv_inline Iterator &operator++() {
                    next();
                    return *this;
                }
                
                sv_inline Iterator &operator--() {
                    prev();
                    return *this;
                }
                
                sv_inline Iterator operator++(s32) {
                    Iterator it = *this;
                    next();
                    return it;
                }
                
                sv_inline Iterator operator--(s32) {
                    Iterator it = *this;
                    prev();
                    return it;
                }
                
            private:
                
                void next() {
                    if(node->right != 0) {
                        node = node->right;
                        while(node->left != 0) {
                            node = node->left;
                        }
                    } else {
                        Node *p = node->parent;
                        while(p != 0 && node == p->right) {
                            node = p;
                            p = p->parent;
                        }
                        if(p == 0 || node->right != p) {
                            node = p;
                        }
                    }
                }
                
                void prev() {
                    if(node->left != 0) {
                        node = node->left;
                        while(node->right != 0) {
                            node = node->right;
                        }
                    } else {
                        Node *p = node->parent;
                        while(p != 0 && node == p->left) {
                            node = p; p = p->parent;
                        }
                        if(p == 0 || node->left != p) {
                            node = p;
                        }
                    }
                }
                
                Node *node;
            };
            
            //SVCoreTree
            SVCoreTree()
            : length(0)
            , root(0) {
            }
            
            SVCoreTree(const SVCoreTree &tree)
            : length(0)
            , root(0) {
                Node *dest_parent = 0;
                copy_proc(root,dest_parent,tree.root);
            }
            
            ~SVCoreTree() {
                delete root;
            }
            
            SVCoreTree &operator=(const SVCoreTree &tree) {
                if(this == &tree)
                    return *this;
                length = 0;
                delete root;
                root = 0;
                Node *dest_parent = 0;
                copy_proc(root,dest_parent,tree.root);
                return *this;
            }
            
            void swap(SVCoreTree &tree) {
                if(this == &tree)
                    return;
                s32 i = length;
                length = tree.length;
                tree.length = i;
                Node *n = root;
                root = tree.root;
                tree.root = n;
            }
            
            sv_inline Iterator begin() const {
                if(root) {
                    Node *node = root;
                    while(node->left)
                        node = node->left;
                    return Iterator(node);
                }
                return Iterator(0);
            }
            
            sv_inline Iterator back() const {
                if(root) {
                    Node *node = root;
                    while(node->right) {
                        node = node->right;
                    }
                    return Iterator(node);
                }
                return Iterator(0);
            }
            
            sv_inline Iterator end() const {
                return Iterator(0);
            }
            
            sv_inline s32 size() const {
                return length;
            }
            
            sv_inline s32 empty() const {
                return (length == 0);
            }
            
            void clear() {
                length = 0;
                delete root;
                root = 0;
            }
            
            template <class T> Iterator find(const T &key) const {
                Node *node = root;
                while(node) {
                    if(node->key == key){
                        break;
                    }
                    node = (key < node->key) ? node->left : node->right;
                }
                return Iterator(node);
            }
            
            sv_inline void remove(const Key &key) {
                s32 change = 0;
                Node *node = remove_proc(key,root,change);
                delete node;
            }
            
            sv_inline void remove(const Iterator &it) {
                remove(it->key);
            }
            
        protected:
            void copy_proc(Node *&dest_root,Node *&dest_parent,const Node *src_root) {
                if(src_root == 0)
                    return;
                length++;
                dest_root = new Node(src_root->key);
                dest_root->parent = dest_parent;
                dest_root->balance = src_root->balance;
                const Data *src = static_cast<const Data*>(src_root);
                Data *dest = static_cast<Data*>(dest_root);
                *dest = *src;
                copy_proc(dest_root->left,dest_root,src_root->left);
                copy_proc(dest_root->right,dest_root,src_root->right);
            }
            
            Node *append_proc(const Key &key,Node *&root,Node *&parent,s32 &change) {
                if(root == 0) {
                    length++;
                    root = new Node(key);
                    root->parent = parent;
                    change = 1;
                    return root;
                }
                Node *ret = 0;
                s32 increase = 0;
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
                if(increase && root->balance) {
                    change = 1 - balance(root);
                } else {
                    change = 0;
                }
                return ret;
            }
            
            Node *remove_proc(const Key &key,Node *&root,s32 &change) {
                if(root == 0) {
                    change = 0;
                    return 0;
                }
                Node *ret = 0;
                s32 decrease = 0;
                if(key < root->key) {
                    ret = remove_proc(key,root->left,change);
                    if(ret == 0)
                        return ret;
                    decrease = -change;
                } else if(root->key < key) {
                    ret = remove_proc(key,root->right,change);
                    if(ret == 0)
                        return ret;
                    decrease = change;
                } else {
                    length--;
                    ret = root;
                    if(root->left == 0 && root->right == 0) {
                        change = 1;
                        root = 0;
                        return ret;
                    } else if(root->left == 0 || root->right == 0) {
                        change = 1;
                        if(root->left) {
                            root = root->left;
                        } else {
                            root = root->right;
                        }
                        root->parent = ret->parent;
                        ret->left = 0;
                        ret->right = 0;
                        return ret;
                    } else {
                        ret = remove_proc(root->right,decrease);
                        const_cast<Key&>(root->key) = ret->key;
                        const Data *src = static_cast<const Data*>(ret);
                        Data *dest = static_cast<Data*>(root);
                        *dest = *src;
                    }
                }
                root->balance -= decrease;
                if(decrease && root->balance) {
                    change = balance(root);
                } else if(decrease) {
                    change = 1;
                } else {
                    change = 0;
                }
                return ret;
            }
            
            Node *remove_proc(Node *&root,s32 &change) {
                Node *ret = 0;
                s32 decrease = 0;
                if(root->left != 0) {
                    ret = remove_proc(root->left,change);
                    if(ret == 0)
                        return ret;
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
                if(decrease && root->balance) {
                    change = balance(root);
                } else if(decrease){
                    change = 1;
                } else {
                    change = 0;
                }
                return ret;
            }
            
            s32 rotate_left_once(Node *&root) {
                s32 change = (root->right->balance != 0);
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
            
            s32 rotate_right_once(Node *&root) {
                s32 change = (root->left->balance != 0);
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
            
            s32 rotate_left_twice(Node *&root) {
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
            
            s32 rotate_right_twice(Node *&root) {
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
            
            s32 balance(Node *&root) {
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
            
            s32 length;
            Node *root;
        };

        
    }//!namespace util
    
}//!namespace sv



#endif /* __TREE_H__ */
