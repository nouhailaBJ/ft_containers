/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbjaghou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 20:24:51 by nbjaghou          #+#    #+#             */
/*   Updated: 2021/12/16 20:24:52 by nbjaghou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <memory>
#include <type_traits>
#include <cstddef>
#include "algorithm.hpp"
#include "iterator.hpp"

/*
	Vector like arrays with the ability to resize itself automatically when 
	an element is inserted or deleted

	Some Note:
		- Before a qualified dependent type, you need typename
*/
namespace ft
{
	template<class T, class Alloc = std::allocator<T> >
	class vector
	{
		public:
			/************************ Member types ******************************/
			typedef T												value_type;
			typedef Alloc											allocator_type;
			typedef typename allocator_type::reference				reference;
			typedef typename allocator_type::const_reference		const_reference;
			typedef typename allocator_type::pointer				pointer;
			typedef typename allocator_type::const_pointer			const_pointer;
			typedef ft::random_access_iterator<value_type>			iterator;
			typedef ft::random_access_iterator<const value_type>	const_iterator;
			typedef ft::reverse_iterator<iterator>					reverse_iterator;
			typedef ft::reverse_iterator<const_iterator>			const_reverse_iterator;
			typedef ptrdiff_t										difference_type;
			typedef size_t											size_type;
			/************************ End Member types **************************/

			/************************ Constructors  ******************************/
			explicit vector (const allocator_type& alloc = allocator_type()): _alloc(alloc), _size(0), _capacity(0), _ptr(NULL) {}

			explicit vector (size_type n, const value_type& val = value_type(),
							const allocator_type& alloc = allocator_type())
			{
				_size = n;
				_capacity = n;
				_alloc = alloc;
				_ptr = _alloc.allocate(n);
				for (size_type i = 0; i < n; i++)
					_alloc.construct(_ptr + i, val);
			}

			// we check here if  InputIterator is not an integral type with enable_if that have a cond bool and Type 
			// we named the type (defined only if Cond is true)
			template <class InputIterator>
					vector (InputIterator first, InputIterator last,
							const allocator_type& alloc = allocator_type(), typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type = InputIterator())
			{
				size_type diff = last - first;
				_ptr = _alloc.allocate(diff);
				for (size_type i = 0; i < diff; i++)
				{
					_alloc.construct(_ptr + i, *first);
					first++;
				}
				_size = diff;
				_capacity = diff;
				_alloc = alloc;
			}

			vector (const vector& x)
			{
				_size = 0;
				_capacity = 0;
				*this = x;
			}
			~vector()
			{
				for(size_type i = 0; i < _size; i++)
					_alloc.destroy(_ptr + i);
				if (_capacity)
					_alloc.deallocate(_ptr, _capacity);
			}
			vector& operator= (const vector& x)
			{
				if (this != &x)
				{
					clear();
					if (_capacity)
						_alloc.deallocate(_ptr, _capacity);
					_size = x._size;
					_capacity = x._capacity;
					_alloc = x._alloc;
					if (_capacity)
						_ptr = _alloc.allocate(_capacity);
					for(size_type i = 0; i < _size; i++)
						_alloc.construct(_ptr + i, x._ptr[i]);	
				}
				return (*this);
			}
			/************************ End Constructors  ****************************/


			/**************************** Iterators  ******************************/
			iterator begin() { return (iterator(_ptr));}
			const_iterator begin() const { return (const_iterator(_ptr));}
			iterator end() { return (iterator(_ptr + _size));}
			const_iterator end() const { return (const_iterator(_ptr + _size));}
			// Returns a reverse iterator pointing to the last element in the vector
			reverse_iterator rbegin() {return (reverse_iterator(end()));}
			const_reverse_iterator rbegin() const {return (const_reverse_iterator(end()));}
			reverse_iterator rend() {return (reverse_iterator(begin())); }
			const_reverse_iterator rend() const {return (const_reverse_iterator(begin())); }
			/**************************** End Iterators  ******************************/


