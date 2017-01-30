//project start time: 11-28-2016
//project finish time: 12-08-2016
//author: hank zhang

/**
* implement a container like std::map
*/
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include <iostream>
#include <string>
#include "utility.hpp"
#include "exceptions.hpp"

#define L 0
#define R 1

namespace sjtu
{
	template
		< class Key, class T, class Compare = std::less<Key> >
		class map
	{
	public:
		/**
		* the internal type of vt.
		* it should have a default constructor, a copy constructor.
		* You can use sjtu::map as value_type by typedef.
		*/

		typedef pair<const Key, T> value_type;
	private:
		struct RBTNode // red = 0  black = 1
		{
			value_type vt;
			bool color;
			RBTNode *left, *right, *parent;
			RBTNode(const value_type &other) :vt(other), left(nullptr), right(nullptr), parent(nullptr), color(false) {}
		};

		RBTNode *root;
		RBTNode *endNd;
		size_t sz;
		Compare cmp;

		RBTNode *cpyTree(RBTNode *other)
		{
			if (!other)
				return nullptr;
			RBTNode* nd = new RBTNode(other->vt);
			nd->color = other->color;
			nd->left = cpyTree(other->left);
			if (nd->left)
				nd->left->parent = nd;
			nd->right = cpyTree(other->right);
			if (nd->right)
				nd->right->parent = nd;
			return nd;
		}

		void des(RBTNode *nd)
		{
			if (!nd) return;
			des(nd->left);
			des(nd->right);
			delete nd;
			sz--;
		}

		//Rotates
		void LLb(RBTNode *nd)
		{
			RBTNode *tmp1, *tmp2, *tmp3, *tmp4;
			tmp1 = nd;
			tmp2 = nd->left;
			tmp3 = nd->right;
			tmp4 = nd->left->left;
			tmp2->parent = tmp1->parent;
			if (tmp1->parent)
			{
				if (getPos(tmp1) == L)
					tmp2->parent->left = tmp2;
				else
					tmp2->parent->right = tmp2;
			}
			else root = tmp2;
			tmp1->left = tmp2->right;
			if (tmp2->right)
				tmp2->right->parent = tmp1;
			tmp2->right = tmp1;
			tmp1->parent = tmp2;
		}

		void RRb(RBTNode *nd)
		{
			RBTNode *tmp1, *tmp2, *tmp3, *tmp4;
			tmp1 = nd;
			tmp2 = nd->left;
			tmp3 = nd->right;
			tmp4 = nd->right->right;
			tmp3->parent = tmp1->parent;
			if (tmp1->parent)
			{
				if (getPos(tmp1) == L) tmp3->parent->left = tmp3;
				else tmp3->parent->right = tmp3;
			}
			else root = tmp3;
			tmp1->right = tmp3->left;
			if (tmp3->left)
				tmp3->left->parent = tmp1;
			tmp3->left = tmp1;
			tmp1->parent = tmp3;
		}

		inline void LRb(RBTNode *nd) { RRb(nd->left); LLb(nd); }
		inline void RLb(RBTNode *nd) { LLb(nd->right); RRb(nd); }
		inline void LL(RBTNode *nd) { LLb(nd); nd->color = false; nd->parent->color = true; }
		inline void RR(RBTNode *nd) { RRb(nd); nd->color = false; nd->parent->color = true; }
		inline void LR(RBTNode *nd) { LRb(nd); nd->color = false; nd->parent->color = true; }
		inline void RL(RBTNode *nd) { RLb(nd); nd->color = false; nd->parent->color = true; }
		inline void brush(RBTNode *nd) { nd->color ^= 1; nd->left->color ^= 1; nd->right->color ^= 1; }
		inline bool getColor(RBTNode *nd)
		{
			if (!nd || nd->color == true)
				return true;
			else return false;
		}

		inline RBTNode* getBro(RBTNode *nd)
		{
			if (nd->parent->left == nd) return nd->parent->right;
			else return nd->parent->left;
		}

		inline bool getPos(RBTNode *nd)
		{
			if (nd->parent->left == nd) return L;
			else return R;
		}

