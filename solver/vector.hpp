#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <initializer_list>
#include <cstddef>
#include <ostream>

#include "fract.hpp"

template<class T>
class Vector
{
private:
	size_t size_ = 0;
	size_t capacity_ = 0;
	T *data = nullptr;

public:
	Vector();

	explicit Vector(const size_t &capacity_);

	Vector(const Vector&);

	Vector(const std::initializer_list<T>&);

	~Vector();

    void clear() { size_ = 0; }

	T* begin() noexcept;
	const T* cbegin() const noexcept;

	T* end() noexcept;
	const T* cend() const noexcept;

	const Vector<T> &operator=(const Vector<T>&);

	void realloc(const size_t &new_capacity_);

    const size_t size();
	const size_t size() const;

	T &operator[](const size_t&);
	const T operator[](const size_t&) const;

	// NOT TRUE at() !!!
	const T _at(const size_t &);
	const T _at(const size_t &) const;

	void append(const T&);

	// TODO: reshape
	// now it is NOT CORRECT +=
	// it can be much more faster
	const Vector<T> &operator+=(const Vector<T>&);

	// TODO: reshape
	// now it is NOT CORRECT -=
	// it can be much more faster
	const Vector<T> &operator-=(const Vector<T>&);

	// DO NOT TOUCH *= and /= !!!
	template<class Num>
	const Vector<T> &operator*=(const Num &n) 
	{
		for(size_t i=0; i<size_; ++i)
			data[i] *= n;
		return *this;
	}

	template<class Num>
	const Vector<T> &operator/=(const Num &n) 
	{
		return *this *= 1./n;
	}

	template<class U>
	friend std::ostream &operator<<(std::ostream&, const Vector<U>&);

	template<class U>
	friend const Vector<U> operator+(const Vector<U>&, const Vector<U>&);

	template<class U>
	friend const Vector<U> operator-(const Vector<U>&, const Vector<U>&);

	template<class U, class Num>
	friend const Vector<U> operator*(const Vector<U>&, const Num&);
	template<class U, class Num>
	friend const Vector<U> operator*(const Num&, const Vector<U>&);

	template<class U, class Num>
	friend const Vector<U> operator/(const Vector<U>&, const Num&);
};

#include "vector.cpp"

#endif