			/**************************** Capacity  ******************************/
			size_type size() const{return (_size);}
			//Returns the maximum number of elements that the vector can hold.
			//Returns the maximum theoretically possible value of n, for which the call allocate(n, 0) could succeed
			size_type max_size() const {return (_alloc.max_size());}
			//Resizes the container so that it contains n elements
			//If the requested size is bigger than the current capacity, the capacity is usually doubled until it's large enough
			void resize (size_type n, value_type val = value_type())
			{
				// if the n smaller than size just reduct the size and destroy
				if (n < _size)
				{
					while (_size > n)
					{
						_alloc.destroy(_ptr + _size);
						_size--;
					}
				}
				// if n is greater from size so lets increase our size and insert new elements
				if (n > _size)
				{
					reserve(n);
					for(size_type i = _size; i < n; i++)
					{
						_alloc.construct(_ptr + i, val);
						_size++;
					}
				}
			}
			bool empty() const {return (size() == 0 ? true : false); }
			void reserve (size_type n)
			{
				//If the size requested is greater than the maximum size (vector::max_size), a length_error exception is thrown.
				if (n > max_size())
					throw std::length_error("Reserve error .");
				/*
					Increase the capacity of the vector to a value that's greater or equal to n.
					If n is greater than the current capacity(), new storage is allocated, otherwise the function does nothing.
				*/
				if (n > _capacity)
				{
					// allocate the new_size
					pointer new_ptr = _alloc.allocate(n);
					// copy the old ptr into new_ptr
					for (size_type i = 0; i < _size; i++)
						_alloc.construct(new_ptr + i, _ptr[i]);
					if (_capacity)
						_alloc.deallocate(_ptr, _capacity);
					// swap the contenu
					std::swap(new_ptr, _ptr);
					_capacity = n;
				}
			}
			size_type capacity() const { return (_capacity);}
			/************************** End Capacity  ****************************/
		
			/**************************** Element access:  ******************************/
			reference operator[] (size_type n){return (_ptr[n]);}
			const_reference operator[] (size_type n) const {return (_ptr[n]);}
			reference at (size_type n)
			{
				if (n >= this->size())
					throw (std::out_of_range("Out of range"));
				return (_ptr[n]);
			}
			const_reference at (size_type n) const
			{
				if (n >= this->size())
					throw (std::out_of_range("Out of range"));
				return (_ptr[n]);
			}
			reference front() {return (_ptr[0]);}
			const_reference front() const {return (_ptr[0]);}
			reference back() {return (_ptr[_size - 1]);}
			const_reference back() const {return (_ptr[_size - 1]);}
			/**************************** End  Element access******************************/


			/**************************** Modifiers  ******************************/
			// Assigns new contents to the vector, replacing its current contents, and modifying its size accordingly.
			//destroy all the old elements
			template <class InputIterator>
			void assign (InputIterator first, InputIterator last, typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type = InputIterator())
			{
				size_type diff = last - first;
				clear();
				reserve(diff);
				while (first != last)
					push_back(*first++);
			}
			void assign (size_type n, const value_type& val)
			{
				// n is the new size 
				_size = 0;
				reserve(n);
				for (size_type i = 0; i < n; i++)
					push_back(val);
			}
			void push_back (const value_type& val)
			{
				if (_capacity == 0)
					reserve(1);
				else if (_size + 1 > _capacity)
					reserve(_capacity * 2);
				_alloc.construct(_ptr + _size, val);
				_size++;
			}
			void pop_back()
			{
				if (_size > 0)
				{
					_alloc.destroy(_ptr + _size);
					_size--;
				}
			}
			//inserting new elements before the element at the specified position
			// insert new element before the position 	
			iterator insert (iterator position, const value_type& val)
			{
				vector tmp;
				size_type index_pos = position - begin();
				size_type end_elem = end() - position;

				// first lets store our end elements int tmp
				tmp.assign(position, end());
				// push in the pos the value in our ptr
				_size = index_pos;
				push_back(val);
				// lets push our end elements in our ptr
				for (size_type i = 0; i < end_elem; i++)
					push_back(tmp[i]);
				return (begin() + index_pos); 
			}
			//fill (2)	
			void insert (iterator position, size_type n, const value_type& val)
			{
				vector tmp;
				size_type index_pos = position - begin();
				size_type end_elem = end() - position;

				// first lets store our end elements int tmp
				tmp.assign(position, end());
				
				if (n + _capacity > (_capacity * 2))
					reserve(_size + n);
				else if (!_size)
					reserve(n);
				// push in the pos the value in our ptr
				_size = index_pos;
				for (size_type i = 0; i < n; i++)
					push_back(val);
				// lets push our end elements in our ptr
				for (size_type i = 0; i < end_elem; i++)
					push_back(tmp[i]);
			}
			//range (3)	
			template <class InputIterator>
				void insert (iterator position, InputIterator first, InputIterator last
				, typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type = InputIterator())
			{
				vector tmp;
				difference_type n = last - first;
				size_type index_pos = position - begin();
				size_type end_elem = end() - position;

				// first lets store our end elements int tmp
				tmp.assign(position, end());
				if ((n + _capacity) > (_capacity * 2))
					reserve(_size + n);
				else if (!_size)
					reserve(n);
				// push in the pos the value in our ptr
				_size = index_pos;
				while (first != last)
					push_back(*first++);
				// lets push our end elements in our ptr
				for (size_type i = 0; i < end_elem; i++)
					push_back(tmp[i]);
			}

