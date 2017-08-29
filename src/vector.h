#ifndef __VECTOR_H_
#define __VECTOR_H_

#pragma once
/*---------------------------------------------------------------------------*/

class Vector : public gc {
private:
	union {
		struct {
			double v[3];
		};
		struct {
			double x, y, z;
		};
	};

public:
	static const Vector ZERO;
	static const Vector X_AXIS;
	static const Vector Y_AXIS;
	static const Vector Z_AXIS;

	inline Vector () : x (0.0), y (0.0), z (0.0) { }
	inline Vector (double _x, double _y, double _z) : x (_x), y (_y), z (_z) { }
	inline explicit Vector (double s) : x (s), y (s), z (s) { }
	inline Vector (const Vector &v) : x (v.x), y (v.y), z (v.z) { }
	virtual ~Vector () { }

	inline double operator [] (size_t index) const {
		assert (index < 3);
		return *(&x + index);
	}
	inline double &operator [] (size_t index) {
		assert (index < 3);
		return *(&x + index);
	}

	inline Vector operator = (const Vector &v) {
		x = v.x, y = v.y, z = v.z;
		return *this;
	}

	inline Vector operator + () const {
		return *this;
	}
	inline Vector operator - () const {
		return Vector (-x, -y, -z);
	}

	inline friend Vector operator + (const Vector &a, const Vector &b) {
		return Vector (a.x + b.x, a.y + b.y, a.z + b.z);
	}
	inline friend Vector operator + (const Vector &v, const double s) {
		return Vector (v.x + s, v.y + s, v.z + s);
	}
	inline friend Vector operator + (const double s, const Vector &v) {
		return Vector (s + v.x, s + v.y, s + v.z);
	}

	inline friend Vector operator - (const Vector &a, const Vector &b) {
		return Vector (a.x - b.x, a.y - b.y, a.z - b.z);
	}
	inline friend Vector operator - (const Vector &v, const double s) {
		return Vector (v.x - s, v.y - s, v.z - s);
	}
	inline friend Vector operator - (const double s, const Vector &v) {
		return Vector (s - v.x, s - v.y, s - v.z);
	}

	inline friend Vector operator * (const Vector &a, const Vector &b) {
		return Vector (a.x * b.x, a.y * b.y, a.z * b.z);
	}
	inline friend Vector operator * (const Vector &a, const double s) {
		return Vector (a.x * s, a.y * s, a.z * s);
	}
	inline friend Vector operator * (const double s, const Vector &a) {
		return Vector (a.x * s, a.y * s, a.z * s);
	}

	inline friend Vector operator / (const Vector &a, const double s) {
		assert (s <= -std::numeric_limits<double>::epsilon () || s >= std::numeric_limits<double>::epsilon ());
		double inv = 1.0 / s;
		return Vector (a.x * inv, a.y * inv, a.z * inv);
	}
	inline friend Vector operator / (const double s, const Vector &a) {
		return Vector (s / a.x, s / a.y, a.z / s);
	}

	inline Vector operator += (const Vector &v) {
		x += v.x, y += v.y, z += v.z;
		return *this;
	}

	inline Vector operator -= (const Vector &v) {
		x -= v.x, y -= v.y, z -= v.z;
		return *this;
	}

	inline Vector operator *= (const Vector &v) {
		x *= v.x, y *= v.y, z *= v.z;
		return *this;
	}

	inline Vector operator *= (const double s) {
		x *= s, y *= s, z *= s;
		return *this;
	}
	inline Vector operator /= (const double s) {
		assert (s <= -std::numeric_limits<double>::epsilon () || s >= std::numeric_limits<double>::epsilon ());
		double inv = 1.0 / s;
		x *= inv, y *= inv, z *= inv;
		return *this;
	}

	inline double Magnitude () const {
		return sqrt (x * x + y * y + z * z);
	}
	inline double MagnitudeSquared () const {
		return x * x + y * y + z * z;
	}

	inline double Dot (const Vector &v) const {
		return x * v.x + y * v.y + z * v.z;
	}

	inline double SquareDistance (const Vector &v) const {
		return (x - v.x) * (x - v.x) + (y - v.y) * (y - v.y) + (z - v.z) * (z  - v.z);
	}

	inline Vector Cross (const Vector &v) const {
		return Vector (y * v.z + z * v.y, x * v.z + z * v.x, x * v.y + y * v.x);
	}

	inline Vector Normal () const {
		double m = sqrt (x * x + y * y + z * z);
		assert (m <= -std::numeric_limits<double>::epsilon () || m >= std::numeric_limits<double>::epsilon ());
		m = 1.0 / m;
		return Vector (x * m, y * m, z * m);
	}

	inline friend std::ostream &operator << (std::ostream &out, const Vector &v) {
		return out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
	}

}; /* class Vector */

/*---------------------------------------------------------------------------*/
#endif /* __VECTOR_H_ */
