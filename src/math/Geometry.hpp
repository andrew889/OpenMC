#ifndef OPENMC_MATH_GEOMETRY
#define OPENMC_MATH_GEOMETRY

#include "Vector.hpp"

namespace math {

template <unsigned int N>
struct NOrthotope {
	mat<N, 1> origin;
	mat<N, 1> size;

	inline NOrthotope()
	{
	}

	inline NOrthotope(const mat<N, 1>& origin, const mat<N, 1>& size)
		: origin(origin), size(size)
	{
	}

	inline NOrthotope(const mat<N, 1>& a, const mat<N, 1>& b, void*)
		: origin(a), size(b)
	{
		size -= origin;
		normalize();
	}

	void normalize()
	{
		for (unsigned int i = 0; i < N; ++i) {
			if (size < 0.f) {
				size[i] = -size[i];
				origin[i] -= size[i];
			}
		}
	}

	bool contains(const mat<N, 1>& p) const
	{
		bool inside = true;
		for (unsigned int i = 0; i < N; ++i) {
			inside = inside && (p[i] >= origin[i] && p[i] <= origin[i] + size[i]);
		}
		return inside;
	}

	mat<N, 1> center() const
	{
		return origin + (size / 2.f);
	}
};

typedef NOrthotope<2> Rect;
typedef NOrthotope<3> Box;

} // namespace math

#endif // OPENMC_MATH_GEOMETRY
