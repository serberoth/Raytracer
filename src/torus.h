#ifndef __TORUS_H_
#define __TORUS_H_

#pragma once
/*---------------------------------------------------------------------------*/

class Torus : public Entity {
private:
	double radius;
	double a;
	double b;
	double radius_squared;
	double a_squared;
	double b_squared;
	double inverse_b_squared;

	double p;
	double A0;
	double B0;

public:
	inline Torus (const double _radius, const double _a, const double _b) : Entity (), radius (_radius), a (_a), b (_b), radius_squared (radius * radius), a_squared (a * a), b_squared (b * b), inverse_b_squared (1.0 / b_squared), p (a_squared / b_squared), A0 (4 * radius_squared), B0 (radius_squared - a_squared) { }
	inline Torus (const Vector &position, const double _radius, const double _a, const double _b) : Entity (position), radius (_radius), a (_a), b (_b), radius_squared (radius * radius), a_squared (a * a), b_squared (b * b), inverse_b_squared (1.0 / b_squared), p (a_squared / b_squared), A0 (4 * radius_squared), B0 (radius_squared - a_squared) { }
	virtual ~Torus () { }

	virtual int Intersection (const Ray &ray, double &dist) const {
		Vector U (orientation[0], 0.0, 0.0);
		Vector N (0.0, orientation[1], 0.0);
		Matrix M = Matrix (U, N, U.Cross (N), -position);
		Vector pos = ray.Origin () * M;
		Vector dir = ray.Direction () * M;

		double f = 1.0 - dir[1] * dir[1];
		double g = f + p * dir[1] * dir[1];
		double l = 2.0 * (pos[0] * dir[0] + pos[2] * dir[2]);
		double t = pos[0] * pos[0] + pos[2] * pos[2];
		double q = A0 / (g * g);
		double m = (l + 2.0 *p * pos[1]) / g;
		double u = (t + p * pos[1] * pos[1] + B0) / g;

		double c4 = 1.0;
		double c3 = 2.0 * m;
		double c2 = m * m + 2 * u - q * f;
		double c1 = 2.0 * m * u;
		double c0 = u * u - q * t;

		// TODO: Complete this method and solve the quartic

		return 0;
	}

	virtual Vector NormalAt (const Vector &p) const {
		double d = sqrt (p[0] * p[0] + p[2] * p[2]);
		double f = (2.0 * (d - radius)) / (d * a_squared);
		return Vector (p[0] * f, 2.0 * p[1] * inverse_b_squared, p[2] * f);
	}

	virtual Color TexelAt (const Vector &p) const {
		return material->Ambient ();
	}

}; /* class Torus */

/*---------------------------------------------------------------------------*/
#endif /* __TORUS_H_ */
