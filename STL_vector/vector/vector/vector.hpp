//programed twice
//all rights belong to HZ(StanwieCB)
//2016-10-11 project started
//2016-10-27 project finished

#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>
#include <vector>

namespace sjtu 
{
	/**
	* a data container like std::vector
	* store data in a successive memory and support random access.
	*/
	template<typename T>
	class vector 
	{
	//
	//
	private:
		T** data;
		int maxSize;
		int currentLength;

		void doubleSpace() 
		{
			T **tmp = data;
			data = new T*[2 * maxSize];
			for (int i = 0; i < currentLength; i++)
			{
				data[i] = new T(*tmp[i]);
			}
			maxSize *= 2;
			for (int i = 0; i < currentLength; i++)
			{
				delete tmp[i];
			}
			delete[] tmp;
		};

	public:
		/**
		* TODO
		* a type for actions of the elements of a vector, and you should write
		*   a class named const_iterator with same interfaces.
		*/
		/**
		* you can see RandomAccessIterator at CppReference for help.
		*/
		class const_iterator;
		class iterator 
		{
		private:
			/**
			* TODO add data members
			*   just add whatever you want.
			*/

			T** idata;
		public:
			//constructor
			iterator(T** p)
			{
				idata = p;
			}
			/**
			* return a new iterator which pointer n-next elements
			*   even if there are not enough elements, just return the answer.
			* as well as operator-
			*/

			//problem: just return the answer?
			iterator operator+(const int &n) const 
			{
				iterator tmp(idata + n);
				return tmp;
			}
			iterator operator-(const int &n) const
			{
				iterator tmp(idata - n);
				return tmp;
			}
			// return th distance between two iterator,
			// if these two iterators points to different vectors, throw invaild_iterator.

			int operator-(const iterator &rhs) const
			{
				if (rhs.idata - data > currentLength - 1 || rhs.idata - data < 0)
				{
					throw invalid_iterator();
				}
				return idata - rhs.idata;
			}
			iterator operator+=(const int &n)
			{
				idata += n;
				return *this;
			}
			iterator operator-=(const int &n)
			{
				idata -= n;
				return *this;
			}
			/**
			* TODO iter++
			*/
			iterator operator++(int) 
			{
				iterator tmp = *this;
				idata++;
				return tmp;
			}
			/**
			* TODO ++iter
			*/
			iterator& operator++() 
			{
				idata++;
				return *this;
			}
			/**
			* TODO iter--
			*/
			iterator operator--(int) 
			{
				iterator tmp = *this;
				idata--;
				return tmp;
			}
			/**
			* TODO --iter
			*/
			iterator& operator--() 
			{
				idata--;
				return *this;
			}
			//get address
			T** address()
			{
				return idata;
			}
			/**
			* TODO *it
			*/
			T& operator*() const 
			{
				return **idata;
			}
			/**
			* a operator to check whether two iterators are same (pointing to the same memory).
			*/
			bool operator==(const iterator &rhs) const 
			{
				return idata == rhs.idata;
			}
			bool operator==(const const_iterator &rhs) const 
			{
				return idata == rhs.idata;
			}
			/**
			* some other operator for iterator.
			*/
			bool operator!=(const iterator &rhs) const 
			{
				return idata != rhs.idata;
			}
			bool operator!=(const const_iterator &rhs) const 
			{
				return idata != rhs.idata;
			}
		};
		/**
		* TODO
		* has same function as iterator, just for a const object.
		*/
		class const_iterator
		{
		private:
			/**
			* TODO add data members
			*   just add whatever you want.
			*/

			//constructing!!!
			T** idata;
		public:
			//constructor
			const_iterator(T** p)
			{
				idata = p;
			}

			/**
			* return a new iterator which pointer n-next elements
			*   even if there are not enough elements, just return the answer.
			* as well as operator-
			*/

			//problem: just return the answer?
			const_iterator operator+(const int &n) const
			{
				const_iterator tmp(idata + n);
				return tmp;
			}
			const_iterator operator-(const int &n) const
			{
				const_iterator tmp(idata - n);
				return tmp;
			}
			// return th distance between two iterator,
			// if these two iterators points to different vectors, throw invaild_iterator.

