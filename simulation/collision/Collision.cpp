#include <algorithm>
#include <limits>

#include "Collision.h"

namespace Sim {
	Collision::Collision(const Collision::ColPoints& pts)
	{
		mNpCount = pts.size();
		mPoints = ColPoints(pts);
		
		if(mNpCount == 0)
			return;
		
		generateBbox();
		
		// Normals and projection only exist for non-point collision
		if(mNpCount >= 2) {
			mNormals.resize(mNpCount);
			mProjection.resize(mNpCount);
			
			generateNormals();
			generateProjection();
		}
	}
	
	Collision::~Collision()
	{}
	
	void Collision::generateBbox()
	{
		bbLow = bbHigh = mPoints[0];
		
		for(size_t i=1; i<mNpCount; i++) {
			Vector p = mPoints[i];
			
			if(p.x < bbLow.x)       bbLow.x = p.x;
			else if(p.x > bbHigh.x) bbHigh.x = p.x;
			
			if(p.y < bbLow.y)       bbLow.y = p.y;
			else if(p.y > bbHigh.y) bbHigh.y = p.y;
		}
	}
	
	void Collision::generateNormals()
	{
		for(size_t i=0; i<mNpCount-1; i++) {
			mNormals[i] = mPoints[i+1]-mPoints[i];
		}
		mNormals[mNpCount-1] = mPoints[0]-mPoints[mNpCount-1];
		
		for(size_t i=0; i<mNpCount; i++) {
			mNormals[i] = mNormals[i].rightNormal().normalize();
		}
	}
	
	void Collision::generateProjection()
	{
		double minv, maxv;
		for(size_t ni=0; ni<mNpCount; ni++) {
			minv = maxv = mPoints[0].dot(mNormals[ni]);
			
			for(size_t pi=0; pi<mNpCount; pi++) {
				double dot = mPoints[pi].dot(mNormals[ni]);
				
				if(dot < minv) minv = dot;
				else if(dot > maxv) maxv = dot;
			}
			
			mProjection[ni].x = minv;
			mProjection[ni].y = maxv;
		}
	}

	// Helper functions
	//
	//
	inline void Collision::computeDistance(
		double &dist, int &dir,
		const double my_min, const double my_max,
		const double th_min, const double th_max) {
		if(th_max < my_min) {
			dist = my_min - th_max;
			dir = 1;
		} else {
			dist = th_min - my_max;
			dir = -1;
		}
	}
	
	inline Vector Collision::getNormal(
		const Collision *src, const Collision *they,
		const size_t ax)
	{
		if(ax < src->mNpCount)
			return src->mNormals[ax];
		else
			return src->mNormals[ax-src->mNpCount];
	}
	
	inline void Collision::computeProjection(
		double &my_min, double &my_max,
		const Collision *src,
		const Vector &n,
		const double offset_dot = 0.0) {
		my_min = my_max = src->mPoints[0].dot(n);
		
		for(size_t i=1; i<src->mNpCount; i++) {
			double dot = src->mPoints[i].dot(n);
			
			if(dot < my_min) my_min = dot;
			else if(dot > my_max) my_max = dot;
		}
		
		my_min += offset_dot;
		my_max += offset_dot;
	}
	
	inline void Collision::copyProjection(
		const Collision *src,
		double &my_min, double &my_max,
		const size_t index, const double offset_dot = 0.0) {
		my_min = src->mProjection[index].x + offset_dot;
		my_max = src->mProjection[index].y + offset_dot;
	}
	
	template<class Predicate>
	Collision::Result Collision::checkPredicate(
		const Collision *src,
		const Vector &mypos, const Vector &myvel,
		const Vector &theirpos,
		const Collision *they,
		Predicate predicate
	) {
		Collision::Result ret;
		ret.isCol = true;
		
		// Relative distance between the two collision objects
		Vector offset = mypos-theirpos;
		
		// Projection variables
		double my_min, my_max;
		double th_min, th_max;
		double dist;
		int direction=1;
		Vector n;
		
		// Minimums
		double mindist = -std::numeric_limits<double>::infinity();
		Vector minn;
		int mindir=-1;
		
		if(src->mNpCount == 1) {
			// Special case for point-polygon testing
			
			if(they->mNpCount == 1)
				ret.isCol = false;
			
			Vector p = src->mPoints[0] + offset;
			
			for(size_t ax=0; ax<they->mNpCount; ax++) {
				n = they->mNormals[ax];
				
				// Project self (src) onto normal
				double me = p.dot(n);
				
				// Use projection buffer for them
				th_min = they->mProjection[ax].x;
				th_max = they->mProjection[ax].y;
				
				// Calculate intersection
				dist = me;
				if(th_max < me) dist = me - th_max;
				else dist = th_min - me;
				
				if( predicate(dist) ) {
					ret.isCol = false;
					break;
				} else if(dist > mindist) {
					mindist = dist;
					minn = n;
				}
			}
		} else {
			// Normal polygon-polygon testing
			for(size_t ax=0; ax < src->mNpCount+they->mNpCount; ax++) {
				// Select normal
				n = Collision::getNormal(src, they, ax);
				
				double offset_dot = offset.dot(n);
				if( ax < src->mNpCount )
					Collision::copyProjection(src, my_min,my_max, ax, offset_dot);
				else
					Collision::computeProjection(my_min,my_max, src, n, offset_dot);
				
				// Project them onto the normal
				if( ax < src->mNpCount )
					Collision::computeProjection(th_min,th_max, they, n);
				else
					Collision::copyProjection(they, th_min,th_max, ax-src->mNpCount);
				
				Collision::computeDistance(dist, direction, my_min,my_max,th_min,th_max);
				if( predicate(dist) ) {
					ret.isCol = false;
					break;
				} else if(dist > mindist) {
					mindist = dist;
					minn = n;
					mindir = direction;
				}
			}
		}
		
		if(ret.isCol) {
			ret.n = minn*mindir;
			ret.dist = -mindist;
		}
		
		return ret;
	}
	
