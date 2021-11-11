/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Stack.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cshells <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/06 15:06:12 by cshells           #+#    #+#             */
/*   Updated: 2021/10/06 15:06:13 by cshells          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef __FT_STACK_H__
#define __FT_STACK_H__

#include <algorithm>
#include <type_traits>
#include <cstddef>
#include <cassert>
#include "Vector.hpp"

namespace ft {

    template < class T, class Container = ft::Vector<T> >
    class Stack {


    public:

        typedef Container                                       container_type;
        typedef typename container_type::value_type             value_type;
        typedef typename container_type::reference              reference;
        typedef typename container_type::const_reference        const_reference;
        typedef typename container_type::size_type              size_type;


    protected:

        container_type _container;

    public:

        /******************************************************************************/
        /*****************************   CONSTRUCTORS   *******************************/
        /******************************************************************************/

        explicit Stack(const container_type& cont = container_type()) : _container(cont) {}
        Stack(const Stack& s) : _container(s._container) {}

        Stack& operator=(const Stack& s) {
            if (this != &s) {
                _container = s._container;
            }
            return (*this);
        }

        /******************************************************************************/
        /*******************************   CAPACITY   *********************************/
        /******************************************************************************/

        bool empty() const {
            return (_container.empty());
        }

        size_type size() const {
            return (_container.size());
        }

        /******************************************************************************/
        /****************************   ELEMENT ACCESS   ******************************/
        /******************************************************************************/

        reference top() {
            return (_container.back());
        }

        const_reference top() const {
            return (_container.back());
        }

        /******************************************************************************/
        /******************************   MODIFIERS   *********************************/
        /******************************************************************************/

        void push(const value_type& v) {
            return (_container.push_back(v));
        }

        void pop() {
            return (_container.pop_back());
        }

        /******************************************************************************/
        /******************************************************************************/
        /****************************   RELATIONAL OPS   ******************************/

        template <class V, class Cont>
        friend
        bool
        operator==(const ft::Stack<V, Cont>& x, const ft::Stack<V, Cont>& y) {
            return x._container == y._container;
        }

        template <class V, class Cont>
        friend
        bool
        operator< (const ft::Stack<V, Cont>& x, const ft::Stack<V, Cont>& y) {
            return x._container < y._container;
        }

        template <class V, class Cont>
        friend
        bool
        operator!=(const ft::Stack<V, Cont>& x, const ft::Stack<V, Cont>& y) {
            return !(x == y);
        }

        template <class V, class Cont>
        friend
        bool
        operator> (const ft::Stack<V, Cont>& x, const ft::Stack<V, Cont>& y) {
            return (y < x);
        }

        template <class V, class Cont>
        friend
        bool
        operator>=(const ft::Stack<V, Cont>& x, const ft::Stack<V, Cont>& y) {
            return !(x < y);
        }

        template <class V, class Cont>
        friend
        bool
        operator<=(const ft::Stack<V, Cont>& x, const ft::Stack<V, Cont>& y) {
            return !(y < x);
        }

    };
}


#endif
