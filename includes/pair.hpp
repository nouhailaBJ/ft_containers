/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pair.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbjaghou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 20:44:38 by nbjaghou          #+#    #+#             */
/*   Updated: 2021/12/16 20:44:40 by nbjaghou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

namespace ft
{
	template <class T1, class T2>
	class pair
	{
		public:
			/*** Member variables ***/
			T1 first;
			T2 second;

			/*** Member types ***/
			typedef T1 first_type;
			typedef T2 second_type;
			/*** Member functions ***/

			//default (Constructs a pair object with its elements value-initialized)	
			pair(): first(first_type()), second(second_type()) {}

			//copy
			template<class U, class V> 
			pair (const pair<U,V>& pr): first(pr.first), second(pr.second){}

			//initialization 
			pair (const first_type& a, const second_type& b): first(a), second(b) {}

			/*** operator= ***/
			pair& operator= (const pair& pr)
			{
				first = pr.first;
				second = pr.second;
				return (*this);
			}
	};
	//Construct pair object (with its first element set to x and its second element set to y)
	template <class T1,class T2>
	pair<T1,T2> make_pair (T1 x, T2 y)
	{
		return (pair<T1, T2>(x,y));
	}
	/** Non-member function overloads **/
	// when  we compare two pair we compare the first of lhs with the first of rhs and the second also the same
	template <class T1, class T2>
	bool operator== (const pair<T1,T2>& lhs, const pair<T1,T2>& rhs)
	{
		return (lhs.first == rhs.first && lhs.second == rhs.second);
	}

	template <class T1, class T2>
	bool operator!=(const pair<T1,T2>& lhs, const pair<T1,T2>& rhs)
	{
		return !(lhs == rhs);
	}

	template <class T1, class T2>
	bool operator< (const pair<T1,T2>& lhs, const pair<T1,T2>& rhs)
	{
		return (lhs.first < rhs.first || (!(rhs.first < lhs.first) && lhs.second < rhs.second));
	}

	template <class T1, class T2>
	bool operator<=(const pair<T1,T2>& lhs, const pair<T1,T2>& rhs)
	{
		return !(rhs < lhs);
	}

	template <class T1, class T2>
	bool operator>(const pair<T1,T2>& lhs, const pair<T1,T2>& rhs)
	{
		return (rhs < lhs);
	}

	template <class T1, class T2>
	bool operator>=(const pair<T1,T2>& lhs, const pair<T1,T2>& rhs)
	{
		return !(lhs < rhs);
	}
}
