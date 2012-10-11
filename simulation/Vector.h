#ifndef SIM_VECTOR_H
#define SIM_VECTOR_H

#include <limits>
#include <ostream>

#include <math.h>
#include <boost/operators.hpp>

namespace Sim {
	class Vector :
		boost::equality_comparable<Vector>,
		boost::less_than_comparable<Vector>,
		boost::arithmetic<Vector>
	{
		public:
			double x,y;
			
			Vector(double x, double y) : x(x), y(y)
				{}
			Vector(double v) : x(v), y(v)
				{}
			Vector() : x(0), y(0)
				{}
			
			inline double operator[] (const unsigned int i) const
			{	return *(&x+i); }
			
			Vector &operator =(const Vector &b)
			{ x=b.x; y=b.y; return *this; }
			
			Vector &operator =(const double &d)
			{ x=d; y=d; return *this; }
			
			bool operator <(const Vector &b) const
			{ return (x<b.x) || (y<b.y); }
			// by less_than_comparable: > <= >=
			
			bool operator ==(const Vector &b) const
			{ return (x==b.x) && (y==b.y); }
			// by equality_comparable: !=
			
			Vector operator -(void) const { return Vector(-x,-y); }
			
			void operator +=(const Vector &b) { x+=b.x; y+=b.y; }
			void operator -=(const Vector &b) { x-=b.x; y-=b.y; }
			void operator *=(const Vector &b) { x*=b.x; y*=b.y; }
			void operator /=(const Vector &b) { x/=b.x; y/=b.y; }
			
			void operator +=(const double &b) { x+=b; y+=b; }
			void operator -=(const double &b) { x-=b; y-=b; }
			void operator *=(const double &b) { x*=b; y*=b; }
			void operator /=(const double &b) { x/=b; y/=b; }
			
			double len() const { return sqrt(dot()); }
			double dot() const { return x*x+y*y; }
			double dot(const Vector &b) const
			{ return x*b.x+y*b.y; }
			
			Vector normalize()
				{ double L = len(); return Vector(x/L, y/L); }
			
			Vector rightNormal()
				{ return Vector(-y, x); }
			
			static Vector infinity() {
				double infVal = std::numeric_limits<double>::infinity();
				return Vector(infVal,infVal);
			}
			
			friend std::ostream& operator<<(
			std::ostream &stream, const Vector &v) {
				return (stream << "(" << v.x << "," << v.y << ")");
			}
	};
}

#endif
