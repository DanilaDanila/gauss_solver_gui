#include "fract.hpp"

#include <numeric>
#include <cmath>
#include <ostream>

Fract::Fract() { p=0; q=1; }

Fract::Fract(long p, size_t q): p(p), q(q) {}

Fract::Fract(const long &l) { p=l; q=1; }

void Fract::reduce()
{
	if(p == 0)
	{
		q = 1;
		return;
	}
	size_t t = std::gcd((size_t)std::abs(p), q);
   
	short sign = (p>=0)?1:-1;
	p = sign*(std::abs(p)/t);
	q /= t;
}

Fract::~Fract() {}

const Fract &Fract::operator+=(Fract f)
{
	p *= f.q;
	f.p *= q;
	q *= f.q;

	p += f.p;

	reduce();

	return *this;
}

const Fract &Fract::operator-=(Fract f)
{
	p *= f.q;
	f.p *= q;
	q *= f.q;

	p -= f.p;

	reduce();

	return *this;
}

const Fract &Fract::operator*=(const Fract &f)
{
	p *= f.p;
	q *= f.q;

	reduce();

	return *this;
}

const Fract &Fract::operator/=(const Fract &f)
{
	Fract new_f(q, std::abs(f.p));
	new_f.p *= (f.p>0)?1:-1;

	*this *= new_f;

	reduce();

	return *this;
}

std::ostream &operator<<(std::ostream &out, const Fract &f)
{
	if(f.q == 0)
		out<<"NaN";
	else if(f.q == 1)
		out<<f.p;
	else
		out<<f.p<<"/"<<f.q;

	return out;
}

const Fract operator+(const Fract &f0, const Fract &f1)
{
	Fract result = f0;
	result += f1;
	return result;
}

const Fract operator-(const Fract &f0, const Fract &f1)
{
	Fract result = f0;
	result -= f1;
	return result;
}

const Fract operator*(const Fract &f0, const Fract &f1)
{
	Fract result = f0;
	result *= f1;
	return result;
}

const Fract operator/(const Fract &f0, const Fract &f1)
{
	Fract result = f0;
	result /= f1;
	return result;
}
