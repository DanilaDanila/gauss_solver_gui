#ifndef TOOLS_HPP
#define TOOLS_HPP

// greatest common divisor
// P.S. сами пишите своего евклида
// P.P.S. "х&@$ х&@$ и в продакшн!"
template<class T>
const T gcd(const T &t0, const T &t1)
{
    for(T i=t0; i>T(0); --i)
        if ((t0 % i == T(0)) && (t1 % i == T(0)))
            return i;
    return T(1);
}

// least common multiple
template<class T>
const T lcm(const T &t0, const T &t1)
{
    return (t0 * t1) / gcd(t0, t1);
}

#endif
