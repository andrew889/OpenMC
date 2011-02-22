#ifndef OPENMC_MATH_COMPILERMATH_HPP
#define OPENMC_MATH_COMPILERMATH_HPP

template <int Base, int Exp>
struct cpow
{
	static const int x =  Base * cpow<Base, Exp - 1>::x;
};

template <int Base>
struct cpow<Base, 1>
{
	static const int x = Base;
};

template <int Base>
struct cpow<Base, 0>
{
	static const int x = 1;
};


// Base**N + Base**(N-1) + Base**(N-2) + ... + Base**2 + Base**1 + Base**0
template <int Base, int N>
struct csum_powers
{
	static const int x = cpow<Base, N>::x + csum_powers<Base, N - 1>::x;
};

template <int Base>
struct csum_powers<Base, 0>
{
	static const int x = 1;
};

#endif // OPENMC_MATH_COMPILERMATH_HPP