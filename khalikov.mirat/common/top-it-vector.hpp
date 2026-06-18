#ifndef TOP_IT_VECTOR_HPP
#define TOP_IT_VECTOR_HPP
#include <cstddef>
#include <utility>
#include <cassert>
#include <stdexcept>
#include <initializer_list>
#include <new>
#include "iter.hpp"
#include "citer.hpp"

namespace khalikov
{
  template< class T >
  struct Vector
  {
    Vector();
    explicit Vector< T >(std::initializer_list< T >) noexcept;
    ~Vector();
    Vector(const Vector< T > &);
    Vector< T > & operator=(const Vector< T > & rhs);
    T & operator[](size_t id) noexcept;
    const T & operator[](size_t id) const noexcept;
		Vector(Vector< T > &&);
		Vector< T > & operator=(Vector< T > &&) noexcept;

    bool isEmpty() const noexcept;
    void pushBack(const T &);
    void popBack();
    size_t getSize() const noexcept;
    size_t getCapacity() const noexcept;
    void resize(size_t newCapacity);
		void swap(Vector< T > & rhs) noexcept;
		void pushFront(const T &);
		T & at(size_t id);
		const T & at(size_t id) const;
		void reserve(size_t cap);
		void shrinkToFit();

		template< class IT >
		size_t pushBackRange(IT begin, size_t k);

		void erase(size_t i);
		void erase(size_t beg, size_t end);
		void erase(LIter< T > pos);
		void erase(LIter< T > first, LIter< T > last);
		void erase(LIter< T > pos, size_t k);
		void insert(size_t i, const T & val);
		void insert(size_t i, const Vector< T > & rhs, size_t beg, size_t end);
		void insert(LIter< T > pos, const T & val);
		void insert(LIter< T > pos, const Vector< T > & other);
		void insert(LIter< T > pos, size_t k, const T & val);

  private:
    T * data_;
    size_t cap_, size_;
    friend class LIter< T >;
    friend class LCIter< T >;
    //без проверки на капасити
		void pushBackImpl(const T &);
		//классная(::operator new и тд)
		void reserve(size_t pos, size_t count);
    explicit Vector(size_t k);
  };
}

template< class T >
void khalikov::Vector< T >::reserve(size_t cap)
{
	if (cap_ >= cap)
	{
		return;
	}
	size_t i = 0;
	T * d = static_cast< T * >(::operator new(sizeof(T) * cap));
	try
	{
		for (; i < getSize(); i++)
		{
			new (d + i) T(std::move(data_[i]));
		}
	}
	catch (...)
	{
		for (size_t j = 0; j < i; ++j)
		{
			(d + j)->~T();
		}
		::operator delete(d);
		throw;
	}
	::operator delete(data_);
	data_ = d;
	cap_ = cap;
}

template< class T >
template< class IT >
size_t khalikov::Vector< T >::pushBackRange(IT begin, size_t k)
{
	size_t oldSize = size_;
	size_t newSize = size_ + k;
	if (newSize > cap_)
	{
		size_t newCap = cap_ == 0 ? k : cap_;
		while (newCap < newSize)
		{
			newCap = newCap * 2;
		}
		reserve(newCap);
		for (size_t i = 0; i < k; ++i)
		{
			new (data_ + size_) T(*begin);
			++size_;
			++begin;
		}
	}
	return newSize;
}

template< class T >
void khalikov::Vector< T >::pushBackImpl(const T &val)
{
	new (data_ + size_) T(val);
	++size_;
}

template< class T >
void khalikov::Vector< T >::shrinkToFit()
{
	if (size_ == cap_)
	{
		return;
	}

	if (size_ == 0)
	{
		::operator delete(data_);
		data_ = nullptr;
		cap_ = 0;
		return;
	}

	T * d = static_cast< T * >(::operator new(sizeof(T) * size_));
	size_t i = 0;
	try
	{
		for (; i < size_; ++i)
		{
			new (d + i) T(std::move(data_[i]));
		}
	}
	catch (...)
	{
		for (size_t j = 0; j < i; j++)
		{
			(d + j)->~T();
		}
		::operator delete(d);
		throw;
	}
	for (size_t j = 0; j < size_; ++j)
	{
		data_[j].~T();
	}
	::operator delete(data_);
}