		void fix(RBTNode *pos)
		{
			if (pos && pos->color == true && pos != root && getBro(pos)->color == true)
			{
				RBTNode *bro = getBro(pos);
				bool pColor = pos->parent->color;
				if (getColor(bro->left) == false && getColor(bro->right) == false)
				{
					bro->parent->color = true;
					if (getPos(pos) == L)
					{
						bro->left->color = pColor;
						RLb(pos->parent);
					}
					else
					{
						bro->right->color = pColor;
						LRb(pos->parent);
					}
				}
				else if (getColor(bro->left) == false || getColor(bro->right) == false)
				{
					bro->parent->color = true;
					if (getPos(pos) == L)
					{
						if (getColor(bro->right) == false)
						{
							bro->color = pColor;
							bro->right->color = true;
							RRb(pos->parent);
						}
						else
						{
							bro->left->color = pColor;
							RLb(pos->parent);
						}
					}
					else
					{
						if (getColor(bro->left) == false)
						{
							bro->color = pColor;
							bro->left->color = true;
							LLb(pos->parent);
						}
						else
						{
							bro->right->color = pColor;
							LRb(pos->parent);
						}
					}
				}
				else
				{
					bro->color = false;
					if (pos->parent->color == false)
						pos->parent->color = true;
					else if (pos->parent == root);
					else
					{
						RBTNode *tmp = pos->parent;
						while (tmp->color == true && tmp != root && getColor(getBro(tmp)) == true &&
							getColor(getBro(tmp)->left) == true && getColor(getBro(tmp)->right) == true)
						{
							getBro(tmp)->color = false;
							tmp = tmp->parent;
						}

						if (tmp == root);
						else if (tmp->color == false)
							tmp->color = true;
						else if (getColor(getBro(tmp)) == false)
						{
							if (getPos(tmp) == L)
								RRb(tmp->parent);
							else
								LLb(tmp->parent);
							tmp->parent->parent->color = true;
							tmp->parent->color = false;
							fix(tmp);
						}
						else
							fix(tmp);
					}
				}
			}
		}

	public:
		RBTNode *findNd(const Key &key, RBTNode *nd)
		{
			if (!nd)
				return nullptr;
			if (!cmp(nd->vt.first, key) && !cmp(key, nd->vt.first))
				return nd;
			else
			{
				if (!cmp(key, nd->vt.first))
					return findNd(key, nd->right);
				else
					return findNd(key, nd->left);
			}
		}

		const RBTNode *findNd(const Key &key, RBTNode *nd) const
		{
			if (!nd)
				return nullptr;
			if (!cmp(nd->vt.first, key) && !cmp(key, nd->vt.first)) return nd;
			else
			{
				if (!cmp(key, nd->vt.first))
					return findNd(key, nd->right);
				else
					return findNd(key, nd->left);
			}
		}

		RBTNode *findP(const Key &key, RBTNode *nd)
		{
			if (cmp(nd->vt.first, key))
			{
				if (nd->right)
					return findP(key, nd->right);
				else return nd;
			}
			else
			{
				if (nd->left)
					return findP(key, nd->left);
				else return nd;
			}
		}
	public:
		/**
		* see BidirectionalIterator at CppReference for help.
		*
		* if there is anything wrong throw invalid_iterator.
		*     like it = map.begin(); --it;
		*       or it = map.end(); ++end();
		*/
		class const_iterator;
		class iterator {
		public:
			/**
			* TODO add vt members
			*   just add whatever you want.
			*/
			RBTNode *nd;
			RBTNode *itrRt;
			RBTNode *endNd;