			int operator-(const const_iterator &rhs) const
			{
				if (rhs.idata - data > currentLength - 1 || rhs.idata - data < 0)
				{
					throw invalid_iterator();
				}
				return idata - rhs.idata;
			}
			const_iterator operator+=(const int &n)
			{
				idata += n;
				return *this;
			}
			const_iterator operator-=(const int &n)
			{
				idata -= n;
				return *this;
			}
			/**
			* TODO iter++
			*/
			const_iterator operator++(int)
			{
				const_iterator tmp = *this;
				idata++;
				return tmp;
			}
			/**
			* TODO ++iter
			*/
			const_iterator& operator++()
			{
				idata++;
				return *this;
			}
			/**
			* TODO iter--
			*/
			const_iterator operator--(int)
			{
				const_iterator tmp = *this;
				idata--;
				return tmp;
			}
			/**
			* TODO --iter
			*/
			const_iterator& operator--()
			{
				idata--;
				return *this;
			}
			//get address
			T** address()
			{
				return idata;
			}
			/**
			* TODO *it
			*/
			T& operator*() const
			{
				return **idata;
			}
			/**
			* a operator to check whether two iterators are same (pointing to the same memory).
			*/
			bool operator==(const iterator &rhs) const
			{
				return idata == rhs.idata;
			}
			bool operator==(const const_iterator &rhs) const
			{
				return idata == rhs.idata;
			}
			/**
			* some other operator for iterator.
			*/
			bool operator!=(const iterator &rhs) const
			{
				return idata != rhs.idata;
			}
			bool operator!=(const const_iterator &rhs) const
			{
				return idata != rhs.idata;
			}
		};
		/**
		* TODO Constructs
		* Atleast three: default constructor, copy constructor and a constructor for std::vector
		*/
		vector() 
		{
			maxSize = 10;
			currentLength = 0;
			data = new T*[maxSize];
		}
		vector(const vector &other) 
		{
			maxSize = other.maxSize;
			currentLength = other.currentLength;
			data = new T*[maxSize];
			for (int i = 0; i < currentLength; i++)
			{
				data[i] = new T(*other.data[i]);
			}
		}
		vector(const std::vector<T> &other)
		{
			int otherLength = other.size();
			maxSize = otherLength * 2;
			currentLength = otherLength;
			data = new T*[maxSize];
			for (int i = 0; i < currentLength; i++)
			{
				data[i] = new T(*other[i]);
			}
		}
		/**
		* TODO Destructor
		*/
		~vector() 
		{
			clear();
		}
		/**
		* TODO Assignment operator
		*/
		vector& operator=(const vector &other)
		{
			if (this == &other)
			{
				return *this;
			}
			clear();
			maxSize = other.maxSize;
			currentLength = other.currentLength;
			data = new T*[maxSize];
			for (int i = 0; i < currentLength; i++)
			{
				data[i] = new T(*other.data[i]);
			}
			return *this;
		}
		/**
		* assigns specified element with bounds checking
		* throw index_out_of_bound if pos is not in [0, size)
		*/
		T & at(const size_t &pos) 
		{
			if (pos<0 || pos>=currentLength)
			{
				throw index_out_of_bound();
			}
			return *data[pos];
		}
		const T & at(const size_t &pos) const 
		{
			if (pos < 0 || pos >= currentLength)
			{
				throw index_out_of_bound();
			}
			return *data[pos];
		}
		/**
		* assigns specified element with bounds checking
		* throw index_out_of_bound if pos is not in [0, size)
		* !!! Pay attentions
		*   In STL this operator does not check the boundary but I want you to do.
		*/
		T& operator[](const size_t &pos) 
		{
			if (pos >= currentLength || pos < 0)
			{
				throw index_out_of_bound();
			}
			return *data[pos];
		}
		const T& operator[](const size_t &pos) const 
		{
			if (pos >= currentLength || pos < 0)
			{
				throw index_out_of_bound();
			}
			return *data[pos];
		}
		/**
		* access the first element.
		* throw container_is_empty if size == 0
		*/
		const T& front() const 
		{
			if (currentLength == 0)
			{
				throw container_is_empty();
			}
			return *data[0];
		}
		/**
		* access the last element.
		* throw container_is_empty if size == 0
		*/
		const T& back() const 
		{
			if (currentLength == 0)
			{
				throw container_is_empty();
			}
			return *data[currentLength - 1];
		}
		/**
		* returns an iterator to the beginning.
		*/
		iterator begin()
		{
			return iterator(data);
		}
		const_iterator cbegin() const 
		{
			return const_iterator(data);
		}
		/**
		* returns an iterator to the end.
		*/
		iterator end() 
		{
			return iterator(data + currentLength);
		}
		const_iterator cend() const 
		{
			return const_iterator(data + currentLength);
		}
		/**
		* checks whether the container is empty
		*/
		bool empty() const 
		{
			if (currentLength == 0)
			{
				return true;
			}
			return false;
		}
		/**
		* returns the number of elements
		*/
		size_t size() const 
		{
			return currentLength;
		}
		/**
		* returns the number of elements that can be held in currently allocated storage.
		*/
		size_t capacity() const 
		{
			return maxSize;
		}
		/**
		* clears the contents
		*/
		void clear()
		{
			for (int i = 0; i < currentLength; i++)
			{
				delete data[i];
			}
			delete[] data;
			currentLength = 0;
			maxSize = 0;
		}
		/**
		* inserts value before pos
		* returns an iterator pointing to the inserted value.
		*/
		iterator insert(iterator pos, const T &value)
		{
			unsigned int ind = pos.address() - data;
			if (currentLength == maxSize)
			{
				doubleSpace();
			}
			data[currentLength] = new T(*data[currentLength - 1]);
			for (int i = currentLength - 1; i > ind; i--)
			{
				*data[i] = *data[i - 1];
			}
			*data[ind] = value;
			currentLength++;
			
			iterator tmp(data+ind);
			return tmp;
		}
		/**
		* inserts value at index ind.
		* after inserting, this->at(ind) == value is true
		* returns an iterator pointing to the inserted value.
		* throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
		*/
		iterator insert(const size_t &ind, const T &value) 
		{
			iterator tmp;
			if (ind > currentLength || ind < 0) 
			{
				throw index_out_of_bound();
				tmp.p = NULL;
			}
			else
			{
				if (currentLength == maxSize)
				{
					doubleSpace();
				}
				data[currentLength] = new T(*data[currentLength - 1]);
				for (int i = currentLength - 1; i > ind; i--)
				{
					*data[i] = *data[i - 1];
				}
				*data[ind] = value;
				currentLength++;
				tmp.p = &data[ind];
			}
			return tmp;
		}
		/**
		* removes the element at pos.
		* return an iterator pointing to the following element.
		* If the iterator pos refers the last element, the end() iterator is returned.
		*/
		iterator erase(iterator pos) 
		{
			unsigned int ind = pos.address() - data;
			for (int i = ind; i < currentLength - 1; i++)
			{
				*data[i] = *data[i + 1];
			}
			currentLength--;
			delete data[currentLength];
			return pos;
		}
		/**
		* removes the element with index ind.
		* return an iterator pointing to the following element.
		* throw index_out_of_bound if ind >= size
		*/
		iterator erase(const size_t &ind) 
		{
			iterator res;
			if (ind >= currentLength || ind < 0)
			{
				throw index_out_of_bound();
			}
			else 
			{
				for (int i = ind; i < currentLength - 1; i++) 
				{
					*data[i] = *data[i + 1];
				}
				currentLength--;
				delete data[currentLength];

				if (ind == currentLength + 1)
				{
					res.p = end();
				}
				else res.p = &data[ind];
			}
			return res;
		}
		/**
		* adds an element to the end.
		*/
		void push_back(const T &value) 
		{
			if (currentLength == maxSize)
			{
				doubleSpace();
			}
			data[currentLength] = new T(value);
			currentLength++;
		}
		/**
		* remove the last element from the end.
		* throw container_is_empty if size() == 0
		*/
		void pop_back() 
		{
			if (currentLength == 0)
			{
				throw container_is_empty();
			}
			currentLength--;
			delete data[currentLength];
		}
	};
}

#endif