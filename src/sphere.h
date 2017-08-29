#ifndef __SPHERE_H_
#define __SPHERE_H_

#pragma once
/*---------------------------------------------------------------------------*/

class Sphere : public Entity {
private:
	double radius;
	double radius_squared;
	double inverse_radius;

public:
	inline Sphere () : Entity (), radius (1.0), radius_squared (radius * radius), inverse_radius (1.0 / radius) { }
	inline Sphere (const double _radius) : Entity (), radius (_radius), radius_squared (radius * radius), inverse_radius (1.0 / radius) { }
	inline Sphere (const Vector &position, const double _radius) : Entity (position), radius (_radius), radius_squared (radius * radius), inverse_radius (1.0 / radius) { }
	virtual ~Sphere () { }

	inline double Radius () const {
		return radius;
	}

	inline double RadiusSquared () const {
		return radius_squared;
	}

	inline double InverseRadius () const {
		return inverse_radius;
	}

	virtual int Intersection (const Ray &ray, double &dist) const {
		Vector v = ray.Origin () - position;
		double b = -v.Dot (ray.Direction ());
		double det = (b * b) - v.Dot (v) + radius_squared;
		if (det > 0) {
			det = sqrt (det);
			double root1 = b - det;
			double root2 = b + det;
			if (root2 > 0) {
				if (root1 < 0) {
					if (root2 < dist) {
						dist= root2;
						return -1;
					}
				} else {
					if (root1 < dist) {
						dist = root1;
						return 1;
					}
				}
			}
		}
		return 0;
	}

	virtual Vector NormalAt (const Vector &p) const {
		return (p - position).Normal (); // * inverse_radius;
	}
	
	virtual Color TexelAt (const Vector &p) const {
		if (material->GetTexture ()  != NULL) {
			Vector sn = NormalAt (p);
			// Y-axis used as the pole vector
			double phi = acos (-sn.Dot (Vector::Y_AXIS));
			double v = phi * INV_PI;
			// X-axis used as equatorial point
			double theta = acos (Vector::X_AXIS.Dot (sn) / sin (phi)) * INV_TWO_PI;
			double u;
			// Z-axis used as pole.Dot(equator)
			if (Vector::Z_AXIS.Dot (sn) > 0) {
				u = theta;
			} else {
				u = 1.0 - theta;
			}
			return material->Ambient () * material->GetTexture ()->Filter (p, u, v);
		}
		return material->Ambient ();
	}

}; /* class Sphere */

/*---------------------------------------------------------------------------*/
#endif /* __SPHERE_H_ */