template< class T >
khalikov::Vector< T >::Vector(std::initializer_list< T > il) noexcept:
	Vector< T >(il.size())
{
	size_t i = 0;
	for (auto &&v: il)
	{
		data_[i++] = std::move(v);
	}
}

template< class T >
size_t khalikov::Vector< T >::getSize() const noexcept
{
	return size_;
}

template< class T >
void khalikov::Vector< T >::pushFront(const T & val)
{
	Vector< T > cpy(getSize() + 1);
	cpy.data_[0] = val;
	for (size_t i = 1; i <= size_; ++i)
	{
		cpy.data_[i] = data_[i-1];
	}
	cpy.size_ = size_ + 1;
	swap(cpy);
}

template< class T >
size_t khalikov::Vector< T >::getCapacity() const noexcept
{
	return cap_;
}

template< class T >
bool khalikov::Vector< T >::isEmpty() const noexcept
{
	return !size_;
}

template< class T >
void khalikov::Vector< T >::popBack()
{
	Vector< T > cpy(size_ - 1);
	for (size_t i = 0; i < size_ - 1; i++)
	{
		cpy.data_[i] = data_[i];
		cpy.size_++;
	}
	swap(cpy);
}

template< class T >
khalikov::Vector< T >::Vector():
	data_(nullptr),
	cap_(0),
	size_(0)
{}

template< class T >
khalikov::Vector< T >::Vector(size_t k):
	data_(static_cast< T * >(::operator new(sizeof(T) * k))),
	cap_(k),
	size_(0)
{}

template< class T >
khalikov::Vector< T >::~Vector()
{
	delete[] data_;
}

template< class T >
khalikov::Vector< T >::Vector(const Vector< T > & rhs):
	data_(nullptr),
	cap_(0),
	size_(0)
{
	Vector< T > temp(rhs.cap_);
	for (size_t i = 0; i < rhs.size_; i++)
	{
		temp.data_[i] = rhs.data_[i];
	}
	temp.size_ = rhs.size_;
	swap(temp);
}

template< class T >
khalikov::Vector< T >::Vector(Vector< T > && rhs):
	data_(rhs.data_),
	cap_(rhs.cap_),
	size_(rhs.size_)
{
	rhs.data_ = nullptr;
}

template< class T >
khalikov::Vector< T > & khalikov::Vector< T >::operator=(Vector< T > && rhs) noexcept
{
	if (this == std::addressof(rhs))
	{
		return *this;
	}
	Vector< T > cpy(std::move(rhs));
	swap(cpy);
	return *this;
}

template< class T >
void khalikov::Vector< T >::swap(Vector< T > & rhs) noexcept
{
	std::swap(rhs.data_, data_);
	std::swap(rhs.size_, size_);
	std::swap(rhs.cap_, cap_);
}

template< class T >
khalikov::Vector< T > & khalikov::Vector< T >::operator=(const Vector< T > & rhs)
{
	if (this == std::addressof(rhs))
	{
		return *this;
	}
	Vector< T > cpy(rhs);
	swap(rhs);
	return *this;
}

template< class T >
void khalikov::Vector< T >::resize(size_t newCapacity)
{
	Vector< T > res(newCapacity);
	size_t index = 0;
	for (size_t i = 0; i < size_; i++)
	{
		res.data_[index++] = data_[i];
	}
	res.size_ = index;
	swap(res);
}

template< class T >
T & khalikov::Vector< T >::operator[](size_t id) noexcept
{
	const Vector< T > * cthis = this;
	const T& ret = (*cthis)[id];
	return const_cast< T & >(ret);
}

template< class T >
const T & khalikov::Vector< T >::operator[](size_t id) const noexcept
{
	assert(id < getSize());
	return data_[id];
}

template< class T >
T& khalikov::Vector< T >::at(size_t id)
{
	const Vector< T > * cthis = this;
	const T& ret = cthis->at(id);
	return const_cast< T & >(ret);
}

template< class T >
const T& khalikov::Vector< T >::at(size_t id) const
{
	if (id < getSize())
	{
		return (*this)[id];
	}
	throw std::range_error("bad_id");
}


template< class T >
void khalikov::Vector< T >::pushBack(const T & val)
{
	if (cap_ > size_)
	{
		data_[size_] = val;
		size_++;
	}
	else
	{
		size_t newCapacity = cap_ + 5;
		resize(newCapacity);
		data_[size_] = val;
		size_++;
	}
}

#endif
