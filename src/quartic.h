#ifndef __QUARTIC_H_
#define __QUARTIC_H_

#pragma once
/*---------------------------------------------------------------------------*/

class Quartic : public gc {
private:
	union {
		struct {
			double v[4];
		};
		struct {
			double a, b, c, d, e;
		};
	};
	
public:
	Quartic (double _a, double _b, double _c, double _d, double _e) : a (_a), b (_b), c (_c), d (_d), e (_e) { }
	virtual ~Quartic () { }
	
	inline double operator [] (size_t index) const {
		assert (index < 4);
		return *(&a + index);
	}
	inline double &operator [] (size_t index) {
		assert (index < 4);
		return *(&a + index);
	}
	inline double operator [] (double t) const {
		double result = e;
		result += d * t, t *= t;
		result += c * t, t *= t;
		result += b * t, t *= t;
		return result + a * t; 
	}
	
	inline Quartic operator = (const Quartic &eq) {
		a = eq.a, b = eq.b, c = eq.c, d = eq.d, e = eq.e;
		return *this;
	}
	
	inline Quartic operator + () const {
		return *this;
	}
	inline Quartic operator - () const {
		return Quartic (-a, -b, -c, -d, -e);
	}
	
	inline friend Quartic operator + (const Quartic &a, const Quartic &b) {
		return Quartic (a.a + b.a, a.b + b.b, a.c + b.c, a.d + b.d, a.e + b.e);
	}
	inline friend Quartic operator - (const Quartic &a, const Quartic &b) {
		return Quartic (a.a - b.a, a.b - b.b, a.c - b.c, a.d - b.d, a.e - b.e);
	}
	
	inline friend Quartic operator * (const Quartic &eq, const double v) {
		return Quartic (eq.a * v, eq.b * v, eq.c * v, eq.d * v, eq.e * v);
	}
	inline friend Quartic operator * (const double v, const Quartic &eq) {
		return Quartic (v * eq.a, v * eq.b, v * eq.c, v * eq.d, v * eq.e);
	}
	inline friend Quartic operator / (const Quartic &eq, const double v) {
		assert (v <= -std::numeric_limits<double>::epsilon () || v >= std::numeric_limits<double>::epsilon ());
		double inv = 1.0 / v;
		return Quartic (eq.a * inv, eq.b * inv, eq.c * inv, eq.d * inv, eq.e * inv);
	}
	inline friend Quartic operator / (const double v, const Quartic &eq) {
		Quartic q = eq;
		q.a = (q.a != 0.0) ? v / q.a : q.a;
		q.b = (q.b != 0.0) ? v / q.b : q.b;
		q.c = (q.c != 0.0) ? v / q.c : q.c;
		q.d = (q.d != 0.0) ? v / q.d : q.d;
		q.e = (q.e != 0.0) ? v / q.e : q.e;
		return q;
	}
	
	double[] Solve () const {
		double roots[4];
		
		double a_sqrd = a * a;
		double a_cubd = a_sqrd * a;
		double a_frth = a_cubd * a;
		double b_sqrd = b * b;
		double b_cubd = b_sqrd * b;
		double b_frth = b_cubd * b;
		double inv_a = 1.0 / a;
		
		double alpha = (-3 * b_sqrd) / (8 * a_sqrd) + c * inv_a;
		double beta = b_cubd / (8 * a_cubd) - (b * c)  / (2 * a_sqrd) + d * inv_a;
		double gamma = (-3 * b_frth) / (256 * a_frth) + (c * b_sqrd) / (16 * a_cubd) - (b * d) / (4 * a_sqrd) + e * inv_a;
		
		if (beta > -std::numeric_limits<double>::epsilon () || beta < std::numeric_limits<double>::epsilon ()) {
			// TODO: Solve the bi-quadratic here
		}
		if (gamma > -std::numeric_limits<double>::epsilon () || gamma < std::numeric_limits<double>::epsilon ()) {
		}
		
		// u^4 + alpha * u^2 + beta * u + gamma = 0
		
		Quartic eq = *this / this.a;
		return eq.v;
	}
	
};

/*---------------------------------------------------------------------------*/
#endif /* __QUARTIC_H_ */
