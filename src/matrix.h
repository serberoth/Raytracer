#ifndef __MATRIX_H_
#define __MATRIX_H_

#pragma once
/*---------------------------------------------------------------------------*/

class Matrix : public gc {
private:
	union {
		struct {
			double a[16];
		};
		struct {
			double m[4][4];
		};
		struct {
			double m00, m01, m02, m03;
			double m10, m11, m12, m13;
			double m20, m21, m22, m23;
			double m30, m31, m32, m33;
		};
	};
	
	inline double Minor (const size_t r0, const size_t r1, const size_t r2, const size_t c0, const size_t c1, const size_t c2) const {
		return m[r0][c0] * (m[r1][c1] * m[r2][c2] - m[r2][c1] * m[r1][c2]) -
			m[r0][c1] * (m[r1][c0] * m[r2][c2] - m[r2][c0] * m[r1][c2]) +
			m[r0][c2] * (m[r1][c0] * m[r2][c1] - m[r2][c0] * m[r1][c1]);
	}

public:
	static const Matrix ZERO;
	static const Matrix IDENTITY;
	
	inline Matrix () :
		m00 (1.0), m01 (0.0), m02 (0.0), m03 (0.0),
		m10 (0.0), m11 (1.0), m12 (0.0), m13 (0.0),
		m20 (0.0), m21 (0.0), m22 (1.0), m23 (0.0),
		m30 (0.0), m31 (0.0), m32 (0.0), m33 (1.0) { }
	inline Matrix (double _m00, double _m01, double _m02,  double _m03,
		double _m10, double _m11, double _m12,  double _m13,
		double _m20, double _m21, double _m22,  double _m23,
		double _m30, double _m31, double _m32,  double _m33) :
		m00 (_m00), m01 (_m01), m02 (_m02), m03 (_m03),
		m10 (_m10), m11 (_m11), m12 (_m12), m13 (_m13),
		m20 (_m20), m21 (_m21), m22 (_m22), m23 (_m23),
		m30 (_m30), m31 (_m31), m32 (_m32), m33 (_m33) { }
	inline Matrix (const Vector &a, const Vector &b, const Vector &c, const Vector &d) :
		m00 (a[0]), m01 (a[1]), m02 (a[2]), m03 (0.0),
		m10 (b[0]), m11 (b[1]), m12 (b[2]), m13 (0.0),
		m20 (c[0]), m21 (c[1]), m22 (c[2]), m23 (0.0),
		m30 (d[0]), m31 (d[1]), m32 (d[2]), m33 (1.0) { }
	virtual ~Matrix () { }
	
	inline const double * const operator [] (size_t i) const {
		assert (i < 4);
		return m[i];
	}
	inline double *operator [] (size_t i) {
		assert (i < 4);
		return m[i];
	}
	
	inline Matrix operator = (const Matrix &m) {
		m00 = m.m00, m01 = m.m01, m02 = m.m02, m03 = m.m03; 
		m10 = m.m10, m11 = m.m11, m12 = m.m12, m13 = m.m13; 
		m20 = m.m20, m21 = m.m21, m22 = m.m22, m23 = m.m23; 
		m30 = m.m30, m31 = m.m31, m32 = m.m32, m33 = m.m33; 
		return *this;
	}
	
	inline Matrix operator + () const {
		return *this;
	}
	inline Matrix operator - () const {
		return *this;
	}
	
	inline friend Matrix operator + (const Matrix &a, const Matrix &b) {
		return Matrix (a.m[0][0] + b.m[0][0], a.m[0][1] + b.m[0][1], a.m[0][2] + b.m[0][2], a.m[0][3] + b.m[0][3],
			a.m[1][0] + b.m[1][0], a.m[1][1] + b.m[1][1], a.m[1][2] + b.m[1][2], a.m[1][3] + b.m[1][3],
			a.m[2][0] + b.m[2][0], a.m[2][1] + b.m[2][1], a.m[2][2] + b.m[2][2], a.m[2][3] + b.m[2][3],
			a.m[3][0] + b.m[3][0], a.m[3][1] + b.m[3][1], a.m[3][2] + b.m[3][2], a.m[3][3] + b.m[3][3]);
	}
	inline friend Matrix operator - (const Matrix &a, const Matrix &b) {
		return Matrix (a.m[0][0] - b.m[0][0], a.m[0][1] - b.m[0][1], a.m[0][2] - b.m[0][2], a.m[0][3] - b.m[0][3],
			a.m[1][0] - b.m[1][0], a.m[1][1] - b.m[1][1], a.m[1][2] - b.m[1][2], a.m[1][3] - b.m[1][3],
			a.m[2][0] - b.m[2][0], a.m[2][1] - b.m[2][1], a.m[2][2] - b.m[2][2], a.m[2][3] - b.m[2][3],
			a.m[3][0] - b.m[3][0], a.m[3][1] - b.m[3][1], a.m[3][2] - b.m[3][2], a.m[3][3] - b.m[3][3]);
	}
	