			iterator() :nd(nullptr), itrRt(nullptr), endNd(nullptr) {}
			iterator(const iterator &other) { nd = other.nd; itrRt = other.itrRt; endNd = other.endNd; }
			iterator(const const_iterator &other) { nd = other.nd; itrRt = other.itrRt; endNd = other.endNd; }
			iterator(RBTNode *n, RBTNode *r, RBTNode *e) { nd = n; itrRt = r; endNd = e; }
			/**
			* return a new iterator which pointer n-next elements
			*   even if there are not enough elements, just return the answer.
			* as well as operator-
			*/
			/**
			* TODO iter++
			*/
			iterator operator++(int)
			{
				if (nd == endNd)
				{
					throw invalid_iterator();
				}
				iterator tmp = *this;
				if (nd->right != nullptr)
				{
					nd = nd->right;
					while (nd->left) { nd = nd->left; }
					return tmp;
				}
				if (!nd->parent)
				{
					nd = endNd;
					return tmp;
				}
				if (nd->parent->left == nd)
				{
					nd = nd->parent;
					return tmp;
				}
				else
				{
					while (nd->parent->right == nd)
					{
						nd = nd->parent;
						if (!nd->parent) { nd = endNd; return tmp; }
					}
					nd = nd->parent;
					return tmp;
				}
			}
			/**
			* TODO ++iter
			*/
			iterator & operator++()
			{
				if (nd == endNd)
				{
					throw invalid_iterator();
				}
				if (nd->right != nullptr)
				{
					nd = nd->right;
					while (nd->left) { nd = nd->left; }
					return *this;
				}
				if (!nd->parent)
				{
					nd = endNd;
					return *this;
				}
				if (nd->parent->left == nd)
				{
					nd = nd->parent;
					return *this;
				}
				else
				{
					while (nd->parent->right == nd)
					{
						nd = nd->parent;
						if (!nd->parent) { nd = endNd; return *this; }
					}
					nd = nd->parent;
					return *this;
				}
			}
			/**
			* TODO iter--
			*/
			iterator operator--(int)
			{
				iterator tmp = *this;
				if (!nd || nd == endNd)
				{
					nd = itrRt;
					if (!nd) nd = endNd;
					while (nd->right) nd = nd->right;
					return tmp;
				}
				if (nd->left != nullptr)
				{
					nd = nd->left;
					while (nd->right) { nd = nd->right; }
					return tmp;
				}
				while (nd->parent && nd->parent->left == nd)
				{
					nd = nd->parent;
				}
				if (!nd->parent)
				{
					nd = nullptr;
					return tmp;
				}
				nd = nd->parent;
				return tmp;
			}
			/**
			* TODO --iter
			*/
			iterator & operator--()
			{
				if (!nd || nd == endNd)
				{
					nd = itrRt;
					if (!nd) nd = endNd;
					while (nd->right) nd = nd->right;
					return *this;
				}
				if (nd->left != nullptr)
				{
					nd = nd->left;
					while (nd->right) { nd = nd->right; }
					return *this;
				}
				while (nd->parent && nd->parent->left == nd)
				{
					nd = nd->parent;
				}
				if (!nd->parent)
				{
					nd = nullptr;
					return *this;
				}
				nd = nd->parent;
				return *this;
			}
			/**
			* a operator to check whether two iterators are same (pointing to the same memory).
			*/
			value_type & operator*() const { return nd->vt; }
			bool operator==(const iterator &rhs) const { return nd == rhs.nd; }
			bool operator==(const const_iterator &rhs) const { return nd == rhs.nd; }
			/**
			* some other operator for iterator.
			*/
			bool operator!=(const iterator &rhs) const { return nd != rhs.nd; }
			bool operator!=(const const_iterator &rhs) const { return nd != rhs.nd; }

			iterator &operator=(RBTNode *n) { nd = n; return *this; }
			/*
			* for the support of it->first.
			* See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
			*/
			value_type* operator->() const noexcept { return &(nd->vt); }
		};
		class const_iterator {
			// it should has similar member method as const_iterator.
			//  and it should be able to construct from an const_iterator.
		public:
			/**
			* TODO add vt members
			*   just add whatever you want.
			*/
			const RBTNode *nd;
			const RBTNode *itrRt;
			const RBTNode *endNd;

