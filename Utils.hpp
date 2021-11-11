/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cshells <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/30 19:49:37 by cshells           #+#    #+#             */
/*   Updated: 2021/09/30 19:49:40 by cshells          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef __UTILS_H__
#define __UTILS_H__

namespace ft {

    template <class T, class Allocator = std::allocator<T> >
    struct Node {

        typedef Allocator                                                       allocator_type;
        typedef std::allocator_traits<Allocator>                                alloc_traits;
        typedef typename alloc_traits::pointer                                 pointer;
        typedef typename alloc_traits::const_pointer                           const_pointer;
        typedef typename alloc_traits::size_type                               size_type;
        typedef typename alloc_traits::difference_type                         difference_type;

        Node*       left;
        Node*       right;
        Node*       parent;
        Node*       begin;
        bool        red;
        bool        nil;
        T          *data;
        allocator_type _alloc;

        Node(const allocator_type& alloc = allocator_type()) : left(this), right(this), parent(NULL), begin(NULL), red(0), nil(1), data(NULL), _alloc(alloc) {}

        Node(const T& x, const allocator_type& alloc = allocator_type()) : left(this), right(this), parent(NULL), begin(NULL),
        red(0), nil(0), _alloc(alloc) {
            data = _alloc.allocate(sizeof(T));
            _alloc.construct(data, x);
        }

        ~Node() {
            if (data) {
                _alloc.destroy(data);
                _alloc.deallocate(data, sizeof(T));
            }
        }

        template <class U>
        Node(Node<U>& n) : left(n.left), right(n.right),
                            parent(n.parent), begin(n.begin), red(n.red), nil(n.nil), data(new T(n.data)) {}

        friend bool operator==(const Node& x, const Node& y) {
            return &x == &y;
        }
        friend bool operator!=(const Node& x, const Node& y) {
            return !(x == y);
        }
    };

    template <class T, class U>
    struct Pair {

        typedef T        type;

        T  first;
        U   second;

        Pair() : first() {}
        Pair(const T& t, const U& u) : first(t), second(u) {}

        template <class I, class V>
        Pair(const Pair<I, V>& p) : first(p.first), second(p.second) {}

        Pair& operator=(const Pair& p) {
            if (this != &p) {
                first = p.first;
                second = p.second;
            }
            return *this;
        }

        void swap(Pair& p) {

            using std::swap;

            swap(first, p.first);
            swap(second, p.second);
        }
    };

    template <class T, class U>
    inline
    bool
    operator==(const ft::Pair<T, U>& x, const ft::Pair<T, U>& y) {
        return (x.first == y.first && x.second == y.second);
    }

    template <class T, class U>
    inline
    bool
    operator!=(const ft::Pair<T, U>& x, const ft::Pair<T, U>& y) {
        return !(x == y);
    }

    template <class T, class U>
    inline
    bool
    operator< (const ft::Pair<T, U>& x, const ft::Pair<T, U>& y) {
        return (x.first < y.first || (!(y.first < x.first) && x.second < y.second));
    }

    template <class T, class U>
    inline
    bool
    operator> (const ft::Pair<T, U>& x, const ft::Pair<T, U>& y) {
        return (y < x);
    }

    template <class T, class U>
    inline
    bool
    operator>=(const ft::Pair<T, U>& x, const ft::Pair<T, U>& y) {
        return !(x < y);
    }

    template <class T, class U>
    inline
    bool
    operator<=(const ft::Pair<T, U>& x, const ft::Pair<T, U>& y) {
        return !(y < x);
    }

    template <class T, class U>
    inline
    Pair<T, U> make_pair(T x, U y) {
        return (Pair<T, U>(x, y));
    }

    template <bool B, class T = void>
    struct Enable_if {
    };

    template <class T>
    struct Enable_if<true, T> {
        typedef T type;
    };

    template <class T, T v>
    struct Integral_constant
    {
        static const T     value = v;
        typedef T          value_type;
        typedef Integral_constant type;
        operator value_type() const {return value;}
    };

    #define Integral_constant(b) Integral_constant<bool,(b)>

    typedef Integral_constant(true)     true_type;
    typedef Integral_constant(false)     false_type;

    template <class T>
    struct Is_integral : public false_type {
        static const bool value = false;
    };

    template <>
    struct Is_integral<bool> : public true_type {
        static const bool value = true;
    };

    template <>
    struct Is_integral<char> : public true_type {
        static const bool value = true;
    };

    template <>
    struct Is_integral<char16_t> : public true_type {
        static const bool value = true;
    };

    template <>
    struct Is_integral<char32_t> : public true_type {
        static const bool value = true;
    };

    template <>
    struct Is_integral<wchar_t> : public true_type {
        static const bool value = true;
    };

    template <>
    struct Is_integral<signed char> : public true_type {
        static const bool value = true;
    };

    template <>
    struct Is_integral<short int> : public true_type {
        static const bool value = true;
    };

    template <>
    struct Is_integral<int> : public true_type {
        static const bool value = true;
    };

    template <>
    struct Is_integral<long int> : public true_type {
        static const bool value = true;
    };

    template <>
    struct Is_integral<long long int> : public true_type {
        static const bool value = true;
    };

    template <>
    struct Is_integral<unsigned char> : public true_type {
        static const bool value = true;
    };

    template <>
    struct Is_integral<unsigned short int> : public true_type {
        static const bool value = true;
    };

    template <>
    struct Is_integral<unsigned int> : public true_type {
        static const bool value = true;
    };

    template <>
    struct Is_integral<unsigned long int> : public true_type {
        static const bool value = true;
    };

    template <>
    struct Is_integral<unsigned long long int> : public true_type {
        static const bool value = true;
    };

    template <class InputIt1, class InputIt2>
    bool equal(InputIt1 first1, InputIt1 last1,
               InputIt2 first2) {
        for (; first1 != last1; ++first1, ++first2) {
            if (!(*first1 == *first2))
                return false;
        }
        return true;
    }

    template <class InputIt1, class InputIt2, class BinaryPredicate>
    bool equal(InputIt1 first1, InputIt1 last1,
               InputIt2 first2, BinaryPredicate p) {
        for (; first1 != last1; ++first1, ++first2) {
            if (!p(*first1, *first2))
                return false;
        }
        return true;
    }

    template <class InputIt1, class InputIt2>
    bool lexicographical_compare(InputIt1 first1, InputIt1 last1,
                                 InputIt2 first2, InputIt2 last2) {

        for (; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2) {
            if (*first1 < *first2) return true;
            if (*first2 < *first1) return false;
        }
        return (first1 == last1) && (first2 != last2);
    }

    template <class InputIt1, class InputIt2, class Compare>
    bool lexicographical_compare(InputIt1 first1, InputIt1 last1,
                                 InputIt2 first2, InputIt2 last2,
                                 Compare comp) {

        for (; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2) {
            if (comp(first1, first2)) return true;
            if (comp(first2, first1)) return false;
        }
        return (first1 == last1) && (first2 != last2);
    }
}

#endif
