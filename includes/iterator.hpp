/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iterator.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbjaghou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 20:25:02 by nbjaghou          #+#    #+#             */
/*   Updated: 2021/12/16 20:25:04 by nbjaghou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <cstddef>
#include "AVL.hpp"
namespace ft
{
	// iterator traits means the feature/attribute of C++ Iterator
	//generic definition
	template <class Iterator>
	class iterator_traits
	{
		public:
			typedef typename Iterator::difference_type difference_type;
			typedef typename Iterator::value_type value_type;
			typedef typename Iterator::pointer pointer;
			typedef typename Iterator::reference reference;
			typedef typename Iterator::iterator_category iterator_category;
	};
	// T* specialization
	template <class T>
	class iterator_traits<T *>
	{
		public:
			typedef ptrdiff_t diffrence_type;
			typedef T value_type;
			typedef T *pointer;
			typedef T &reference;
			typedef std::random_access_iterator_tag iterator_category;
	};
	//const T* specialization
	template <class T>
	class iterator_traits<const T *>
	{
		public:
			typedef ptrdiff_t diffrence_type;
			typedef T value_type;
			typedef const T *pointer;
			typedef const T &reference;
			typedef std::random_access_iterator_tag iterator_category;
	};
	//https://en.cppreference.com/w/cpp/iterator/reverse_iterator
	// reverse iterator is an iterator that reverse the direction of a given iterator

	template<class Iterator>
	class reverse_iterator
	{
		public:
			/************************ Member types ******************************/
			typedef Iterator												iterator_type;
			typedef typename iterator_traits<Iterator>::iterator_category	iterator_category;
			typedef typename iterator_traits<Iterator>::value_type			value_type;
			typedef typename iterator_traits<Iterator>::difference_type		difference_type;
			typedef typename iterator_traits<Iterator>::pointer				pointer;
			typedef typename iterator_traits<Iterator>::reference			reference;
			/************************ End Member types **************************/

			/************************ End Member functions **************************/
			reverse_iterator() {}
			explicit reverse_iterator (iterator_type it): _it(it) {}
			template <class Iter>
			reverse_iterator (const reverse_iterator<Iter>& rev_it){_it = rev_it.base();}
			~reverse_iterator() {}
			//Returns a copy of the base iterator
			iterator_type base() const { return (_it);}
			//Internally, the function decreases a copy of its base iterator and returns the result of dereferencing it.
			reference operator*() const { return *(--Iterator(_it)); }
			//the function applies the binary operator- on the base iterator
			reverse_iterator operator+ (difference_type n) const {
				return (reverse_iterator(_it - n));
			}
			reverse_iterator operator- (difference_type n) const {
				return (reverse_iterator(_it + n));
			}
			//The pre-increment
			reverse_iterator& operator++()
			{
				_it--;
				return (*this);
			}
			//The post-increment
			reverse_iterator  operator++(int)
			{
				reverse_iterator tmp = *this;
				++(*this);
				return (tmp);
			}
			//The post-decrements
			reverse_iterator  operator--(int)
			{
				reverse_iterator tmp = *this;
				--(*this);
				return (tmp);
			}
			//The pre-decrements
			reverse_iterator& operator--()
			{
				_it++;
				return (*this);
			}
			reverse_iterator& operator+= (difference_type n)
			{
				_it -= n;
				return (*this);
			}
			reverse_iterator& operator-= (difference_type n)
			{
				_it += n;
				return (*this);
			}
			pointer operator->() const {return &(operator*());}
			reference operator[] (difference_type n) const { return (base()[-n-1]);}
			/************************ End Member functions **************************/
		private:
			iterator_type _it;
	};

	/******** Non-member function overloads of reverse_iterator ****************/
	// need to add
	template <class Iterator>
	bool operator== (const reverse_iterator<Iterator>& lhs,
					const reverse_iterator<Iterator>& rhs)
	{return (lhs.base() == rhs.base());}
	template <class Iterator>
	bool operator!= (const reverse_iterator<Iterator>& lhs,
					const reverse_iterator<Iterator>& rhs)
	{return (lhs.base() != rhs.base());}
	template <class Iterator>
	bool operator<  (const reverse_iterator<Iterator>& lhs,
					const reverse_iterator<Iterator>& rhs)
	{return (lhs.base() > rhs.base());}
	template <class Iterator>
	bool operator<= (const reverse_iterator<Iterator>& lhs,
					const reverse_iterator<Iterator>& rhs)
	{return (lhs.base() >= rhs.base());}
	template <class Iterator>
	bool operator>  (const reverse_iterator<Iterator>& lhs,
					const reverse_iterator<Iterator>& rhs)
	{return (lhs.base() < rhs.base());}
	template <class Iterator>
	bool operator>= (const reverse_iterator<Iterator>& lhs,
					const reverse_iterator<Iterator>& rhs)
	{return (lhs.base() <= rhs.base());}
	template <class Iterator>
	reverse_iterator<Iterator> operator+(
		typename reverse_iterator<Iterator>::difference_type n,
		const reverse_iterator<Iterator> &rev_it)
	{
		return (reverse_iterator<Iterator>(rev_it.base() - n));
	}
	template <class Iterator>
	typename reverse_iterator<Iterator>::difference_type operator-(
		const reverse_iterator<Iterator> &lhs,
		const reverse_iterator<Iterator> &rhs)
	{
		return (rhs.base() - lhs.base());
	}
	/*************************** Random Access Iterator *************************/	
	//https://docs.oracle.com/cd/E19205-01/819-3704/Ran_7821.htm
	// this is the iterator for vector
	/*
		They are the most powerful iterators. They are not limited to
		moving sequentially, as their name suggests, they can randomly access any element
		inside the container. They are the ones whose functionality is same as pointers.
	*/
	template<typename T>
	class random_access_iterator
	{
		public:
			/************************ Member types ******************************/
			typedef T											value_type;
			typedef std::random_access_iterator_tag				iterator_category;
			typedef T*											pointer;
			typedef T&											reference;
			typedef ptrdiff_t									difference_type;
			/************************ End Member types **************************/
			//Constructors 	X a; X b(a); b = a;
			random_access_iterator(): _it(NULL) {}
			explicit random_access_iterator(value_type *it): _it(it) {}
			random_access_iterator(random_access_iterator const &src) {*this = src;}
			random_access_iterator& operator=(random_access_iterator const &src)
			{
				_it = src._it;
				return (*this);
			}
			~random_access_iterator() {}
			// Can be dereferenced *a && a->m
			reference operator*() const { return (*_it);}
			pointer operator->() const {return &(operator*());}
			reference operator[] (difference_type n) const { return (_it[n]);}
			pointer base() const{return (_it);}
			//a == b, a != b	return value convertible to bool
			// If operator== is a non static data member, is should take only one paramete
			bool operator==(const random_access_iterator<T>& src){
				return (_it == src._it);
			}
			bool operator!=(const random_access_iterator<T>& src){
				return (_it != src._it);
			}
			random_access_iterator &operator++()
			{
				_it++;
				return (*this);
			}
			random_access_iterator operator++(int)
			{
				random_access_iterator tmp = *this;
				++(*this);
				return (tmp);
			}
			random_access_iterator &operator--()
			{
				_it--;
				return (*this);
			}
			random_access_iterator operator--(int)
			{
				random_access_iterator tmp = *this;
				--(*this);
				return (tmp);
			}
			random_access_iterator& operator+= (difference_type n)
			{
				_it += n;
				return (*this);
			}
			random_access_iterator& operator-= (difference_type n)
			{
				_it -= n;
				return (*this);
			}
			random_access_iterator operator+ (difference_type n) const {
				return (random_access_iterator(_it + n));
			}
			random_access_iterator operator- (difference_type n) const {
				return (random_access_iterator(_it - n));
			}
			operator random_access_iterator<const T>() const {
				return (random_access_iterator<const T>(_it));
			}
		private:
			pointer _it;
	};
	// Non-member function overload of random access iterator
	template <class Iterator>
	bool operator==(const random_access_iterator<Iterator> &lhs, const random_access_iterator<Iterator> &rhs)
	{
		return (lhs.base() == rhs.base());
	}
	template <class Iterator>
	bool operator!=(const random_access_iterator<Iterator> &lhs,
		const random_access_iterator<Iterator> &rhs)
	{
		return (lhs.base() != rhs.base());
	}
	template <class Iterator>
	bool operator<(const random_access_iterator<Iterator> &lhs,
			const random_access_iterator<Iterator> &rhs)
	{
		return (lhs.base() < rhs.base());
	}
	template <class Iterator>
	bool operator<=(const random_access_iterator<Iterator> &lhs,
		const random_access_iterator<Iterator> &rhs)
	{
		return (lhs.base() <= rhs.base());
	}
	template <class Iterator>
	bool operator>(const random_access_iterator<Iterator> &lhs,
		const random_access_iterator<Iterator> &rhs)
	{
		return (lhs.base() > rhs.base());
	}
	template <class Iterator>
	bool operator>=(const random_access_iterator<Iterator> &lhs,
		const random_access_iterator<Iterator> &rhs)
	{
		return (lhs.base() >= rhs.base());
	}
	template <class Iterator>
	random_access_iterator<Iterator> operator+(
		typename random_access_iterator<Iterator>::difference_type n,
		const random_access_iterator<Iterator> &rev_it)
	{
		return (random_access_iterator<Iterator>(rev_it.base() + n));
	}
	template <class Iterator>
	typename random_access_iterator<Iterator>::difference_type operator-(
		const random_access_iterator<Iterator> &lhs,
		const random_access_iterator<Iterator> &rhs)
	{
		return (lhs.base() - rhs.base());
	}
	/*
		Bidirectional iterators are iterators that can be used to access the sequence of elements in a
		range in both directions (towards the end and towards the beginning). They are similar to
		forward iterators, except that they can move in the backward direction also, unlike the forward
		iterators, which can move only in the forward direction.
	*/
	template <typename T, typename Compare, typename Alloc>
	class bidirectional_iterator
	{
		public:
			typedef T                               value_type; // DRY
			typedef int                             difference_type;
			typedef T*                              pointer;
			typedef T&                              reference;
			typedef std::bidirectional_iterator_tag iterator_category;
			bidirectional_iterator(pointer ptr, const AVL<value_type, Compare, Alloc>* avl = NULL)
			: _ptr(ptr), _avl(avl){};
			bidirectional_iterator(): _ptr(NULL) {}
			~bidirectional_iterator(){}
			bidirectional_iterator&  operator=(const bidirectional_iterator& bid_it)
			{
				_ptr = bid_it._ptr;
				_avl = bid_it._avl;
				return (*this);
			}
			operator bidirectional_iterator<const T, Compare, Alloc> () const { return bidirectional_iterator<const T, Compare, Alloc>(_ptr, reinterpret_cast<const AVL<const value_type, Compare, Alloc>*>(_avl)); }
			bidirectional_iterator& operator++()
			{
				//TODO
				AvlNode<value_type, Alloc> *node = _avl->find(_avl->root, *_ptr);
				if (node)
				{
					AvlNode<value_type, Alloc> *succ = _avl->successor(*_ptr);
					if (succ)
						_ptr = succ->data;
					else
						_ptr = NULL;
				}
				return (*this);
			}
			bidirectional_iterator operator++(int)
			{
				bidirectional_iterator i = *this;
				++(*this);
				return i;
			}
			bidirectional_iterator& operator--()
			{
				//we have a problem here when the ptr NULL 
				// so lets get the max value is th ene
				AvlNode<value_type, Alloc> *node = NULL;
				if (!_ptr)
				{
					node = _avl->findMax(_avl->root);
					if (node)
						_ptr = node->data;
					return (*this);
				}
				node = _avl->find(_avl->root, *_ptr);
				if (node)
				{
					AvlNode<value_type, Alloc> *succ = _avl->predecessor(*_ptr);
					if (succ)
						_ptr = succ->data;
					else
						_ptr = NULL;
				}
				return (*this);
			}
			bidirectional_iterator operator--(int)
			{
				bidirectional_iterator i = *this;
				--(*this);
				return i;
			}
			difference_type operator-(bidirectional_iterator src)const { return (_ptr - src._ptr);}
			reference operator*()const { return *_ptr;}
			pointer operator->() const{ return _ptr;}
			friend bool operator== (const bidirectional_iterator& lhs, const bidirectional_iterator& rhs)
			{
				return (lhs._ptr == rhs._ptr);
			}
			friend bool operator!= (const bidirectional_iterator& lhs, const bidirectional_iterator& rhs)
			{
				return !(lhs == rhs);
			}
		public:
			pointer _ptr;
			const AVL<value_type, Compare, Alloc>* _avl;
	};
}