/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Iterator.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cshells <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/24 19:31:31 by cshells           #+#    #+#             */
/*   Updated: 2021/09/24 19:31:32 by cshells          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef __FT_ITERATOR_H__
#define __FT_ITERATOR_H__

#include <cstddef>
#include <iterator>
#include <type_traits>
#include <__config>
#include "Utils.hpp"

namespace ft {

    template <class Category, class T, class Distance = std::ptrdiff_t,
            class Pointer = T*, class Reference = T&>
                    struct Iterator {
                        typedef T                                   value_type;
                        typedef Distance                            difference_type;
                        typedef Pointer                             pointer;
                        typedef Reference                           reference;
                        typedef Category                            iterator_category;

                    };

    template <class Iter>
    struct Iterator_traits {

    public:
                typedef typename Iter::value_type           value_type;
                typedef typename Iter::difference_type      difference_type;
                typedef typename Iter::pointer              pointer;
                typedef typename Iter::const_pointer              const_pointer;
                typedef typename Iter::reference            reference;
                typedef typename Iter::const_reference            const_reference;
                typedef typename Iter::iterator_category    iterator_category;
            };

    template <typename T>
    struct Iterator_traits<T*> {

    public:
        typedef T                                       value_type;
        typedef T*                                      pointer;
        typedef const T*                                const_pointer;
        typedef T&                                      reference;
        typedef const T&                                const_reference;
        typedef ptrdiff_t                               difference_type;
        typedef std::random_access_iterator_tag         iterator_category;
    };

    template <class Iter>
            class wrap_iter {

            public:
                typedef Iter                                                            iterator_type;
                typedef typename ft::Iterator_traits<iterator_type>::value_type         value_type;
                typedef typename ft::Iterator_traits<iterator_type>::pointer            pointer;
                typedef typename ft::Iterator_traits<iterator_type>::const_pointer            const_pointer;
                typedef typename ft::Iterator_traits<iterator_type>::reference          reference;
                typedef typename ft::Iterator_traits<iterator_type>::const_reference          const_reference;
                typedef typename ft::Iterator_traits<iterator_type>::difference_type    difference_type;
                typedef typename ft::Iterator_traits<iterator_type>::iterator_category  iterator_category;

            private:
                iterator_type   _i;

            public:

                wrap_iter() : _i(NULL) {};
                explicit wrap_iter(iterator_type _x) : _i(_x) {};

                template <class U>
                wrap_iter(const wrap_iter<U>& other,
                          typename ft::Enable_if<std::is_convertible<U, iterator_type>::value>::type* = 0)
                          : _i(other.base()) {};

                wrap_iter& operator=(const wrap_iter& other) {
                    this->_i = other._i;

                    return (*this);
                }

                reference operator*() const                      { return *_i; }
                pointer operator->() const                       { return &(*_i); }
                wrap_iter& operator++()                          { ++_i; return *this; }
                wrap_iter operator++(int)                        { wrap_iter tmp(*this); ++(*this); return tmp; }
                wrap_iter& operator--()                          { --_i; return *this; }
                wrap_iter operator--(int)                        { wrap_iter tmp(*this); --(*this); return tmp; }
                wrap_iter operator+(difference_type n) const     { wrap_iter w(*this); w += n; return w; }
                wrap_iter& operator+=(difference_type n)         { _i += n; return *this; }
                wrap_iter operator-(difference_type n) const     { return *this + (-n); }
                wrap_iter& operator-=(difference_type n)         { _i += -n; return *this; }
                reference operator[](difference_type n) const    { return _i[n]; }
                iterator_type base() const                       { return _i; }

                template <class Iter1, class Iter2>
                        friend bool operator==(const wrap_iter<Iter1>&, const wrap_iter<Iter2>&);
                template <class Iter1, class Iter2>
                        friend bool operator!=(const wrap_iter<Iter1>&, const wrap_iter<Iter2>&);
                template <class Iter1, class Iter2>
                        friend bool operator>(const wrap_iter<Iter1>&, const wrap_iter<Iter2>&);
                template <class Iter1, class Iter2>
                        friend bool operator>=(const wrap_iter<Iter1>&, const wrap_iter<Iter2>&);
                template <class Iter1, class Iter2>
                        friend bool operator<(const wrap_iter<Iter1>&, const wrap_iter<Iter2>&);
                template <class Iter1, class Iter2>
                        friend bool operator<=(const wrap_iter<Iter1>&, const wrap_iter<Iter2>&);

            };

    template <class Iter1, class Iter2>
    inline
    bool operator==(const wrap_iter<Iter1>& x, const wrap_iter<Iter2>& y)
    { return x.base() == y.base(); }

    template <class Iter1, class Iter2>
    inline
    bool operator<(const wrap_iter<Iter1>& x, const wrap_iter<Iter2>& y)
    { return x.base() < y.base(); }

    template <class Iter1, class Iter2>
    inline
    bool operator!=(const wrap_iter<Iter1>& x, const wrap_iter<Iter2>& y)
    { return !(x == y); }

    template <class Iter1, class Iter2>
    inline
    bool operator>(const wrap_iter<Iter1>& x, const wrap_iter<Iter2>& y)
    { return y < x; }

    template <class Iter1, class Iter2>
    inline
    bool operator>=(const wrap_iter<Iter1>& x, const wrap_iter<Iter2>& y)
    { return !(x < y); }

    template <class Iter1, class Iter2>
    inline
    typename wrap_iter<Iter1>::difference_type operator-(const wrap_iter<Iter1>& x, const wrap_iter<Iter2>& y)
    { return x.base() - y.base(); }

    template <class Iter1, class Iter2>
    inline
    bool operator<=(const wrap_iter<Iter1>& x, const wrap_iter<Iter2>& y)
    { return !(y < x); }

    template <class Iter>
    inline
    wrap_iter<Iter> operator+(typename wrap_iter<Iter>::difference_type n, wrap_iter<Iter> x) {

        x += n;
        return x;
    }


    template <class Iter, class V>
    class NodeIterator {

    public:

        typedef Iter                                            iterator_type;
        typedef V                                            value_type;
        typedef ptrdiff_t                                       difference_type;
        typedef V&                                           reference;
        typedef const V&                                     const_reference;
        typedef V*                                           pointer;
        typedef const V*                                     const_pointer;
        typedef std::bidirectional_iterator_tag                 iterator_category;

    private:

        iterator_type   _i;

    public:

        NodeIterator() : _i(NULL) {}

        explicit NodeIterator(iterator_type x) : _i(x) {}
        NodeIterator(const NodeIterator& other) : _i(other.base()) {}

        template <class U>
                NodeIterator(const NodeIterator<U, V>& other,
                             typename ft::Enable_if<std::is_convertible<U, Iter>::value>::type* = 0) : _i(other.base()) {}

    public:
        NodeIterator& operator=(const NodeIterator& n) {

            if (this != &n) {
                this->_i = n.base();
            }
            return *this;
        }

    public:
        iterator_type base() const { return _i; }

    private:

        void increment(iterator_type& n) {

            iterator_type tmp;
            iterator_type current;

            if (n->nil && n->begin != n) {
                n = n->begin;
            }
            else if (!n->right->nil) {
                n = n->right;
                while (!n->left->nil) {
                    n = n->left;
                }
            } else {
                tmp = n;
                current = n;
                if (!(n->parent)) {
                    n = current->right;
                    return ;
                }
                n = n->parent;
                while (n->left != tmp) {
                    if (!(n->parent)) {
                        n = current->right;
                        return ;
                    }
                    tmp = n;
                    n = n->parent;
                }
            }
        }

        void decrement(iterator_type& n) {
            iterator_type tmp;

            if (n->nil)
                n = n->parent;
            else if (!n->left->nil) {
                n = n->left;
                while (!n->right->nil) {
                    n = n->right;
                }
            } else {
                tmp = n;
                n = n->parent;
                while (n->right != tmp) {
                    tmp = n;
                    if (!(n->parent)) {
                        n = tmp->left - 1;
                        break ;
                    }
                    n = n->parent;
                }
            }
        }

    public:

        reference operator*() { return *(_i->data); }
        const_reference operator*() const { return *(_i->data); }
        pointer operator->() { return _i->data; }
        const_pointer operator->() const { return _i->data; }
        NodeIterator& operator++() { increment(_i); return *this; }
        NodeIterator operator++(int) { NodeIterator tmp(*this); increment(this->_i); return tmp; }
        NodeIterator& operator--() { decrement(_i); return *this; }
        NodeIterator operator--(int) { NodeIterator tmp(*this); decrement(this->_i); return tmp; }


        template <class Iter1, class Iter2, class V1>
        friend
        bool operator==(const NodeIterator<Iter1, V1>& x, const NodeIterator<Iter2, V1>& y);

        template <class Iter1, class Iter2, class V1>
        friend
        bool operator!=(const NodeIterator<Iter1, V1 >& x, const NodeIterator<Iter2, V1>& y);

    };

    template <class Iter1, class Iter2, class V1>
    bool operator==(const NodeIterator<Iter1, V1 >& x, const NodeIterator<Iter2, V1 >& y) {
        return (x.base() == y.base());
    }

    template <class Iter1, class Iter2, class V1 >
    bool operator!=(const NodeIterator<Iter1, V1 >& x, const NodeIterator<Iter2, V1 >& y) {
        return !(x == y);
    }

    template <class Iter>
    class reverse_iterator : public Iterator<typename Iterator_traits<Iter>::iterator_category,
                                             typename Iterator_traits<Iter>::value_type,
                                             typename Iterator_traits<Iter>::difference_type,
                                             typename Iterator_traits<Iter>::reference,
                                             typename Iterator_traits<Iter>::pointer>
    {

    protected:
        Iter _current;

    public:
        typedef Iter                                                iterator_type;
        typedef typename Iterator_traits<Iter>::difference_type     difference_type;
        typedef typename Iterator_traits<Iter>::reference           reference;
        typedef typename Iterator_traits<Iter>::const_reference     const_reference;
        typedef typename Iterator_traits<Iter>::pointer             pointer;
        typedef typename Iterator_traits<Iter>::const_pointer       const_pointer;

        reverse_iterator() : _current() {};
        explicit reverse_iterator(Iter _x) : _current(_x) {};

        template <class U>
        reverse_iterator(const reverse_iterator<U>& u) : _current(u.base()) {};

        template <class U>
        reverse_iterator& operator=(const reverse_iterator<U>& u)
                { _current = u.base(); return *this; }

        Iter base() const
        { return _current; }

        reference operator*() const                         { Iter tmp = _current; return *tmp; } // delete -- prev tmp
//        const_reference operator*()                        { Iter tmp = _current; return *tmp; }
        pointer operator->() const                          { return &(operator*()); }
//        const_pointer operator->()                           { return &(operator*()); }
        reverse_iterator& operator++()                      { --_current; return *this; }
        reverse_iterator operator++(int)                    { reverse_iterator tmp(*this); _current--; return tmp; }
        reverse_iterator& operator--()                      { ++_current; return *this; }
        reverse_iterator operator--(int)                    { reverse_iterator tmp(*this); _current++; return tmp; }
        reverse_iterator operator+(difference_type n) const { return reverse_iterator(_current - n); }
        reverse_iterator& operator+=(difference_type n)     { _current -= n; return *this; }
        reverse_iterator operator-(difference_type n) const { return reverse_iterator(_current + n); }
        reverse_iterator& operator-=(difference_type n)     { _current += n; return *this; }
        reference operator[](difference_type n) const       { return *(*this + n); }

    };

    template <class Iter1, class Iter2>
    inline bool operator==(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y)
    { return x.base() == y.base(); }

    template <class Iter1, class Iter2>
    inline bool operator<(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y)
    { return x.base() > y.base(); }

    template <class Iter1, class Iter2>
    inline bool operator!=(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y)
    { return x.base() != y.base(); }

    template <class Iter1, class Iter2>
    inline bool operator>(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y)
    { return x.base() < y.base(); }

    template <class Iter1, class Iter2>
    inline bool operator>=(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y)
    { return x.base() <= y.base(); }

    template <class Iter1, class Iter2>
    inline bool operator<=(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y)
    { return x.base() >= y.base(); }

    template <class Iter1, class Iter2>
    inline
    typename reverse_iterator<Iter1>::difference_type
    operator-(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y)
    { return y.base() - x.base(); }

    template <class Iter>
    inline
    reverse_iterator<Iter>
    operator+(typename reverse_iterator<Iter>::difference_type n, const reverse_iterator<Iter> x)
    { return reverse_iterator<Iter>(x.base() - n); }


    template <class T, size_t N>
    inline
    T* begin(T (&array)[N])
    { return array; }

    template <class T, size_t N>
    inline
    T* end(T (&array)[N])
    { return array + N; }

    template <class C>
    inline
    typename C::iterator begin(C& c)
    { return c.begin(); }

    template <class C>
    inline
    typename C::const_iterator begin(const C& c)
    { return c.begin(); }

    template <class C>
    inline
    typename C::iterator end(C& c)
    { return c.end(); }

    template <class C>
    inline typename C::const_iterator end(const C& c)
    { return c.end(); }


}

#endif
