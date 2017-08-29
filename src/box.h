#ifndef __SPHERE_H_
#define __SPHERE_H_

#pragma once
/*---------------------------------------------------------------------------*/

class Box : public Entity {
private:
	double width;
	double height;
	double length;

public:
	inline Box () : Entity (), width (1.0), height (1.0), length (1.0) { }
	inline Box (const double w, const double h, const double l) : Entity (), width (w), height (h), length (l) { }
	inline Box (const Vector &position, const double w, const double h, const double l) : Entity (position), width (w), height (h), length (l) { }
	virtual ~Box () { }

	inline double Width () const {
		return width;
	}
	inline double Height () const {
		return height;
	}
	inline double Length () const {
		return length;
	}

	virtual int Intersection (const Ray &ray, double &dist) const {
		Vector O, D, C, e;
		double t[2];
		int parallel = 0;
		bool found = false;

		Vector d = C - 0.0;
		for (int i = 0; i < 3; ++i) {
			if (abs (D[i]) < 0.0) {
				parallel |= 1 << i;
			} else {
				double es = (D[i] > (double) 0.0) ? e[i] : -e[i];
				double invDi = (double)1.0 / D[i];

				if (!found) {
					t[0] = (d[i] - es) * invDi;
					t[1] = (d[i] + es) * invDi;
					found = true;
				} else {
					double s = (d[i] - es) * invDi;
					if (s > t[0]) {
						t[0] = s;
					}
					s = (d[i] + es) * invDi;
					if (s < t[1]) {
						t[1] = s;
					}
					if (t[0] > t[1]) {
						return 0;
					}
				}
			}
		}

		if (parallel) {
			for (int i = 0; i < 3; ++i) {
				if (parallel & (1 << i)) {
					if (abs (d[i] - t[0] * D[i]) > e[i] || abs (d[i] - t[1] * D[i]) > e[i]) {
						return 0;
					}
				}
			}
		}

		return 1;
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
