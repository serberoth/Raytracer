#ifndef __QUATERNION_H_
#define __QUATERNION_H_

#pragma once
/*---------------------------------------------------------------------------*/

class Quaternion : public gc {
private:
	union {
		struct {
			double v[4];
		};
		struct {
			double x, y, z, w;
		};
	};

public:
	static const Quaternion ZERO;
	static const Quaternion IDENTITY;
	
	inline Quaternion () : x(0.0), y (0.0), z (0.0), w (0.0) { }
	inline Quaternion (const double _x, const double _y, const double _z, const double _w) : x (_x), y (_y), z (_z), w (_w) { }
	inline Quaternion (const Vector &v) : x (v[0]), y (v[1]), z (v[2]), w(0.0) { }
	inline Quaternion (const Vector &v, const double theta) {
		double halfTheta = 0.5 * theta;
		double sinTheta = sin (halfTheta);
		x = sinTheta * v[0];
		y = sinTheta * v[1];
		z = sinTheta * v[2];
		w = cos (halfTheta);
	}
	virtual ~Quaternion () { }
	
	inline double operator [] (size_t index) const {
		assert (index < 4);
		return *(v + index);
	}
	inline double &operator [] (size_t index) {
		assert (index < 4);
		return *(v + index);
	}

	inline Quaternion operator + () const {
		return *this;
	}
	inline Quaternion operator - () const {
		return Quaternion (-x, -y, -z, w);
	}

