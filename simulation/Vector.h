#ifndef SIM_VECTOR_H
#define SIM_VECTOR_H

#include <math.h>

namespace Sim {
	class Vector {
		public:
			double x,y;
			
			Vector(double x, double y) : x(x), y(y)
				{}
			Vector(double v) : x(v), y(v)
				{}
			Vector() : x(0), y(0)
				{}
			
			Vector &operator =(const Vector &b)
			{ x=b.x; y=b.y; return *this; }
			
			Vector &operator =(const double &d)
			{ x=d; y=d; return *this; }
			
			// Vector operations
			#define VECV_OPER(op) \
				Vector operator op(const Vector &b) const \
				{ return Vector(x op b.x, y op b.y); } \
				void operator op##=(const Vector &b) \
				{ x op##=b.x; y op##= b.y; }
			VECV_OPER(+)
			VECV_OPER(-)
			VECV_OPER(*)
			VECV_OPER(/)
			#undef VECV_OPER
			
			#define VECD_OPER(op) \
				Vector operator op(const double &d) const \
				{ return Vector(x op d, y op d); } \
				void operator op##=(const double &d) \
				{ x op##=d; y op##= d; }
			VECD_OPER(*)
			VECD_OPER(/)
			
			double len() const { return sqrt(dot()); }
			double dot() const { return x*x+y*y; }
			double dot(const Vector &b) const
			{ return x*b.x+y*b.y; }
			
			Vector normalize()
				{ double L = len(); return Vector(x/L, y/L); }
			
			Vector rightNormal()
				{ return Vector(-y, x); }
	};
}

#endif