			const_iterator() :nd(nullptr), itrRt(nullptr), endNd(nullptr) {}
			const_iterator(const const_iterator &other) { nd = other.nd; itrRt = other.itrRt; endNd = other.endNd; }
			const_iterator(const iterator &other) { nd = other.nd; itrRt = other.itrRt; endNd = other.endNd; }
			const_iterator(const RBTNode *n, const RBTNode *r, const RBTNode *e) { nd = n; itrRt = r; endNd = e; }
			/**
			* return a new const_iterator which pointer n-next elements
			*   even if there are not enough elements, just return the answer.
			* as well as operator-
			*/
			/**
			* TODO iter++
			*/
			const_iterator operator++(int)
			{
				if (nd == endNd)
				{
					throw invalid_iterator();
				}
				const_iterator tmp = *this;
				if (nd->right != nullptr)
				{
					nd = nd->right;
					while (nd->left) { nd = nd->left; }
					return tmp;
				}
				if (!nd->parent)
				{
					nd = endNd;
					return tmp;
				}
				if (nd->parent->left == nd)
				{
					nd = nd->parent;
					return tmp;
				}
				else
				{
					while (nd->parent->right == nd)
					{
						nd = nd->parent;
						if (!nd->parent) { nd = endNd; return tmp; }
					}
					nd = nd->parent;
					return tmp;
				}
			}
			/**
			* TODO ++iter
			*/
			const_iterator & operator++()
			{
				if (nd == endNd)
				{
					throw invalid_iterator();
				}
				if (nd->right != nullptr)
				{
					nd = nd->right;
					while (nd->left) { nd = nd->left; }
					return *this;
				}
				if (!nd->parent)
				{
					nd = endNd;
					return *this;
				}
				if (nd->parent->left == nd)
				{
					nd = nd->parent;
					return *this;
				}
				else
				{
					while (nd->parent->right == nd)
					{
						nd = nd->parent;
						if (!nd->parent) { nd = endNd; return *this; }
					}
					nd = nd->parent;
					return *this;
				}
			}
			/**
			* TODO iter--
			*/
			const_iterator operator--(int)
			{
				const_iterator tmp = *this;
				if (!nd || nd == endNd)
				{
					nd = itrRt;
					if (!nd) nd = endNd;
					while (nd->right) nd = nd->right;
					return tmp;
				}
				if (nd->left != nullptr)
				{
					nd = nd->left;
					while (nd->right) { nd = nd->right; }
					return tmp;
				}
				while (nd->parent && nd->parent->left == nd)
				{
					nd = nd->parent;
				}
				if (!nd->parent)
				{
					nd = nullptr;
					return tmp;
				}
				nd = nd->parent;
				return tmp;
			}
			/**
			* TODO --iter
			*/
			const_iterator & operator--()
			{
				if (!nd || nd == endNd)
				{
					nd = itrRt;
					if (!nd) nd = endNd;
					while (nd->right) nd = nd->right;
					return *this;
				}
				if (nd->left != nullptr)
				{
					nd = nd->left;
					while (nd->right) { nd = nd->right; }
					return *this;
				}
				while (nd->parent && nd->parent->left == nd)
				{
					nd = nd->parent;
				}
				if (!nd->parent)
				{
					nd = nullptr;
					return *this;
				}
				nd = nd->parent;
				return *this;
			}
			/**
			* a operator to check whether two iterators are same (pointing to the same memory).
			*/
			const value_type & operator*() const { return nd->vt; }
			bool operator==(const iterator &rhs) const { return nd == rhs.nd; }
			bool operator==(const const_iterator &rhs) const { return nd == rhs.nd; }
			/**
			* some other operator for const_iterator.
			*/
			bool operator!=(const iterator &rhs) const { return nd != rhs.nd; }
			bool operator!=(const const_iterator &rhs) const { return nd != rhs.nd; }

