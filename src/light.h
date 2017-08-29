#ifndef __LIGHT_H_
#define __LIGHT_H_

#pragma once
/*---------------------------------------------------------------------------*/

class Light : public Entity {
public:
	enum Type {
		POINT,
		DIRECTIONAL,
		SPOT,
	};

private:
	Type type;
	double radius;
	double radius_squared;
	double inverse_radius;
	double inner_cone_angle;
	double outer_cone_angle;
	double falloff;
	double cutoff;

public:
	Light () : Entity (), type (POINT), radius (0.1), radius_squared (radius * radius), inverse_radius (1.0 / radius), inner_cone_angle (), outer_cone_angle (), falloff (), cutoff () { }
	Light (const Type t) : Entity (), type (t), radius (0.1), radius_squared (radius * radius), inverse_radius (1.0 / radius), inner_cone_angle (), outer_cone_angle (), falloff (), cutoff () { }
	Light (const Vector &position, const Type t) : Entity (position), type (t), radius (0.1), radius_squared (radius * radius), inverse_radius (1.0 / radius), inner_cone_angle (), outer_cone_angle (), falloff (), cutoff () { }
	virtual ~Light () { }

	inline Type GetType () const {
		return type;
	}
	
	inline double InnerConeAngle () const {
		return inner_cone_angle;
	}
	inline double &InnerConeAngle () {
		return inner_cone_angle;
	}

	inline double OuterConeAngle () const {
		return outer_cone_angle;
	}
	inline double &OuterConeAngle () {
		return outer_cone_angle;
	}

	inline double Falloff () const {
		return falloff;
	}
	inline double &Falloff () {
		return falloff;
	}

	inline double Cutoff () const {
		return cutoff;
	}
	inline double &Cutoff () {
		return cutoff;
	}

	inline double Radius () const {
		return radius;
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
		return (p - position) * inverse_radius;
	}
	
	virtual Color TexelAt (const Vector &p) const {
		return material->Emission ();
	}

}; /* class Light */

/*---------------------------------------------------------------------------*/
#endif /* __LIGHT_H_ */
