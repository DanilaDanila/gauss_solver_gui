//#include "vector.hpp"
#include "fract.hpp"

template<class T>
Vector<T>::Vector() {}

template<class T>
Vector<T>::Vector(const size_t &capacity_): capacity_(capacity_)
{
	data = new T[capacity_];
	size_ = capacity_;
}

template<class T>
Vector<T>::Vector(const Vector &v): capacity_(v.capacity_), size_(v.size_)
{
	data = new T[capacity_];

	for(size_t i=0; i<size_; ++i)
		data[i] = v.data[i];
}

template<class T>
Vector<T>::Vector(const std::initializer_list<T> &list)
{
	for(const T &elem : list)
		append(elem);
}

template<class T>
Vector<T>::~Vector()
{
	if(data != nullptr)
		delete [] data;
}

template<class T>
T* Vector<T>::begin() noexcept { return data; }
template<class T>
const T* Vector<T>::cbegin() const noexcept { return data; }

template<class T>
T* Vector<T>::end() noexcept { return data + size_; }
template<class T>
const T* Vector<T>::cend() const noexcept { return data + size_; }
 
template<class T>
const Vector<T> &Vector<T>::operator=(const Vector<T> &v)
{
	if(data != nullptr)
		delete [] data;

	capacity_ = size_ = v.size_;
	data = new T[size_];
	for(size_t i=0; i<size_; ++i)
		data[i] = v.data[i];

	return *this;
}

template<class T>
void Vector<T>::realloc(const size_t &new_capacity_)
{
	if(capacity_ == new_capacity_)
		return;

	T *new_data = new T[new_capacity_];

	size_t new_size_ = std::min(new_capacity_, size_);
	for(size_t i=0; i<new_size_; ++i)
	new_data[i] = data[i];

	delete [] data;
	size_ = new_size_;
	capacity_ = new_capacity_;
	data = new_data;
}

template<class T>
const size_t Vector<T>::size() { return size_; }
template<class T>
const size_t Vector<T>::size() const { return size_; }

template<class T>
T &Vector<T>::operator[](const size_t &i) { return data[i]; }
template<class T>
const T Vector<T>::operator[](const size_t &i) const { return data[i]; }

// NOT TRUE at() !!!
template<class T>
const T Vector<T>::_at(const size_t &i)
{
	if (i >= size_)
		return T(0);
	return data[i];
}

template<class T>
const T Vector<T>::_at(const size_t &i) const
{
	if (i >= size_)
		return T(0);
	return data[i];
}

template<class T>
void Vector<T>::append(const T &elem)
{
	if(size_ < capacity_)
		data[size_++] = elem;
	else
	{
		realloc(unsigned(1.5*capacity_+1));
		data[size_++] = elem;
	}
}

// TODO: reshape
// now it is NOT CORRECT +=
// it can be much more faster
template<class T>
const Vector<T> &Vector<T>::operator+=(const Vector<T> &v)
{
	realloc(std::max(size_, v.size_));

	for(size_t i=0; i<capacity_; ++i)
		data[i] = _at(i) + v._at(i);

	return *this;
}

// TODO: reshape
// now it is NOT CORRECT -=
// it can be much more faster
template<class T>
const Vector<T> &Vector<T>::operator-=(const Vector<T> &v)
{
	realloc(std::max(size_, v.size_));

	for(size_t i=0; i<capacity_; ++i)
		data[i] = _at(i) - v._at(i);

	return *this;
}

template<class T>
std::ostream &operator<<(std::ostream &os, const Vector<T> &v)
{
	os<<"vector<"<<typeid(T).name()<<">["<<v.size_<<"]{";

	if(v.size_ > 0)
		os<<v.data[0];

	for(size_t i=1; i<v.size_; ++i)
		os<<", "<<v.data[i];
	os<<"}";

	return os;
}

template<class T>
const Vector<T> operator+(const Vector<T> &v0, const Vector<T> &v1)
{
	Vector<T> result(v0);
	result += v1;

	return result;
}

template<class T>
const Vector<T> operator-(const Vector<T> &v0, const Vector<T> &v1)
{
	Vector<T> result(v0);
	result -= v1;

	return result;
}

template<class T, class Num>
const Vector<T> operator*(const Vector<T> &v, const Num &n)
{
	Vector<T> result(v);
	result *= n;

	return result;
}

template<class T, class Num>
const Vector<T> operator*(const Num &n, const Vector<T> &v)
{ return v*n; }

template<class T, class Num>
const Vector<T> operator/(const Vector<T> &v, const Num &n)
{
	Vector<T> result(v);
	result /= n;

	return result;
}