			const_iterator &operator=(RBTNode *n) { nd = n; return *this; }
			/*
			* for the support of it->first.
			* See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
			*/
			const value_type* operator->() const noexcept { return &(nd->vt); }
		};
		/**
		* TODO two constructors
		*/
		map() :root(nullptr), endNd(nullptr), sz(0) {}
		map(const map &other)
		{
			root = cpyTree(other.root);
			sz = other.sz;
			/*if (endNd)
				delete endNd;
			if (root)
				endNd = new RBTNode(root->vt);
			else */
				endNd = nullptr;
		}
		/**
		* TODO assignment operator
		*/
		map & operator=(const map &other)
		{
			if (this == &other) 
				return *this;
			delete endNd;
			if (root && other.root)
				endNd = new RBTNode(other.root->vt);
			else
				endNd = nullptr;
			des(root);
			root = cpyTree(other.root);
			sz = other.sz;
			return *this;
		}
		/**
		* TODO Destructors
		*/
		~map()
		{
			if (root)
				delete endNd;
			des(root);
		}
		/**
		* TODO
		* access specified element with bounds checking
		* Returns a reference to the mapped value of the element with key equivalent to key.
		* If no such element exists, an exception of type `index_out_of_bound'
		*/
		T & at(const Key &key)
		{
			RBTNode *target = findNd(key, root);
			if (!target) throw index_out_of_bound();
			return target->vt.second;
		}
		const T & at(const Key &key) const
		{
			const RBTNode *target = findNd(key, root);
			if (!target) throw index_out_of_bound();
			return target->vt.second;
		}
		/**
		* TODO
		* access specified element
		* Returns a reference to the value that is mapped to a key equivalent to key,
		*   performing an insertion if such key does not already exist.
		*/
		T & operator[](const Key &key)
		{
			RBTNode *target = findNd(key, root);
			if (!target)
			{
				T t;
				value_type value(key, t);
				pair<iterator, bool> ans = insert(value);
				return ans.first->second;
			}
			else return target->vt.second;
		}
		/**
		* behave like at() throw index_out_of_bound if such key does not exist.
		*/
		const T & operator[](const Key &key) const
		{
			const RBTNode *target = findNd(key, root);
			if (!target)
				throw index_out_of_bound();
			else return target->vt.second;
		}
		/**
		* return a iterator to the beginning
		*/
		iterator begin()
		{
			RBTNode *target = root;
			if (!target)
			{
				iterator ans(endNd, root, endNd);
				return ans;
			}
			else
			{
				while (target->left) target = target->left;
				iterator ans(target, root, endNd);
				return ans;
			}
		}
		const_iterator cbegin() const
		{
			RBTNode *target = root;
			if (!target)
			{
				const_iterator ans(target, root, endNd);
				return ans;
			}
			else
			{
				while (target->left) target = target->left;
				const_iterator ans(target, root, endNd);
				return ans;
			}
		}
		/**
		* return a iterator to the end
		* in fact, it returns past-the-end.
		*/
		iterator end()
		{
			iterator ans(endNd, root, endNd);
			return ans;
		}
		const_iterator cend() const
		{
			const_iterator ans(endNd, root, endNd);
			return ans;
		}
		/**
		* return an iterator to the tail
		*/
		iterator tail()
		{
			RBTNode *target = root;
			if (!target)
			{
				iterator ans(target, root, endNd);
				return ans;
			}
			else
			{
				while (target->right) target = target->right;
				iterator ans(target, root, endNd);
				return ans;
			}
		}
		const_iterator ctail() const
		{
			RBTNode *target = root;
			if (!target)
			{
				const_iterator ans(target, root, endNd);
				return ans;
			}
			else
			{
				while (target->right) target = target->right;
				const_iterator ans(target, root, endNd);
				return ans;
			}
		}
		/**
		* checks whether the container is empty
		* return true if empty, otherwise false.
		*/
		bool empty() const { return (sz == 0); }
		/**
		* returns the number of elements.
		*/
		size_t size() const { return sz; }
		/**
		* clears the contents
		*/
		void clear()
		{
			if (root) { delete endNd; endNd = nullptr; }
			des(root);
			root = nullptr;
			sz = 0;
		}
		/**
		* insert an element.
		* return a pair, the first of the pair is
		*   the iterator to the new element (or the element that prevented the insertion),
		*   the second one is true if insert successfully, or false.
		*/
		pair<iterator, bool> insert(const value_type &value)
		{
			pair<iterator, bool> ans;
			if (!root)
			{
				RBTNode *newNd = new RBTNode(value);
				root = newNd;
				ans.first = root;
				ans.second = true;
				sz++;
				if (!endNd) endNd = new RBTNode(root->vt);
			}
			else
			{
				RBTNode *target = findNd(value.first, root);
				if (target)
				{
					ans.first = target;
					ans.second = false;
				}
				else
				{
					RBTNode *father = findP(value.first, root);
					RBTNode *newNd = new RBTNode(value);
					newNd->parent = father;
					ans.first = newNd;
					ans.second = true;
					sz++;
					if (cmp(father->vt.first, newNd->vt.first)) father->right = newNd;
					else father->left = newNd;
					if (getColor(father) == false)
					{
						if (getColor(getBro(father)) == true)
						{
							if (getPos(father) == L && getPos(newNd) == L)
								LL(father->parent);
							else if (getPos(father) == L && getPos(newNd) == R)
								LR(father->parent);
							else if (getPos(father) == R && getPos(newNd) == L)
								RL(father->parent);
							else RR(father->parent);
						}
						else
						{
							brush(father->parent);
							RBTNode *check = father->parent;
							while (check->parent && check->parent->color == false)
							{
								if (getBro(check->parent)->color == false)
								{
									brush(check->parent->parent);
									check = check->parent->parent;
								}
								else
								{
									if (getPos(check->parent) == L && getPos(check) == L)
										LL(check->parent->parent);
									else if (getPos(check->parent) == L && getPos(check) == R)
										LR(check->parent->parent);
									else if (getPos(check->parent) == R && getPos(check) == L)
										RL(check->parent->parent);
									else
										RR(check->parent->parent);
									break;
								}
							}
						}
					}
				}
			}
			root->color = true;
			return ans;
		}
		/**
		* erase the element at pos.
		*
		* throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
		*/
		void erase(iterator pos)
		{
			if (!pos.nd || pos.nd == endNd || pos.itrRt != root)
				throw index_out_of_bound();
			else
			{
				sz--;
				RBTNode *target = pos.nd;
				if (target->left && target->right)
				{
					iterator tmp = --pos;
					RBTNode *change = tmp.nd, *change_parent = change->parent, *changeL = change->left,
						*changeR = change->right;
					bool change_color = change->color;

					RBTNode *newNd = new RBTNode(*tmp);
					if (getPos(change) == L)
						change_parent->left = newNd;
					else
						change_parent->right = newNd;

					newNd->color = change_color;
					newNd->left = changeL;

					if (newNd->left)
						newNd->left->parent = newNd;
					newNd->right = changeR;
					if (newNd->right)
						newNd->right->parent = newNd;

					newNd->parent = change_parent;

					if (target->parent)
					{
						if (getPos(target) == L)
							target->parent->left = change;
						else
							target->parent->right = change;
					}
					else
						root = change;

					change->left = target->left;
					if (change->left)
						change->left->parent = change;
					change->right = target->right;
					if (change->right)
						change->right->parent = change;

					change->color = target->color;
					change->parent = target->parent;

					delete target;
					target = newNd;
				}
				if (!target->left && !target->right)
				{
					if (target->color == false)
					{
						if (getPos(target) == L)
							target->parent->left = nullptr;
						else
							target->parent->right = nullptr;
						delete target;
					}
					else if (target == root)
					{
						delete root;
						root = nullptr;
						return;
					}
					else
					{
						RBTNode *bro = getBro(target);
						bool pColor = target->parent->color;
						if (getColor(bro) == true)
						{
							fix(target);
							if (getPos(target) == L)
								target->parent->left = nullptr;
							else
								target->parent->right = nullptr;
							delete target;
						}
						else
						{
							target->parent->color = false;
							bro->color = true;
							if (getPos(target) == L)
								RRb(target->parent);
							else
								LLb(target->parent);
							fix(target);
							if (getPos(target) == L)
								target->parent->left = nullptr;
							else
								target->parent->right = nullptr;
							delete target;
						}
					}
				}
				else
				{
					if (target->left)
					{
						if (target == root)
						{
							root = target->left;
							target->left->parent = nullptr;
							delete target;
						}
						else
						{
							RBTNode *tmp = target->left;
							if (getPos(target) == L)
								target->parent->left = target->left;
							else
								target->parent->right = target->left;
							target->left->parent = target->parent;
							target->left->color = true;
							delete target;
						}
					}
					else
					{
						if (target == root)
						{
							root = target->right;
							target->right->parent = nullptr;
							delete target;
						}
						else
						{
							RBTNode *tmp = target->right;
							if (getPos(target) == L)
								target->parent->left = target->right;
							else
								target->parent->right = target->right;
							target->right->parent = target->parent;
							target->right->color = true;
							delete target;
						}
					}
				}
			}
			root->color = true;
		}
		/**
		* Returns the number of elements with key
		*   that compares equivalent to the specified argument,
		*   which is either 1 or 0
		*     since this container does not allow duplicates.
		* The default method of check the equivalence is !(a < b || b > a)
		*/
		size_t count(const Key &key) const
		{
			const RBTNode *target = findNd(key, root);
			if (target) return 1;
			else return 0;
		}
		/**
		* Finds an element with key equivalent to key.
		* key value of the element to search for.
		* Iterator to an element with key equivalent to key.
		*   If no such element is found, past-the-end (see end()) iterator is returned.
		*/
		iterator find(const Key &key)
		{
			RBTNode *target = findNd(key, root);
			if (target) { iterator ans(target, root, endNd); return ans; }
			else { iterator ans(endNd, root, endNd); return ans; }
		}
		const_iterator find(const Key &key) const
		{
			const RBTNode *target = findNd(key, root);
			if (target) { const_iterator ans(target, root, endNd); return ans; }
			else { const_iterator ans(endNd, root, endNd); return ans; }
		}
	};

}

#endif