	inline friend Matrix operator * (const Matrix &a, const Matrix &b) {
		return a.Concatenate (b);
	}
	inline friend Vector operator * (const Matrix &m, const Vector &v) {
		double inverse_w = 1.0 / (v[0] * m.m[3][0] + v[1] * m.m[3][1] + v[2] * m.m[3][2] + m.m[3][3]);
		return Vector ((v[0] * m.m[0][0] + v[1] * m.m[0][1] + v[2] * m.m[0][2] + m.m[0][3]) * inverse_w,
			(v[0] * m.m[1][0] + v[1] * m.m[1][1] + v[2] * m.m[1][2] + m.m[1][3]) * inverse_w,
			(v[0] * m.m[2][0] + v[1] * m.m[2][1] + v[2] * m.m[2][2] + m.m[2][3]) * inverse_w);
	}
	inline friend Vector operator * (const Vector &v, const Matrix &m) {
		double inverse_w = 1.0 / (v[0] * m.m[0][3] + v[1] * m.m[1][3] + v[2] * m.m[2][3] + m.m[3][3]);
		return Vector ((v[0] * m.m[0][0] + v[1] * m.m[1][0] + v[2] * m.m[2][0] + m.m[3][0]) * inverse_w,
			(v[0] * m.m[0][1] + v[1] * m.m[1][1] + v[2] * m.m[2][1] + m.m[3][1]) * inverse_w,
			(v[0] * m.m[0][2] + v[1] * m.m[1][2] + v[2] * m.m[2][2] + m.m[3][2]) * inverse_w);
	}
	inline friend Matrix operator * (const Matrix &m, const double s) {
		return Matrix (m.m[0][0] * s, m.m[0][1] * s, m.m[0][2] * s, m.m[0][3] * s,
			m.m[1][0] * s, m.m[1][1] * s, m.m[1][2] * s, m.m[1][3] * s,
			m.m[2][0] * s, m.m[2][1] * s, m.m[2][2] * s, m.m[2][3] * s,
			m.m[3][0] * s, m.m[3][1] * s, m.m[3][2] * s, m.m[3][3] * s);
	}
	inline friend Matrix operator * (const double s, const Matrix &m) {
		return Matrix (m[0][0] * s, m.m[0][1] * s, m.m[0][2] * s, m.m[0][3] * s,
			m.m[1][0] * s, m.m[1][1] * s, m.m[1][2] * s, m.m[1][3] * s,
			m.m[2][0] * s, m.m[2][1] * s, m.m[2][2] * s, m.m[2][3] * s,
			m.m[3][0] * s, m.m[3][1] * s, m.m[3][2] * s, m.m[3][3] * s);
	}
	
