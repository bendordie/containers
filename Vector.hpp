/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cshells <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/17 21:58:14 by cshells           #+#    #+#             */
/*   Updated: 2021/09/17 21:58:24 by cshells          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef __FT_VECTOR_H__
#define __FT_VECTOR_H__

#include <algorithm>
#include <memory>
#include <cassert>
#include <iterator>
#include <stdexcept>
#include <limits>
#include <type_traits>
#include <__config>
#include "Iterator.hpp"
#include "Utils.hpp"

namespace ft {

    template < class T, class A = std::allocator<T> >
    class Vector {

    public:

        typedef A                                                   allocator_type;
        typedef std::allocator_traits<allocator_type>               alloc_traits;

        typedef T                                                   value_type;
        typedef value_type&                                         reference;
        typedef const value_type&                                   const_reference;
        typedef typename alloc_traits::size_type                    size_type;
        typedef typename alloc_traits::pointer                      pointer;
        typedef typename alloc_traits::const_pointer                const_pointer;
        typedef typename alloc_traits::difference_type              difference_type;
        typedef ft::wrap_iter<pointer>                              iterator;
        typedef ft::wrap_iter<const_pointer>                        const_iterator;
        typedef ft::reverse_iterator<iterator>                      reverse_iterator;
        typedef ft::reverse_iterator<const_iterator>                const_reverse_iterator;


    private:

        pointer         _begin;
        pointer         _end;
        pointer         _end_cap;
        pointer         _container;
        size_type       _size;
        size_type       _capacity;
        allocator_type  _allocator;

    public:

        /****************************************************************************************************/
        /****************************************   CONSTRUCTORS   ******************************************/
        /****************************************************************************************************/

        explicit Vector (const allocator_type& alloc = allocator_type())
        : _begin(0), _end(0), _end_cap(0), _container(0), _size(0), _capacity(0), _allocator(alloc) {

            _begin = _end = _end_cap = _container = this->vallocate(0);
            _size = _capacity = 0;
        }

        explicit Vector (size_type n, const value_type& x = value_type(),
                         const A& alloc = allocator_type())
                         : _begin(0), _end(0), _end_cap(0), _container(0),
                         _size(0), _capacity(0), _allocator(alloc) {

            _allocator = alloc;
            _begin = _container = this->vallocate(n);
            _size = _capacity;
            _end = _end_cap = _begin + _size;
            std::fill(this->begin(), this->end(),  x);

        };

        template <class InputIterator>
                Vector(InputIterator first, InputIterator last,
                       const allocator_type& alloc = allocator_type(),
                       typename ft::Enable_if<!ft::Is_integral<InputIterator>::value, void>::type* = 0)
                       : _begin(0), _end(0), _end_cap(0), _container(0),
                       _size(0), _capacity(0), _allocator(alloc) {

            this->assign(first, last);
        };

        Vector (const Vector& x)
        : _begin(0), _end(0), _end_cap(0), _container(0),
        _size(x.size()), _capacity(0), _allocator(x.get_allocator()){

            pointer         buf = 0;

            buf = vallocate(x.capacity());
            _begin = _container = buf;
            _end = _begin + x.size();
            _end_cap = _begin + x.capacity();
            _size = x.size();
            _capacity = x.capacity();
            std::copy(x.begin(), x.end(), this->begin());
        };


        /****************************************************************************************************/
        /*****************************************   ASSIGNMENT   *******************************************/
        /****************************************************************************************************/

        Vector & operator=(const Vector & v) {
            if (this != &v) {
                this->clear();
                this->vdeallocate();
                this->_allocator = v.get_allocator();
                this->_size = v.size();
                this->_capacity = v.capacity();
                this->_begin = this->_container = this->_allocator.allocate(v.capacity());
                this->_end = this->_begin + this->size();
                this->_end_cap = this->_begin + this->capacity();
                std::copy(v.begin(), v.end(), this->begin());
            }

            return (*this);
        }

        /****************************************************************************************************/
        /*****************************************   DESTRUCTOR   *******************************************/
        /****************************************************************************************************/

        ~Vector() {
            clear();
            this->vdeallocate();
        };

        /****************************************************************************************************/
        /*****************************************   ITERATORS   ********************************************/
        /****************************************************************************************************/

        iterator begin() {
            return iterator(_begin);
        }

        const_iterator begin() const {
            return iterator(_begin);
        }

        iterator end() {
            return iterator(_end);
        }

        const_iterator end() const {
            return iterator(_end);
        }

        reverse_iterator rbegin() {
            return reverse_iterator(--end());
        }

        const_reverse_iterator rbegin() const {
            return const_reverse_iterator(--end());
        }

        reverse_iterator rend() {
            return reverse_iterator(begin());
        }

        const_reverse_iterator rend() const {
            return const_reverse_iterator(begin());
        }

        /****************************************************************************************************/
        /***************************************   ELEMENT ACCESS   *****************************************/
        /****************************************************************************************************/

        value_type* data() {
            return _begin;
        }

        const value_type* data() const {
            return static_cast<const_pointer>(_begin);
        }

        reference operator[](size_type n) {
            return static_cast<reference>(_container[n]);
        }

        const_reference operator[](size_type n) const {
            return static_cast<const_reference>(_container[n]);
        }

        reference at(size_type n) {
            if (n >= this->_size)
                throw std::out_of_range("vector::_M_range_check");
            return static_cast<reference>(_container[n]);
        }

        const_reference at(size_type n) const {
            if (n >= this->_size)
                throw std::out_of_range("vector::_M_range_check");
            return static_cast<const_reference>(_container[n]);
        }

        reference front() {
            return (*this->_begin);
        }

        const_reference front() const {
            return (*this->_begin);
        }

        reference back() {
            return (*(this->_end - 1));
        }

        const_reference back() const {
            return (*(this->_end - 1));
        }

        /****************************************************************************************************/
        /******************************************   CAPACITY   ********************************************/
        /****************************************************************************************************/

        pointer vallocate(size_type n);
        void    vdeallocate();

        bool empty() const {
            return (_size == 0 && _begin == _end ? 1 : 0);
        }

        size_type size() const {
            return (_size);
        }

        void resize(size_type n, value_type val = value_type()) {

            size_type       old_size = this->size();
            size_type       cur_cap = this->capacity();

            if (n > cur_cap) {
                reserve(n);
                std::fill(this->begin() + old_size, this->end() + n - old_size, val);
            } else if (n < old_size) {
                destructElemsInRange(this->begin() + n, this->end());
            } else {
                std::fill(this->begin() + old_size, this->end() + n - old_size, val);
            }
            _end += n - old_size;
            _size = n;
        }

        size_type max_size() const {
            return (std::min((size_type) std::numeric_limits<difference_type>::max(),
                             std::numeric_limits<size_type>::max() / sizeof(value_type)));
        }

        size_type capacity() const {
            return (_capacity);
        }

        void reserve(size_type n) {

            pointer buf = 0;

            if (n > this->capacity()) {
                buf = vallocate(n);
                moveElemsNewStorage(buf);
            }
        }

        /****************************************************************************************************/
        /**************************************   PRIVATE METHODS   *****************************************/
        /****************************************************************************************************/

    private:
        void offsetElemsLeftBy(iterator position, difference_type range);
        void offsetElemsRightBy(iterator position, difference_type range);
        void moveElemsNewStorage(pointer buffer);
        void moveElemsNewStorage(pointer buffer, iterator position, difference_type offset);
        void destructElemsInRange(iterator first, iterator last);

        /****************************************************************************************************/
        /*****************************************   MODIFIERS   ********************************************/
        /****************************************************************************************************/

    public:

        template <class InputIterator>
        typename ft::Enable_if<!ft::Is_integral<InputIterator>::value, void>::type
        assign(InputIterator first, InputIterator last) {

            int             i = 0;
            pointer         buf = 0;
            difference_type range = last - first;

            if (range > _capacity) {
                clear();
                vdeallocate();
                buf = _allocator.allocate(range);
                _begin = _container = buf;
                _end_cap = _end = _begin + range;
                _capacity = _size = range;
            }
            for (; i < range && i < _capacity; ++i, ++first) {
                _container[i] = *first;
            }
            _size = i;
            _end = _begin + i;
        }

        void assign(size_type n, const value_type& val) {
            resize(n , val);
        }

        void push_back(const value_type& val) {
            resize(this->size() + 1, val);
        }

        void pop_back() {
            resize(this->size() - 1);
        }

        void clear() {
            for (; _begin != _end; ++_begin) {
                _allocator.destroy(_begin);
            }
            _size = 0;
            _end_cap = _end = _begin;
        }

        iterator insert(iterator position, const value_type& val) {

            pointer         buf = 0;
            difference_type pos = position - this->begin();

            if (_size + 1 <= _capacity) {
                offsetElemsRightBy(position, 1);
                _container[pos] = val;
            }
            else {
                buf = vallocate(_size + 1);
                moveElemsNewStorage(buf, position, 1);
                _container[pos] = val;
            }
            return iterator(this->begin() + pos);
        }

        iterator insert(iterator position, size_type n, const value_type& val) {

            pointer         buf = 0;
            difference_type pos = position - this->begin();

            if (_size + n <= _capacity) {
                offsetElemsRightBy(position, n);
                std::fill(position, position + n, val);
            }
            else {
                buf = vallocate(_size + n);
                moveElemsNewStorage(buf, position, n);
                std::fill(this->begin() + pos, this->begin() + pos + n, val);
            }
            return iterator(this->begin() + pos);
        }

        template <class InputIterator>
        typename ft::Enable_if<!ft::Is_integral<InputIterator>::value, iterator>::type
        insert(iterator position, InputIterator first, InputIterator last) {

            pointer         buf = 0;
            difference_type pos = position - this->begin();
            difference_type range = last - first;

            if (!validate_iterator_values(first, last, range))
                throw std::exception();
            if (_size + range <= _capacity) {
                offsetElemsRightBy(position, range);
                std::copy(first, last, _container + pos);
            }
            else {

                buf = vallocate(_size + range);
                moveElemsNewStorage(buf, position, range);
                std::copy(first, last, _container + pos);
            }
            return iterator(this->begin() + pos);
        }

        iterator erase(iterator position) {

            difference_type pos = position - this->begin();

            _allocator.destroy(&(*position));
            if (position != this->end()) {
                offsetElemsLeftBy(position, 1);
            }
            return iterator(this->begin() + pos);
        }

        iterator erase(iterator first, iterator last) {

            iterator        position = first;
            difference_type pos = first - this->begin();
            difference_type range = last - first;

            destructElemsInRange(first, last);
            if (position != this->end()) {
                offsetElemsLeftBy(position, range);
            }
            return iterator(this->begin() + pos);
        }

        void swap(Vector& x) {

            if (this != &x) {
                std::swap(this->_size, x._size);
                std::swap(this->_capacity, x._capacity);
                std::swap(this->_begin, x._begin);
                std::swap(this->_end, x._end);
                std::swap(this->_end_cap, x._end_cap);
                std::swap(this->_container, x._container);
                std::swap(this->_allocator, x._allocator);
            }
        }

    private:

        template<class InputIt>
        typename ft::Enable_if<!ft::Is_integral<InputIt>::value, bool>::type
        validate_iterator_values(InputIt first, InputIt last, size_t range) {

            bool result = true;
            pointer reserved_buffer = 0;
            size_t i = 0;

            reserved_buffer = _allocator .allocate(range);
            for (;first != last; ++first, ++i) {
                try { reserved_buffer[i] = *first; }
                catch (...) { result = false; break; }
            }
            _allocator.deallocate(reserved_buffer, range);
            return result;
        }

        /****************************************************************************************************/
        /*****************************************   ALLOCATOR   ********************************************/
        /****************************************************************************************************/

        allocator_type get_allocator() const {
            return (_allocator);
        }

    };

    /****************************************************************************************************/
    /***************************************   RELATIONAL OPS   *****************************************/
    /****************************************************************************************************/

    template <class T, class Allocator>
    inline
    bool operator==(const Vector<T, Allocator>& x, const Vector<T, Allocator>& y) {
        return x.size() == y.size() && ft::equal(x.begin(), x.end(), y.begin());
    }

    template <class T, class Allocator>
    inline
    bool operator!=(const Vector<T, Allocator>& x, const Vector<T, Allocator>& y) {
        return !(x == y);
    }

    template <class T, class Allocator>
    inline
    bool operator<(const Vector<T, Allocator>& x, const Vector<T, Allocator>& y) {
        return ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
    }

    template <class T, class Allocator>
    inline
    bool operator>(const Vector<T, Allocator>& x, const Vector<T, Allocator>& y) {
        return y < x;
    }

    template <class T, class Allocator>
    inline
    bool operator>=(const Vector<T, Allocator>& x, const Vector<T, Allocator>& y) {
        return !(x < y);
    }

    template <class T, class Allocator>
    inline
    bool operator<=(const Vector<T, Allocator>& x, const Vector<T, Allocator>& y) {
        return !(y < x);
    }

    /****************************************************************************************************/
    /**************************************   PRIVATE METHODS   *****************************************/
    /****************************************************************************************************/

    template <class T, class Allocator>
    typename Vector<T, Allocator>::pointer
    Vector<T, Allocator>::vallocate(size_type n) {

        pointer         buf = 0;
        size_type       old_cap = this->capacity();

        if (n > this->max_size())
            throw std::out_of_range("max Vector size:");
        if (n > old_cap * 2) {
            buf = alloc_traits::allocate(_allocator, n);
            _capacity = n;
        }
        else {
            buf = alloc_traits::allocate(_allocator, old_cap * 2);
            _capacity = old_cap * 2;
        }
        return (buf);
    }

    template <class T, class Allocator>
    void
    Vector<T, Allocator>::vdeallocate() {

        if (_container != NULL) {
            _allocator.deallocate(_container, _capacity);
            _begin = _end = _end_cap = NULL;
        }
    }

    template <class T, class Allocator>
    void
    Vector<T, Allocator>::destructElemsInRange(iterator first, iterator last) {

        for (; first != last; ++first) {
            _allocator.destroy(&(*first));
        }
    }

    template <class T, class Allocator>
    void
    Vector<T, Allocator>::offsetElemsLeftBy(iterator position, difference_type range) {

        difference_type pos = position - this->begin();
        difference_type end = this->end() - this->begin();

        if (_container != NULL) {
            for (difference_type i = 0; pos + range + i != end; ++i) {
                _container[pos + i] = _container[pos + range + i];
            }
        }

        _end -= range;
        _size -= range;
    }

    template <class T, class Allocator>
    void
    Vector<T, Allocator>::offsetElemsRightBy(iterator position, difference_type range) {

        difference_type pos = position - this->begin();
        difference_type end = this->end() - this->begin();

        if (_container != NULL &&
        static_cast<difference_type>(this->capacity() - this->size()) <= range) {
            for (; end != pos - 1; --end) {
                _container[end + range] = _container[end];
            }
        }
        _end += range;
        _size += range;
    }

    template <class T, class Allocator>
    void
    Vector<T, Allocator>::moveElemsNewStorage(pointer buffer) {

        std::copy(this->begin(), this->end(), buffer);
        vdeallocate();
        _container = _begin = buffer;
        _end = _begin + this->size();
    }

    template <class T, class Allocator>
    void
    Vector<T, Allocator>::moveElemsNewStorage(pointer buffer, iterator position, difference_type offset) {

        size_type       old_size = this->size();
        difference_type pos = position - this->begin();

        std::copy(this->begin(), this->begin() + pos, buffer);
        std::copy(this->begin() + pos, this->end(), buffer + pos + offset);
        vdeallocate();
        _container = _begin = buffer;
        _end = _begin + old_size + offset;
        _size = old_size + offset;
    }

}

namespace std {

    template <class T, class Allocator>
    inline
    void
    swap(ft::Vector<T, Allocator>& x, ft::Vector<T, Allocator>& y) {
        x.swap(y);
    }
};


#endif
