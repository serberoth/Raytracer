#ifndef __PLANE_H_
#define __PLANE_H_

#pragma once
/*---------------------------------------------------------------------------*/

class Plane : public Entity {
private:
	Vector normal;
	double d;
	Vector u_axis;
	Vector v_axis;

public:
	inline Plane (const Vector &_normal, const double _d) : Entity (), normal (_normal), d (_d), u_axis (normal[1], normal[2], -normal[0]), v_axis  (u_axis.Cross (normal)) { }
	virtual ~Plane () { }

	inline Vector Normal () const {
		return normal;
	}

	inline double D () const {
		return d;
	}

	virtual int Intersection (const Ray &ray, double &dist) const {
		double dot = normal.Dot (ray.Direction ());
		if (dot != 0.0) { // >= std::numeric_limits<double>::epsilon ()) {
			double intersect = -(normal.Dot (ray.Origin ()) + d) / dot;
			if (intersect > 0.0) {
				if (intersect < dist) {
					dist = intersect;
					return 1;
				}
			}
		}
		return 0;
	}

	virtual Vector NormalAt (const Vector &p) const {
		return normal;
	}
	
	virtual Color TexelAt (const Vector &p) const {
		if (material->GetTexture () != NULL) {
			double u = p.Dot (u_axis) * 0.1;
			double v = p.Dot (v_axis) * 0.1;
			return material->GetTexture ()->Filter (p, u, v) * material->Ambient ();
		}
		return material->Ambient ();
	}

}; /* class Plane */

/*---------------------------------------------------------------------------*/
#endif /* __PLANE_H_ */