	inline Matrix Concatenate (const Matrix &m) const {
		Matrix r;
		r.m[0][0] = m[0][0] * m.m[0][0] + m[0][1] * m.m[1][0] + m[0][2] * m.m[2][0] + m[0][3] * m.m[3][0];
		r.m[0][1] = m[0][0] * m.m[0][1] + m[0][1] * m.m[1][1] + m[0][2] * m.m[2][1] + m[0][3] * m.m[3][1];
		r.m[0][2] = m[0][0] * m.m[0][2] + m[0][1] * m.m[1][2] + m[0][2] * m.m[2][2] + m[0][3] * m.m[3][2];
		r.m[0][3] = m[0][0] * m.m[0][3] + m[0][1] * m.m[1][3] + m[0][2] * m.m[2][3] + m[0][3] * m.m[3][3];
	
		r.m[1][0] = m[1][0] * m.m[0][0] + m[1][1] * m.m[1][0] + m[1][2] * m.m[2][0] + m[1][3] * m.m[3][0];
		r.m[1][1] = m[1][0] * m.m[0][1] + m[1][1] * m.m[1][1] + m[1][2] * m.m[2][1] + m[1][3] * m.m[3][1];
		r.m[1][2] = m[1][0] * m.m[0][2] + m[1][1] * m.m[1][2] + m[1][2] * m.m[2][2] + m[1][3] * m.m[3][2];
		r.m[1][3] = m[1][0] * m.m[0][3] + m[1][1] * m.m[1][3] + m[1][2] * m.m[2][3] + m[1][3] * m.m[3][3];
		
		r.m[2][0] = m[2][0] * m.m[0][0] + m[2][1] * m.m[1][0] + m[2][2] * m.m[2][0] + m[2][3] * m.m[3][0];
		r.m[2][1] = m[2][0] * m.m[0][1] + m[2][1] * m.m[1][1] + m[2][2] * m.m[2][1] + m[2][3] * m.m[3][1];
		r.m[2][2] = m[2][0] * m.m[0][2] + m[2][1] * m.m[1][2] + m[2][2] * m.m[2][2] + m[2][3] * m.m[3][2];
		r.m[2][3] = m[2][0] * m.m[0][3] + m[2][1] * m.m[1][3] + m[2][2] * m.m[2][3] + m[2][3] * m.m[3][3];
		
		r.m[3][0] = m[3][0] * m.m[0][0] + m[3][1] * m.m[1][0] + m[3][2] * m.m[2][0] + m[3][3] * m.m[3][0];
		r.m[3][1] = m[3][0] * m.m[0][1] + m[3][1] * m.m[1][1] + m[3][2] * m.m[2][1] + m[3][3] * m.m[3][1];
		r.m[3][2] = m[3][0] * m.m[0][2] + m[3][1] * m.m[1][2] + m[3][2] * m.m[2][2] + m[3][3] * m.m[3][2];
		r.m[3][3] = m[3][0] * m.m[0][3] + m[3][1] * m.m[1][3] + m[3][2] * m.m[2][3] + m[3][3] * m.m[3][3];
		
		return r;
	}
	
	inline Matrix Transpose () const {
		return Matrix (m[0][0], m[1][0], m[2][0], m[3][0],
			m[0][1], m[1][1], m[2][1], m[3][1],
			m[0][2], m[1][2], m[2][2], m[3][2],
			m[0][3], m[1][3], m[2][3], m[3][3]);
	}
	
	inline Matrix SetTranslation (const Vector &v) {
		m[3][0] = v[0], m[3][1] = v[1], m[3][2] = v[2];
		return *this;
	}
	inline Vector GetTranslation () const {
		return Vector (m[3][0], m[3][1], m[3][2]);
	}
	
	inline Matrix SetScale (const Vector &v) {
		m[0][0] = v[0], m[1][1] = v[1], m[2][2] = v[2];
		return *this;
	}
	inline Vector GetScale () const {
		return Vector (m[0][0], m[1][1], m[2][2]);
	}
	
	inline Matrix Adjoint () const {
		return Matrix (Minor (1, 2, 3, 1, 2, 3), -Minor (0, 2, 3, 1, 2, 3), Minor (0, 1, 3, 1, 2, 3), -Minor (0, 1, 2, 1, 2, 3),
			-Minor (1, 2, 3, 0, 2, 3), Minor (0, 2, 3, 0, 2, 3), -Minor (0, 1, 3, 0, 2, 3), Minor (0, 1, 2, 0, 2, 3),
			Minor (1, 2, 3, 0, 1, 3), -Minor (0, 2, 3, 0, 1, 3), Minor (0, 1, 3, 0, 1, 3), -Minor (0, 1, 2, 0, 1, 3),
			-Minor (1, 2, 3, 0, 1, 2), Minor (0, 2, 3, 0, 1, 2), -Minor (0, 1, 3, 0, 1, 2), Minor (0, 1, 2, 0, 1, 2)); 
	}
	
	inline double Determinant () const {
		return m[0][0] * Minor (1, 2, 3, 1, 2, 3) -
			m[0][1] * Minor (1, 2, 3, 0, 2, 3) +
			m[0][2] * Minor (1, 2, 3, 0, 1, 3) -
			m[0][3] * Minor (1, 2, 3, 0, 1, 2);
	}
	
	inline Matrix Inverse () const {
		return Adjoint () * (1.0 / Determinant ());
	}
	
