#ifndef SLAU_HPP
#define SLAU_HPP

#include "vector.hpp"
#include "fract.hpp"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <algorithm>

#include <iostream>

template<class T>
std::string as_slau(const Vector<Vector<T>>&);

template<class T>
std::string as_matrix(const Vector<Vector<T>>&);

template<class T>
std::string as_extended_matrix(const Vector<Vector<T>>&);

template<class T>
void slau_next_step(Vector<Vector<T>>&, const size_t &step);

template<class T>
size_t triangle_matrix_rank(const Vector<Vector<T>>&);

template<class T>
const Vector<Vector<T>> transpose(const Vector<Vector<T>>&);

template<class T>
const Vector<Vector<T>> triangulate(const Vector<Vector<T>>&);

const Vector<Vector<Fract>> get_solution(const Vector<Vector<int>>&);

#include "slau.cpp"

#endif