	inline friend Quaternion operator + (const Quaternion &a, const Quaternion &b) {
		return Quaternion (a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
	}
	
	inline friend Quaternion operator - (const Quaternion &a, const Quaternion &b) {
		return Quaternion (a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
	}
	
	inline friend Quaternion operator * (const Quaternion &a, const Quaternion &b) {
		double prod0 = (a.z - a.y) * (b.y - b.z);
		double prod1 = (a.w + a.x) * (b.w + b.x);
		double prod2 = (a.w - a.x) * (b.y + b.z);
		double prod3 = (a.y + a.z) * (b.w - b.x);
		double prod4 = (a.z - a.x) * (b.x - b.y);
		double prod5 = (a.z + a.x) * (b.x + b.y);
		double prod6 = (a.w + a.z) * (b.w - b.z);
		double prod7 = (a.w - a.z) * (b.w + b.z);
		double prod8 = prod5 + prod6 + prod7;
		double prod9 = 0.5 * (prod4 + prod8);

		return Quaternion (prod1 + prod9 - prod8, prod2 + prod9 - prod7, prod3 + prod9 - prod6, prod0 + prod9 - prod5);
	}
	
	inline friend Quaternion operator / (const Quaternion &a, const Quaternion &b) {
		return a * b.Inverse ();
	}
	
	inline friend Vector operator * (const Quaternion &q, const Vector &v) {
		Vector qv (q.x, q.y, q.z);
		Vector uv = qv.Cross (v);
		Vector uuv = qv.Cross (uv);
		uv *= (2.0 * q.w);
		uuv *= 2.0;
		return v + uv + uuv;
	}

	inline friend Quaternion operator * (const Quaternion &q, const double s) {
		return Quaternion (q.x * s, q.y * s, q.z * s, q.w * s);
	}
	inline friend Quaternion operator * (const double s, const Quaternion &q) {
		return Quaternion (s * q.x, s * q.y, s * q.z, s * q.w);
	}
	
	inline friend Quaternion operator / (const Quaternion &q, const double s) {
		assert (!(s >= -std::numeric_limits<double>::epsilon () && s <= std::numeric_limits<double>::epsilon ()));
		double inverse = 1.0 / s;
		return Quaternion (q.x * inverse, q.y * inverse, q.z * inverse, q.w * inverse);
	}
	
	inline Quaternion Conjugate () const {
		return Quaternion (-v[0], -v[1], -v[2], v[3]);
	}

	inline Quaternion Inverse () const {
		double magSqr = 1.0 / (x * x + y * y + z * z + w * w);
		return Quaternion (-x * magSqr, -y * magSqr, -z * magSqr, w * magSqr);
	}

	inline double Dot (const Quaternion &q) const {
		return w * q.w + x * q.x + y * q.y + z * q.z;
	}

	inline double Magnitude () const {
		return sqrt (x * x + y * y + z * z + w * w);
	}

	inline Quaternion Normal () const {
		double inv_mag = 1.0 / sqrt (x * x + y * y + z * z + w * w);
		return Quaternion (x * inv_mag, y * inv_mag, z * inv_mag, w * inv_mag);
	}

	inline Vector ToVectorAngle (double &theta) const {
		theta = acos (w);
		double invSinTheta = 1.0 / sin (theta);
		theta *= 2.0;
		return Vector (x * invSinTheta, y * invSinTheta, z * invSinTheta);
	}
	
	inline Matrix ToRotationMatrix () const {
		double tx = 2.0 * x;
		double ty = 2.0 * y;
		double tz = 2.0 * w;
		double twx = tx * w;
		double twy = ty * w;
		double twz = tz * w;
		double txx = tx * x;
		double txy = ty * x;
		double txz = tz * z;
		double tyy = ty * y;
		double tyz = tz * y;
		double tzz = tz * z;
		
		return Matrix (1.0 - (tyy + tzz), txy - twz, txz + twy, 0.0,
			txy + twz, 1.0 - (txx + tzz), tyz - twx, 0.0,
			txz - twy, tyz + twx, 1.0 - (txx + tyy), 0.0,
			0.0, 0.0, 0.0, 1.0);
	}

	inline static Quaternion FromEulerAngles (const double thetaZ, const double thetaY, const double thetaX) {
		double halfCosZ = 0.5 * cos (thetaZ);
		double halfCosY = 0.5 * cos (thetaY);
		double halfCosX = 0.5 * cos (thetaX);

		double halfSinZ = 0.5 * cos (thetaZ);
		double halfSinY = 0.5 * cos (thetaY);
		double halfSinX = 0.5 * cos (thetaX);

		return Quaternion (halfCosZ * halfCosY * halfSinX - halfSinZ * halfSinY * halfCosX,
			halfCosZ * halfSinY * halfCosX + halfSinZ * halfCosY * halfSinX,
			halfSinZ * halfCosY * halfCosX - halfCosZ * halfSinY * halfSinX,
			halfCosZ * halfCosY * halfCosX + halfSinZ * halfSinY * halfSinX);
	}

	inline static Quaternion TripleProduct (Quaternion a, Quaternion b, Quaternion c) {
		return a * b * c;
	}
	
	inline static Matrix Transform (const Vector &position, const Vector &scale, const Quaternion &orientation) {
		Matrix m = orientation.ToRotationMatrix ();
		Matrix s = Matrix::Scale (scale);
		Matrix r = m * s;
		r.SetTranslation (position);
		r.m[3][0] = r.m[3][1] = r.m[3][2] = 0.0, r.m[3][3] = 1.0;
		return r;
	}
	inline static Matrix InverseTransform (const Vector &position, const Vector &scale, const Quaternion &orientation) {
		Vector p = -position;
		Vector inverse_s = 1.0 / scale;
		Quaternion rotation = orientation.Inverse ();
		p *= inverse_s;
		p = rotation * p;
		
		Matrix m = rotation.ToRotationMatrix ();
		Matrix s = Matrix::Scale (scale);
		Matrix r = s * m;
		r.SetTranslation (position);
		r.m[3][0] = r.m[3][1] = r.m[3][2] = 0.0, r.m[3][3] = 1.0;
		return r;
	}
	
}; /* class Quaternion */

/*---------------------------------------------------------------------------*/
#endif /* __QUATERNION_H_ */
