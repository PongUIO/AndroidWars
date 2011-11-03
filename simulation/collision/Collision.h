#ifndef SIM_COLLISION_H
#define SIM_COLLISION_H

#include <vector>
#include "../Vector.h"
#include "../Save.h"

namespace Sim {
	class Collision {
		public:
			struct Result {
				Result() : isCol(false)
					{}
				
				Vector n;    ///< The normal in which to escape collision
				double dist; ///< The distance along the normal to escape
				bool isCol;  ///< If true, collision occurred
				
				Vector getOrp()
				{ return n*dist; }
			};
			
			struct RayResult {
				Vector pos;  ///< Exact position of intersection
				double len;  ///< Length of the ray from start to hit
				bool isCol;  ///< True if the ray intersected
			};
			
			typedef std::vector<Vector> ColPoints;
			
			Collision( const ColPoints &pts );
			~Collision();
			
			Result check(const Vector &mypos, const Vector &myvel,
				const Vector &theirpos, const Collision *they) const;
			Result checkClose(const Vector &mypos, const Vector &myvel,
				const Vector &theirpos, const Collision *they, double dist) const;
			RayResult checkRay(const Vector &mypos,
				const Vector &rayStart, const Vector &rayEnd) const;
			
			Vector getBboxLow() const { return bbLow; }
			Vector getBboxHigh() const { return bbHigh; }
			
			void save(Save::BasePtr& fp) const;
			static ColPoints load(Save::BasePtr& fp);
			
		private:
			bool pointOnCol(const Vector &pt, size_t ignoreAx,
				const Vector &dir) const;
			
			void generateNormals();
			void generateBbox();
			void generateProjection();
			
			size_t mNpCount; ///< Number of points/normals
			
			Vector bbLow, bbHigh; ///< Collision bounding box
			
			ColPoints mPoints;
			ColPoints mNormals;
			ColPoints mProjection; ///< Optimized projection of self
			
			/// @name Helper functions
			//@{
				template<class Predicate>
				static Result checkPredicate(
					const Collision *src,
					const Vector &mypos, const Vector &myvel,
					const Vector &theirpos,
					const Collision *they,
					Predicate predicate);
				
				static void computeDistance(
					double &dist, int &dir,
					const double my_min, const double my_max,
					const double th_min, const double th_max);
				
				static Vector getNormal(
					const Collision *src, const Collision *they,
					const size_t ax);
				
				static void computeProjection(
					double &my_min, double &my_max,
					const Collision *src, const Vector &n,
					const double offset_dot);
				
				static void copyProjection(
					const Collision *src,
					double &my_min, double &my_max,
					const size_t index, const double offset_dot);
			//@}
	};
	
}

#endif
