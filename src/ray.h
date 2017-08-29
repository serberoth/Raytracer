#ifndef __RAY_H_
#define __RAY_H_

#pragma once
/*---------------------------------------------------------------------------*/

class Ray : public gc_cleanup {
private:
	const Vector origin;
	const Vector direction;

public:
	inline Ray (const Vector &_origin, const Vector &_direction) : origin (_origin), direction (_direction) { }
	virtual ~Ray () { }

	inline Vector Origin () const {
		return origin;
	}
	inline Vector Direction () const {
		return direction;
	}

	inline Vector PointAt (double t) const {
		return direction * t + origin;
	}

}; /* class Ray */

/*---------------------------------------------------------------------------*/
#endif /* __RAY_H_ */