	struct GreaterEqPred {
		bool operator()(const double &dist)
		{ return dist >= 0.0; }
	};
	
	struct DistancePred {
		DistancePred(double size) : size(size) {}
		
		bool operator()(const double &dist)
		{ return dist > size; }
		
		double size;
	};
	
	
	Collision::Result Collision::check(
		const Vector& mypos, const Vector& myvel,
		const Vector& theirpos, const Collision* they) const
	{
		static GreaterEqPred pred;
		return checkPredicate<>(this, mypos, myvel, theirpos, they, pred);
	}
	
	/**
	 * Checks if a collision object is within a certain distance
	 * of a collision object.
	 */
	Collision::Result Collision::checkClose(
		const Vector& mypos, const Vector& myvel,
		const Vector& theirpos, const Collision* they,
		double dist) const
	{
		static DistancePred dpred(dist);
		
		return checkPredicate<>(this, mypos, myvel, theirpos, they, dpred);
	}
	
	/**
	 * Checks if a point lies within a collision object, for
	 * use in determining if a ray intersection hits.
	 */
	bool Collision::pointOnCol(
		const Vector& pt, size_t ignoreAx, const Vector& dir) const
	{
		for(size_t ax=0; ax<mNpCount; ax++) {
			if(ax == ignoreAx)
				continue;
			
			const Vector &n = mNormals[ax];
			
			double dotpt = pt.dot(n);
			
			double th_min = mProjection[ax].x;
			double th_max = mProjection[ax].y;
			
			double dist;
			if(th_max < dotpt)  dist = dotpt - th_max;
			else dist = th_min - dotpt;
			
			if(dist > 0.0)
				return false;
		}
		
		return true;
	}
	
	/**
	 * Checks for ray collision.
	 */
	Collision::RayResult Collision::checkRay(
		const Vector& mypos,
		const Vector& rayStart, const Vector& rayEnd) const
	{
		RayResult ret;
		ret.isCol = false;
		
		if(mNpCount <= 1)
			return ret;
		
		double rlen = (rayEnd-rayStart).len();
		Vector rdir = (rayEnd-rayStart)/rlen;
		Vector rstart = rayStart-mypos;
		
		double mindist = rlen + 1.0;
		double dist = mindist;
		for(size_t ax=0; ax<mNpCount; ax++) {
			const Vector &n = mNormals[ax];
			
			// Ignore normals facing away from the ray
			double dotv = rdir.dot(n);
			if(dotv <= 0.0)
				continue;
			
			// Project ray onto normal
			dist = (mProjection[ax].x - rstart.dot(n)) / dotv;
			
			// Ignore anything before the ray start
			if(dist < 0.0)
				continue;
			
			// Determine if the point intersects
			if( dist<mindist && pointOnCol(rstart+rdir*dist, ax, rdir) ) {
				mindist = dist;
			}
		}
		
		if(mindist <= rlen) {
			ret.isCol = true;
			ret.pos = rayStart+rdir*mindist;
			ret.len = mindist;
		}
		
		return ret;
	}
	
	void Collision::save(Save::BasePtr& fp) const
	{
		fp.writeInt(mNpCount);
		for(size_t i=0; i<mNpCount; i++) {
			fp.writeVec(mPoints[i]);
		}
	}
	
	Collision::ColPoints Collision::load(Save::BasePtr& fp)
	{
		ColPoints pts;
		size_t ptCount = fp.readInt<size_t>();
		
		pts.reserve(ptCount);
		for(size_t i=0; i<ptCount; i++)
			pts.push_back( fp.readVec() );
		
		return pts;
	}
	
}