	Matrix Orthonormal () const {
		Matrix r = *this;
		double inverse_length = 1.0 / sqrt (r.m[0][0] * r.m[0][0] + r.m[1][0] * r.m[1][0] + r.m[2][0] * r.m[2][0]);
		r.m[0][0] *= inverse_length;
		r.m[1][0] *= inverse_length;
		r.m[2][0] *= inverse_length;
		
		double dot0 = r.m[0][0] * r.m[0][1] + r.m[1][0] * r.m[1][1] + r.m[2][0] * r.m[2][1];
		r.m[0][1] -= dot0 * r.m[0][0];
		r.m[1][1] -= dot0 * r.m[1][0];
		r.m[2][1] -= dot0 * r.m[2][0];
		
		inverse_length = 1.0 / sqrt (r.m[0][1] * r.m[0][1] + r.m[1][1] * r.m[1][1] + r.m[2][1] * r.m[2][1]);
		r.m[0][1] *= inverse_length;
		r.m[1][1] *= inverse_length;
		r.m[2][1] *= inverse_length;
		
		double dot1 = r.m[0][1] * r.m[0][2] + r.m[1][1] * r.m[1][2] + r.m[2][1]* r.m[2][2];
		dot0 = r.m[0][0] * r.m[0][2] + r.m[1][0] * r.m[1][2] + r.m[2][0]* r.m[2][2];
		
		r.m[0][2] -= dot0 * r.m[0][0] + dot1 * r.m[0][1];
		r.m[1][2] -= dot0 * r.m[1][0] + dot1 * r.m[1][1];
		r.m[2][2] -= dot0 * r.m[2][0] + dot1 * r.m[2][1];
		
		inverse_length = 1.0 / sqrt (r.m[0][2] * r.m[0][2] + r.m[1][2] * r.m[1][2] + r.m[2][2] * r.m[2][2]);
		r.m[0][2] *= inverse_length;
		r.m[1][2] *= inverse_length;
		r.m[2][2] *= inverse_length;
		
		return r;
	}
	
	inline static Matrix Translation (const Vector &v) {
		return Matrix (1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			v[0], v[1], v[2], 1.0);
	}
	inline static Matrix Translation (const double x, const double y, const double z) {
		return Matrix (1.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			x, y, z, 1.0);
	}
	
	inline static Matrix Scale (const Vector &v) {
		return Matrix (v[0], 0.0, 0.0, 0.0,
			0.0, v[1], 0.0, 0.0,
			0.0, 0.0, v[2], 0.0,
			0.0, 0.0, 0.0, 1.0);
	}
	inline static Matrix Scale (const double x, const double y, const double z) {
		return Matrix (x, 0.0, 0.0, 0.0,
			0.0, y, 0.0, 0.0,
			0.0, 0.0, z, 0.0,
			0.0, 0.0, 0.0, 1.0);
	}
	
	inline static Matrix Skew (const Vector &v) {
		return Matrix (0.0, -v[2], v[1], 0.0,
			v[2], 0.0, -v[0], 0.0,
			-v[1], v[0], 0.0, 0.0,
			0.0, 0.0, 0.0, 0.0);
	}
	inline static Matrix Skew (const double x, const double y, const double z) {
		return Matrix (0.0, -z, y, 0.0,
			z, 0.0, -x, 0.0,
			-y, x, 0.0, 0.0,
			0.0, 0.0, 0.0, 0.0);
	}
	
	inline static Matrix RotateYaw (const double angle) {
		double c = cos (angle);
		double s = sin (angle);

		return Matrix (1.0, 0.0, 0.0, 0.0,
			0.0, c, -s, 0.0,
			0.0, s, c, 0.0,
			0.0, 0.0, 0.0, 1.0);
	}
	
	inline static Matrix RotatePitch (const double angle) {
		double c = cos (angle);
		double s = sin (angle);

		return Matrix (c, 0.0, s, 0.0,
			0.0, 1.0, 0.0, 0.0,
			-s, 0.0, c, 0.0,
			0.0, 0.0, 0.0, 1.0);
	}
	
	inline static Matrix RotateRoll (const double angle) {
		double c = cos (angle);
		double s = sin (angle);
		
		return Matrix (c, -s, 0.0, 0.0,
			s, c, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 1.0);
	}
	
	inline static Matrix Rotation (const Vector &v) {
		return RotateRoll (v[2]) * RotatePitch (v[1]) * RotateYaw (v[0]);
	}
	inline static Matrix Rotation (const double y, const double p, const double r) {
		return RotateRoll (r) * RotatePitch (p) * RotateYaw (y);
	}
	
}; /* class Matrix */

/*---------------------------------------------------------------------------*/
#endif /* __MATRIX_H_ */
