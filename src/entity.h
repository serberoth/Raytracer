#ifndef __ENTITY_H_
#define __ENTITY_H_

#pragma once
/*---------------------------------------------------------------------------*/

class Entity : public virtual gc_cleanup {
protected:
	Vector position;
	Vector orientation;
	Vector velocity;
	Vector acceleration;
	Vector angular_velocity;
	Vector angular_acceleration;

	Material *material;

public:
	inline Entity () : position (), orientation (), velocity (), acceleration (), angular_velocity (), angular_acceleration (), material (new Material ()) { }
	inline Entity (const Vector &_position) : position (_position), orientation (), velocity (), acceleration (), angular_velocity (), angular_acceleration (), material (new Material ()) { }
	virtual ~Entity () { }

	inline Vector Position () const {
		return position;
	}
	inline Vector &Position () {
		return position;
	}

	inline Vector Orientation () const {
		return orientation;
	}
	inline Vector &Orientation () {
		return orientation;
	}

	inline Vector Velocity () const {
		return velocity;
	}
	inline Vector &Velocity () {
		return velocity;
	}

	inline Vector Acceleration () const {
		return acceleration;
	}
	inline Vector &Acceleration () {
		return acceleration;
	}

	inline Vector AngularVelocity () const {
		return angular_velocity;
	}
	inline Vector &AngularVelocity () {
		return angular_velocity;
	}

	inline Vector AngularAcceleration () const {
		return angular_acceleration;
	}
	inline Vector &AngularAcceleration () {
		return angular_acceleration;
	}

	inline Material *GetMaterial () const {
		return material;
	}

	virtual int Intersection (const Ray &ray, double &dist) const = 0;

	virtual Vector NormalAt (const Vector &p) const = 0;
	
	virtual Color TexelAt (const Vector &p) const = 0;

}; /* class Entity */

/*---------------------------------------------------------------------------*/
#endif /* __ENTITY_H_ */