			// TODO : erase
			// Removes from the vector either a single element (position) or a range of elements ([first,last)).
			//_ptr = &*myIterator : dereference pointer and then take address of object.
			iterator erase (iterator position)
			{
				if (position == end())
					pop_back();
				_alloc.destroy(&*position);
				for (iterator it = position; it != end(); it++)
				{
					_alloc.construct(&*it, *(it + 1));
					_alloc.destroy(&*(it + 1));
				}
				_size--;
				return (position);
			}
			iterator erase (iterator first, iterator last)
			{
				// copy from the last position to the end () and join it in first position
				copy_erase(last, end(), first);
				_size -= last - first;
				return(first);
			}
			//Exchanges the content of the container by the content of x, which is another vector object of the same type. Sizes may differ.
			void swap (vector& x)
			{
				std::swap(_size, x._size);
				std::swap(_capacity, x._capacity);
				std::swap(_ptr, x._ptr);
			}

			//Removes all elements from the vector (which are destroyed), leaving the container with a size of 0
			void clear()
			{
				for (size_type i = 0; i < _size; i++)
					_alloc.destroy(_ptr + i);
				_size = 0;
			}
			/**************************** end Modifiers ******************************/
			allocator_type get_allocator() const {return (_alloc);}
		private:
			allocator_type _alloc;
			size_t _size;
			size_t _capacity;
			pointer _ptr;
			template<class InputIterator, class OutputIterator>
			OutputIterator copy_erase (InputIterator first, InputIterator last, OutputIterator result)
			{
				while (first!=last) {
					*result = *first;
					result++;
					first++;
				}
				return result;
			}
	};
	//Exchange contents of vectors
	template <class T, class Alloc>
	void swap (vector<T,Alloc>& x, vector<T,Alloc>& y)
	{
		x.swap(y);
	}
	//performed by first comparing sizes then use equal to stop at the first  mismatch
	template <class T, class Alloc>
	bool operator== (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
	{
		if (lhs.size() != rhs.size())
			return (false);
		return (ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
	}
	template <class T, class Alloc>
	bool operator!= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
	{
		//!(a==b)
		return !(lhs == rhs);
	}
	//The less-than comparison (operator<) behaves as if using algorithm lexicographical_compare
	template <class T, class Alloc>
	bool operator<  (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
	{
		return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}
	template <class T, class Alloc>
	bool operator<= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
	{
		//	!(b<a)
		return !(rhs < lhs);
	}
	template <class T, class Alloc>
	bool operator>  (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
	{
		//b<a
		return (ft::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end()));
	}
	template <class T, class Alloc>
	bool operator>= (const vector<T,Alloc>& lhs, const vector<T,Alloc>& rhs)
	{
		//!(a<b)
		return !(lhs < rhs);
	}
}