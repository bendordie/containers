/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cshells <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/11 19:59:44 by cshells           #+#    #+#             */
/*   Updated: 2021/10/11 19:59:45 by cshells          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef __FT_MAP_H__
#define __FT_MAP_H__

#include <memory>
#include "Utils.hpp"
#include <iostream>
#include "Iterator.hpp"

namespace ft {

    template <class Key, class T, class Compare = std::less<Key>,
            class Allocator = std::allocator<ft::Pair<const Key, T> > >
    class Map {

    public:

        typedef Key                                                                     key_type;
        typedef T                                                                       mapped_type;
        typedef ft::Pair<const key_type, mapped_type>                                   value_type;
        typedef Node<value_type>                                                        node_type;
        typedef std::allocator_traits<Allocator>                                        alloc_traits;
        typedef Compare                                                                 key_compare;
        typedef Allocator                                                               allocator_type;
        typedef value_type&                                                             reference;
        typedef const value_type&                                                       const_reference;
        typedef NodeIterator<node_type*, value_type>                                    iterator;
        typedef NodeIterator<const node_type*, value_type>                              const_iterator;
        typedef ft::reverse_iterator<iterator>                                          reverse_iterator;
        typedef ft::reverse_iterator<const_iterator>                                    const_reverse_iterator;
        typedef typename allocator_type::template rebind<Node<value_type> >::other      allocator_rebind_node;
        /****************************************************************************************************/
        typedef typename alloc_traits::pointer                                          pointer;
        typedef typename alloc_traits::const_pointer                                    const_pointer;
        typedef typename alloc_traits::size_type                                        size_type;
        typedef typename alloc_traits::difference_type                                  difference_type;


        class value_compare : std::binary_function<value_type, value_type, bool>
        {
            friend class Map;
        protected:
            key_compare comp;

            value_compare() {}
            value_compare(key_compare c) : comp(c) {}

        public:
            bool operator()(const value_type& x, const value_type& y) {
                return comp(x.first, y.first);
            }
        };

    private:

        node_type*               _root;
        node_type                _nil;
        size_type                _size;
        key_compare              _comp;
        allocator_type           _allocator;
        allocator_rebind_node    _allocator_node;

    public:


        /****************************************************************************************************/
        /****************************************   CONSTRUCTORS   ******************************************/
        /****************************************************************************************************/

        explicit Map(const key_compare& comp = key_compare(),
                     const allocator_type& alloc = allocator_type()) : _nil(), _comp(comp), _allocator(alloc) {

            _root = &_nil;
            _size = 0;
        }

        template <class InputIterator>
        Map(InputIterator first, InputIterator last,
            const key_compare& comp = key_compare(),
            const allocator_type& alloc = allocator_type()) : _nil(), _comp(comp), _allocator(alloc) {

            _root = &_nil;
            _comp = comp;
            _size = 0;
            for (; first != last; ++first) {
                insert(ft::make_pair(first->first, first->second));
            }
        }

        Map(const Map& x) : _nil() {
            _root = &_nil;
            _comp = key_compare();
            _size = 0;
            for (const_iterator it = x.begin(); it != x.end(); ++it) {
                insert(ft::make_pair(it->first, it->second));
            }
//            fillTree(x._root);
        }

        /****************************************************************************************************/
        /*****************************************   ASSIGNMENT   *******************************************/
        /****************************************************************************************************/

        Map& operator=(const Map& x) {
            if (this != &x) {
                deleteTree(_root);
                _root = &_nil;
                _size = 0;
                for (const_iterator it = x.begin(); it != x.end(); ++it) {
                    insert(ft::make_pair(it->first, it->second));
                }
//                fillTree(x._root);
            }
            return (*this);
        }

        /****************************************************************************************************/
        /*****************************************   DESTRUCTOR   *******************************************/
        /****************************************************************************************************/

        ~Map() {
            deleteTree(_root);
        }

        /****************************************************************************************************/
        /*****************************************   ITERATORS   ********************************************/
        /****************************************************************************************************/

        iterator begin() {

            node_type *tmp = _root;

            while (!tmp->left->nil) {
                tmp = tmp->left;
            }
            return iterator(tmp);
        }

        const_iterator begin() const {

            node_type *tmp = _root;

            while (!tmp->left->nil) {
                tmp = tmp->left;
            }
            return const_iterator(tmp);
        }

        iterator end() {

            node_type *tmp = _root;

            while (!tmp->right->nil) {
                tmp = tmp->right;
            }
            return iterator(tmp->right);
        }

        const_iterator end() const {

            node_type *tmp = _root;

            while (!tmp->right->nil) {
                tmp = tmp->right;
            }
            return const_iterator(tmp->right);
        }

        reverse_iterator rbegin() {
            return reverse_iterator(--end());
        }

        const_reverse_iterator rbegin() const {
            return const_reverse_iterator(--end());
        }

        reverse_iterator rend() {
            return reverse_iterator(iterator(&_nil));
        }

        const_reverse_iterator rend() const {
            return const_reverse_iterator(iterator(&_nil));
        }

        /****************************************************************************************************/
        /******************************************   CAPACITY   ********************************************/
        /****************************************************************************************************/

        bool empty() const {
            return _root->nil;
        }

        size_type size() const {
            return _size;
        }

        size_type  max_size() const {
            return std::numeric_limits<size_type>::max() / sizeof(value_type) + sizeof(node_type);
        };

        /****************************************************************************************************/
        /***************************************   ELEMENT ACCESS   *****************************************/
        /****************************************************************************************************/

        mapped_type& operator[](const key_type& k) {
           return (*((this->insert(ft::make_pair(k,mapped_type()))).first)).second;
        }

        mapped_type& at(const key_type& k) {

            iterator tmp = find(k);
            return (tmp == this->end() ? throw std::out_of_range("key not found") : tmp->second);
        }

        /****************************************************************************************************/
        /*****************************************   MODIFIERS   ********************************************/
        /****************************************************************************************************/

        Pair<iterator, bool> insert(const value_type& val) {
            return insertNode(iterator(_root), val);
        }

        iterator insert(iterator position, const value_type& val) {

            if (position == end()) {
                return insertNode(iterator(_root), val).first;
            }
            else {
                if (position->first > val.first)
                {
                    iterator prev = position;
                    --prev;
                    while (prev != end() && prev->first >= val.first){
                        --position;
                        --prev;
                    }
                } else if (position->first < val.first) {
                    iterator next = position;
                    ++next;
                    while (next != end() && next->first <= val.first) {
                        ++position;
                        ++next;
                    }
                }
            }
            return insertNode(position, val).first;
        }

        template <class InputIterator>
        void insert(InputIterator first, InputIterator last) {

            for (; first != last && first != end(); ++first) {
                insert(ft::make_pair(first->first, first->second));
            }
        }

        void erase(iterator position) {

            if (!(find(position->first) == end()))
                deleteNode(position.base());
        }

        void erase(iterator first, iterator last) {

            iterator tmp;

            for (; first != last && first != end();) {
                tmp = first++;
                deleteNode(tmp.base());
            }
        }

        size_type erase(const key_type& k) {

            iterator tmp = find(k);

            if (!(tmp == end())) {
                deleteNode(tmp.base());
                return (1);
            }
            return (0);
        }

        void clear() {
            if (_root != &_nil)
                deleteTree(_root);
            _root = &_nil;
            _size = 0;
        }

        void swap(Map& x) {

            if (this != &x) {
                std::swap(*this, x);
            }
        }

        /****************************************************************************************************/
        /*****************************************   OBSERVERS   ********************************************/
        /****************************************************************************************************/

        key_compare key_comp() const {
            return _comp;
        }

        value_compare value_comp() const {
            return value_compare(_comp);
        }

        /****************************************************************************************************/
        /*****************************************   OPERATIONS   *******************************************/
        /****************************************************************************************************/

        iterator find(const key_type& k) {

            node_type *current = _root;

            while(current != &_nil)
                if(k == current->data->first)
                    return (iterator(current));
                else
                    current = _comp(k, current->data->first) ?
                              current->left : current->right;
            return (end());
        }

        const_iterator find(const key_type& k) const {

            node_type *current = _root;
            while(current != &_nil)
                if(k == current->data->first)
                    return (const_iterator(current));
                else
                    current = _comp(k, current->data->first) ?
                            current->left : current->right;
                return (end());
        }

        size_type count(const key_type& k) const {
            return ((find(k) == end()) ? 0 : 1);
        }

        iterator lower_bound(const key_type& k) {
            node_type *current = _root;
            node_type *parent;
            bool      left;

            while(current != &_nil)
                if(k == current->data->first)
                    return (iterator(current));
                else {
                    parent = current;
                    left = _comp(k, current->data->first) ?
                            1 : 0;
                    current = _comp(k, current->data->first) ?
                            current->left : current->right;
                }
                if (left)
                    return iterator(parent);
                else
                    return ++iterator(parent);
        }

        const_iterator lower_bound(const key_type& k) const {
            node_type *current = _root;
            node_type *parent;
            bool      left;

            while(current != &_nil)
                if(k == current->data->first)
                    return (iterator(current));
                else {
                    parent = current;
                    left = _comp(k, current->data->first) ?
                            1 : 0;
                    current = _comp(k, current->data->first) ?
                            current->left : current->right;
                }
                if (left)
                    return const_iterator(parent);
                else
                    return ++const_iterator(parent);
        }

        iterator upper_bound(const key_type& k) {

            iterator res = lower_bound(k);

            if (res == end())
                return end();
            return res->first == k ? ++res : res ;
        }

        const_iterator upper_bound(const key_type& k) const {

            const_iterator res = lower_bound(k);

            if (res == end())
                return end();
            return res->first == k ? ++res : res ;
        }

        /****************************************************************************************************/
        /*****************************************   ALLOCATOR   ********************************************/
        /****************************************************************************************************/

        allocator_type get_allocator() const {
            return _allocator;
        }

        Pair<iterator, iterator> equal_range(const key_type& k) {
            return ft::make_pair(lower_bound(k), upper_bound(k));
        };

        Pair<const_iterator, const_iterator> equal_range(const key_type& k) const {
            return ft::make_pair(lower_bound(k), upper_bound(k));
        };

        /****************************************************************************************************/
        /**************************************   PRIVATE METHODS   *****************************************/
        /****************************************************************************************************/

    private:

        node_type* get_last() {
            node_type *tmp(_root);

            while (!tmp->right->nil) {
                tmp = tmp->right;
            }
            return tmp;
        }

        void rotateLeft(node_type *x) {

            node_type *y = x->right;

            x->right = y->left;
            if (y->left != &_nil) y->left->parent = x;
            if (y != &_nil) y->parent = x->parent;
            if (x->parent) {
                if (x == x->parent->left)
                    x->parent->left = y;
                else
                    x->parent->right = y;
            } else {
                _root = y;
            }
            y->left = x;
            if (x != &_nil) x->parent = y;
        }

        void rotateRight(node_type *x) {

            node_type *y = x->left;

            x->left = y->right;
            if (y->right != &_nil) y->right->parent = x;
            if (y != &_nil) y->parent = x->parent;
            if (x->parent) {
                if (x == x->parent->right)
                    x->parent->right = y;
                else
                    x->parent->left = y;
            } else {
                _root = y;
            }

            y->right = x;
            if (x != &_nil) x->parent = y;
        }

        ft::Pair<iterator, bool> insertNode(iterator pos, const value_type& val) {

            node_type *current = 0;
            node_type *parent = 0;
            node_type *x = 0;



            current = pos.base();
            parent = 0;
            while (current != &_nil) {
                if (val.first == current->data->first)
                    return (ft::make_pair(iterator(current), false));
                parent = current;
                current = _comp(val.first, current->data->first) ?
                        current->left : current->right;
            }
            x = _allocator_node.allocate(sizeof(Node<value_type>));
            _allocator_node.construct(x, val);
            x->parent = parent;
            x->left = &_nil;
            x->right = &_nil;
            x->red = 1;

            if(parent) {
                if(_comp(val.first, parent->data->first))
                    parent->left = x;
                else
                    parent->right = x;
            } else {
                _root = x;
            }
            insertFixup(x);
            if (x == get_last()) {
                _nil.parent = x;
            }
            if (x == begin().base()) {
                _nil.begin = x;
            }
            _size++;
            return(ft::make_pair(iterator(x), true));
        }

        void insertFixup(node_type *x) {

            while (x != _root && x->parent->red) {
                if (x->parent == x->parent->parent->left) {
                    node_type *y = x->parent->parent->right;
                    if (y->red) {
                        x->parent->red = 0;
                        y->red = 0;
                        x->parent->parent->red = 1;
                        x = x->parent->parent;
                    } else {
                        if (x == x->parent->right) {
                            x = x->parent;
                            rotateLeft(x);
                        }
                        x->parent->red = 0;
                        x->parent->parent->red = 1;
                        rotateRight(x->parent->parent);
                    }
                } else {

                    node_type *y = x->parent->parent->left;
                    if (y->red) {
                        x->parent->red = 0;
                        y->red = 0;
                        x->parent->parent->red = 1;
                        x = x->parent->parent;
                    } else {
                        if (x == x->parent->left) {
                            x = x->parent;
                            rotateRight(x);
                        }
                        x->parent->red = 0;
                        x->parent->parent->red = 1;
                        rotateLeft(x->parent->parent);
                    }
                }
            }
            _root->red = 0;
        }

        void deleteFixup(node_type *x) {

            while (x != _root && x->red == 0) {
                if (x == x->parent->left) {
                    node_type *w = x->parent->right;
                    if (w->red) {
                        w->red = 0;
                        x->parent->red = 1;
                        rotateLeft (x->parent);
                        w = x->parent->right;
                    }
                    if (w->left->red == 0 && w->right->red == 0) {
                        w->red = 1;
                        x = x->parent;
                    } else {
                        if (w->right->red == 0) {
                            w->left->red = 0;
                            w->red = 1;
                            rotateRight (w);
                            w = x->parent->right;
                        }
                        w->red = x->parent->red;
                        x->parent->red = 0;
                        w->right->red = 0;
                        rotateLeft (x->parent);
                        x = _root;
                    }
                } else {
                    node_type *w = x->parent->left;
                    if (w->red) {
                        w->red = 0;
                        x->parent->red = 1;
                        rotateRight (x->parent);
                        w = x->parent->left;
                    }
                    if (w->right->red == 0 && w->left->red == 0) {
                        w->red = 1;
                        x = x->parent;
                    } else {
                        if (w->left->red == 0) {
                            w->right->red = 0;
                            w->red = 1;
                            rotateLeft (w);
                            w = x->parent->left;
                        }
                        w->red = x->parent->red;
                        x->parent->red = 0;
                        w->left->red = 0;
                        rotateRight (x->parent);
                        x = _root;
                    }
                }
            }
            x->red = 0;
        }

        void deleteNode(node_type *z) {

            iterator    tmp;
            node_type   *x, *y;

            if (!z || z == &_nil)
                return ;

            if (z->left == &_nil || z->right == &_nil) {
                y = z;
            } else {
                y = z->right;
                while (y->left != &_nil) y = y->left;
            }

            if (y->left != &_nil)
                x = y->left;
            else
                x = y->right;
            x->parent = y->parent;
            if (y->parent)
                if (y == y->parent->left)
                    y->parent->left = x;
                else
                    y->parent->right = x;
                else
                    _root = x;
                if (y != z) {
                    _allocator.destroy(z->data);
                    _allocator.deallocate(z->data, sizeof(mapped_type));
                    z->data = _allocator.allocate(sizeof(mapped_type));
                    _allocator.construct(z->data, *y->data);
                }
                if (y->red == 0)
                    deleteFixup (x);
                _nil.parent = get_last();
                _nil.begin = begin().base();
                _size--;
                delete y;
        }

        void deleteTree(node_type *t) {

            if (t->nil)
                return ;
            if (!t->left->nil)
                deleteTree(t->left);
            if (!t->right->nil)
                deleteTree(t->right);
            _allocator_node.destroy(t);
            _allocator_node.deallocate(t, sizeof(node_type));
        }

        void fillTree(node_type *t) {
            if (!t->left->nil)
                fillTree(t->left);
            if (!t->nil)
                this->insert(*t->data);
            if (!t->right->nil)
                fillTree(t->right);
        }

    };

        /****************************************************************************************************/
        /***************************************   RELATIONAL OPS   *****************************************/
        /****************************************************************************************************/

