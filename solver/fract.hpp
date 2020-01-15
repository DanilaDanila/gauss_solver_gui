#ifndef FRACT_HPP
#define FRACT_HPP

#include <ostream>

// Fractional number
class Fract
{
private:
	long   p;  
	size_t q;
public:
	Fract();

	Fract(long, size_t);

	Fract(const long &);

	Fract(const Fract &) = default;

	void reduce();

	~Fract();

	const Fract &operator+=(Fract);
	
	const Fract &operator-=(Fract);

	const Fract &operator*=(const Fract&);

	const Fract &operator/=(const Fract&);

	friend std::ostream &operator<<(std::ostream&, const Fract&);
	friend const Fract operator+(const Fract&, const Fract&);
	friend const Fract operator-(const Fract&, const Fract&);
	friend const Fract operator*(const Fract&, const Fract&);
	friend const Fract operator/(const Fract&, const Fract&);
};
#endif
