#ifndef __BOUNDING_BOX_H_
#define __BOUNDING_BOX_H_

#pragma once
/*---------------------------------------------------------------------------*/

class BoundingBox : public gc_cleanup {
private:
	Vector max;
	Vector min;
	
public:
	BoundingBox (const Vector &_max, const Vector &_min) : max (_max), min (_min) { }
	virtual ~BoundingBox () { }
	
	int Intersection (const BoundingBox &b) const {
		return 0; 
	}
	
}; /* class BoundingBox */

/*---------------------------------------------------------------------------*/
#endif /* __BOUNDING_BOX_H_ */