template <class Key, class T, class Compare, class Allocator>
inline
bool operator==(const Map<Key, T, Compare, Allocator>& x,
                const Map<Key, T, Compare, Allocator>& y) {
    return x.size() == y.size() && ft::equal(x.begin(), x.end(), y.begin());
}

template <class Key, class T, class Compare, class Allocator>
inline
bool operator <(const Map<Key, T, Compare, Allocator>& x,
        const Map<Key, T, Compare, Allocator>& y) {
return ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

template <class Key, class T, class Compare, class Allocator>
inline
bool operator!=(const Map<Key, T, Compare, Allocator>& x,
        const Map<Key, T, Compare, Allocator>& y) {
return !(x == y);
}

template <class Key, class T, class Compare, class Allocator>
inline
bool operator >(const Map<Key, T, Compare, Allocator>& x,
        const Map<Key, T, Compare, Allocator>& y) {
return y < x;
}

template <class Key, class T, class Compare, class Allocator>
inline
bool operator>=(const Map<Key, T, Compare, Allocator>& x,
        const Map<Key, T, Compare, Allocator>& y) {
return !(x < y);
}

template <class Key, class T, class Compare, class Allocator>
inline
bool operator<=(const Map<Key, T, Compare, Allocator>& x,
        const Map<Key, T, Compare, Allocator>& y) {
return !(y < x);
}

}

namespace std {

    template <class _Key, class _Tp, class _Compare, class _Allocator>
    inline
    void
    swap(ft::Map<_Key, _Tp, _Compare, _Allocator>& x, ft::Map<_Key, _Tp, _Compare, _Allocator>& y) {
        x.swap(y);
    }
}


#endif
