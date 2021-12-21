/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AVL.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nbjaghou <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/16 20:24:57 by nbjaghou          #+#    #+#             */
/*   Updated: 2021/12/16 20:24:59 by nbjaghou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>

namespace ft
{
	template <typename T, class Alloc>
	class AvlNode
	{
		public:
			AvlNode	*left;
			AvlNode	*right;
			AvlNode	*parent;
			T	*data;
			int height;
			Alloc _alloc;
			AvlNode()
			{
				left = NULL;
				right = NULL;
				parent = NULL;
				data = NULL;
				height = 0;
			}
			AvlNode(const T &val)
			{
				left = NULL;
				right = NULL;
				parent = NULL;
				data = _alloc.allocate(1);
				_alloc.construct(data, val);
				height = 1;
			}
			AvlNode &operator=(AvlNode const &src)
			{
				if (this == &src)
					return (*this);
				if(data)
					_alloc.deallocate(data, 1);
				left = src.left;
				right = src.right;
				parent = src.parent;
				height = src.height;
				data = _alloc.allocate(1);
				_alloc.construct(data, *src.data);
				return (*this);
			}
			~AvlNode() {}
	};
	template <typename T, class Compare, class Alloc>
	class AVL
	{
		public:
			typedef T									value_type;
			typedef Alloc								myAllocator;
			typedef Compare 							key_compare;
			AvlNode<value_type, Alloc>					*root;
			// allocator has a member type rebind for allocate objects of another type
			//The advantage of rebind is to allow the user of your template to specify the allocator type only for a single allocated type
			typename Alloc::template rebind<AvlNode<value_type, Alloc> >::other	n_alloc; 
			/*********** Constructors and destructors ************/
			AVL(): root(NULL){}
			AVL(const AVL &src)
			{
				*this = src;
			}
			AVL &operator=(AVL const &src)
			{
				if (this == &src)
					return (*this);
				root = src.root;
				_cmp = src._cmp;
				return (*this);
			}
			~AVL(){}

			/*********** ROTATIONS ************/
			/*
				Right rotation : x - y - z
							z				y
						y	  T4  ====>  x		z
					x	  T3		   T1 T2  T3  T4 
				T1  T2
				- first lets set y with z->left
				- set y->right  to the z->left
				- if y->right not null the lets reset the parent to be z
				- lets put x in z->right
			*/
			AvlNode<value_type, Alloc> *RightRotate(AvlNode<value_type, Alloc> *z)
			{
				AvlNode<value_type, Alloc> *y = z->left; // point at y
				if (!y) return (z);
				AvlNode<value_type, Alloc> *T2 = y->right;
				if (y->right != NULL)
					y->right->parent = z; // update the parent of y->right
				y->right = z; // now lets set in the right of y the z node
				z->left = T2;
				y->parent = z->parent;
				z->parent = y;
				z->height = std::max(getHeight(z->left), getHeight(z->right)) + 1;
				y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;
				return (y);
			}
			AvlNode<value_type, Alloc> *LeftRotate(AvlNode<value_type, Alloc> *z)
			{
				AvlNode<value_type, Alloc> *y = z->right; // point at y
				AvlNode<value_type, Alloc> *T2 = y->left;
				if (y->left != NULL)
					y->left->parent = z; // update the parent of y->right
				y->left = z; // now lets set in the right of y the z node
				z->right = T2;
				y->parent = z->parent;
				z->parent = y;
				z->height = std::max(getHeight(z->left), getHeight(z->right)) + 1;
				y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;
				return (y);
			}
			/******* get height of node *********/
			int getHeight(AvlNode<value_type, Alloc> *node)
			{
				if (!node) return (0);
				return (node->height);
			}
			/******* find node if exist *********/
			AvlNode<value_type, Alloc> *find(AvlNode<value_type, Alloc> *node, const value_type &key) const
			{
				if (!node )
					return (node);
				if (node->data && node->data->first == key.first)
					return (node);
				if (_cmp(key.first, node->data->first))
					return (find(node->left, key));
				else if (_cmp(node->data->first, key.first))
					return (find(node->right, key));
				return (node);
			}
			/************** Insert ***********/
			/*
				insert new node into the tree then rebalance it
				the new node alays inserted as leaf node with balane factor equal to 0
				Algorithm:
					- using the recursive steps compare the key with root key of the current tree
					- if key < RootKey => call insertion in the left of th current node until the leaf node is reached
					- else if keu > RootKey => call insertion in the right of th current node until the leaf node is reached
					- else return leafNode
					- update the balanceFactor , if no belong to {-1, 0, 1} then rebalance it
						- Rebalance: 
							- if BF > 1 => it mean the height of left is greater so do the right rotation f LeftRight rotation
								- if key < leftChildKey => rightRotation else LR rotation
							- if BF < -1 => it mean the height of right is greater so do the left rotation f RightLeft rotation
								- if key > rightChileKey => left Rotate else RL rotate
			*/
			AvlNode<value_type, Alloc> * insert(value_type key)
			{
				this->root = this->insert(this->root, key);
				return (root);
			}
			AvlNode<value_type, Alloc> *newNode(value_type const &key)
			{
				AvlNode<value_type, Alloc>* node = n_alloc.allocate(1);
				n_alloc.construct(node, key);
				return (node);
			}

			AvlNode<value_type, Alloc> *rebalance(AvlNode<value_type, Alloc> *node, value_type const &key)
			{
				// calculate the balance factor
				int BalanceFactor = getHeight(node->left) - getHeight(node->right);
				// bf > 1 && key < node->data->first RightRotae
				if (BalanceFactor > 1)
				{
					if (_cmp(key.first, node->left->data->first))
						return (RightRotate(node));
					else
					{
						node->left = LeftRotate(node->left);
						return (RightRotate(node));
					}
				}
				if (BalanceFactor < -1)
				{
					if (_cmp(node->right->data->first, key.first))
						return (LeftRotate(node));
					else
					{
						node->right = RightRotate(node->right);
						return (LeftRotate(node));
					}
				}
				return (node);
			}
			// _cmp is true when less 
			// _cmp is not true when is greater or equal
			AvlNode<value_type, Alloc> *insert(AvlNode<value_type, Alloc> *node, value_type const &key)
			{
				if (!node)
					return (newNode(key));
				if (_cmp(key.first, node->data->first))
				{
					node->left = insert(node->left, key);
					node->left->parent = node;
				}
				else if (_cmp(node->data->first, key.first))
				{
					node->right = insert(node->right, key);
					node->right->parent = node;
				}
				else 
					return (node);
				// update the height
				node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
				// then lets rebalance if unbalanced
				return (rebalance(node, key));
				return (node);
			}

			/************* Deletion ****************/
			/*
				Delete a node :
					A node is always deleted as a leaf node
					- if the node is a leaf then just put null
					- if the node has one child then set swap the content and the change the parent
					- if the node has two child then fin the inorder Successor (min val in right child) swap it with the node to be deleted then remove the leaf node	
			*/
			bool remove(value_type key)
			{
				if (find(root, key))
				{
					this->root = this->remove(this->root, key);
					return (true);
				}
				return (false);
			}
			AvlNode<value_type, Alloc> *remove(AvlNode<value_type, Alloc> *node, value_type const &key)
			{
				if (!node)
					return (node);
				if (_cmp(key.first, node->data->first))
					node->left = remove(node->left, key);
				else if (_cmp(node->data->first, key.first))
					node->right = remove(node->right, key);
				else if (node->data->first == key.first)
				{
					// here the key iis same like the node->data->first
					//with only one child or no child
					if (!node->left || !node->right)
					{
						AvlNode<value_type, Alloc> *tmp = node->left ? node->left : node->right;
						if (!tmp)
						{
							tmp = node;
							node = NULL;
						}
						else
						{
							// when we copy the content we need to update the parent
							// the parent of our node becode the parent of the child of our node
							AvlNode<value_type, Alloc> *np = node->parent;
							*node = *tmp;
							node->parent = np;
						}
						_alloc.deallocate(tmp->data , 1);
						n_alloc.deallocate(tmp, 1);
					}
					else
					{
						// get the successor(the min value in the right)
						AvlNode<value_type, Alloc> *tmp = findMin(node->right);
						_alloc.construct(node->data, *tmp->data); // coy the successor to the what i want to remove
						// delete the successor from  my node
						node->right = remove(node->right, *tmp->data);
					}
				}
				if (!node)
					return (node);
				// update the height
				node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
				return (rebalance(node, key));
			}
			/********  Min and Max value ****/
			AvlNode<value_type, Alloc> *findMin(AvlNode<value_type, Alloc> *node) const
			{
				if (!node)
					return (NULL);
				AvlNode<value_type, Alloc> *min = node;
				while (min->left)
					min = min->left;
				return (min);
			}
			AvlNode<value_type, Alloc> *findMax(AvlNode<value_type, Alloc> *node) const
			{
				if (!node)
					return (NULL);
				AvlNode<value_type, Alloc> *max = node;
				while (max->right)
					max = max->right;
				return (max);
			}
			AvlNode<value_type, Alloc> * successor(const value_type &ptr) const
			{
				AvlNode<value_type, Alloc> *node = find(root, ptr);
				if (!node)
					return (NULL);
				// step 1 : if the node have a right subtree so lets foound the min value in right child
				if (node->right != NULL)
					return (findMin(node->right));
				//step 2: if there is no right child lets go to the parent and check the 
				// the node->parent->right is not the node because its will be readed before
				AvlNode<value_type, Alloc> *succ = node->parent;
				while (succ != NULL && node == succ->right)
				{
					node = succ;
					succ = succ->parent;
				}
				node = succ;
				return (node);
			}
			AvlNode<value_type, Alloc> * predecessor(const value_type &ptr) const
			{
				AvlNode<value_type, Alloc> *node = find(root, ptr);
				if (!node)
					return (NULL);
				// step 1 : if the node have a left subtree so lets found the max value
				if (node->left != NULL)
					return (findMax(node->left));
				//step 2: if there is no right child lets go to the parent and check the 
				// the node->parent->right is not the node because its will be readed before
				AvlNode<value_type, Alloc> *succ = node->parent;
				while (succ != NULL && node == succ->left)
				{
					node = succ;
					succ = succ->parent;
				}
				node = succ;
				return (node);
			}
			/********** Print our Order *******/
			void PreOrder(AvlNode<value_type, Alloc> *node)
			{
				if (node == NULL)
					return;
				PreOrder(node->left);
				_alloc.deallocate(node->data, 1);
				PreOrder(node->right);
				n_alloc.deallocate(node, 1);
				node = NULL;
			}
			void assign(const AvlNode<value_type, Alloc> *node)
			{
				if (!node)
					return ;
				assign(node->left);
				if (node->data)
					insert(*(node->data));
				assign(node->right);
			}
			private:
				key_compare _cmp;
				myAllocator _alloc;
	};
}