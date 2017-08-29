#ifndef __NOISE_H_
#define __NOISE_H_

#pragma once
/*---------------------------------------------------------------------------*/

/*
 * This source is an adaptation of the Java source by Ken Perlin available at
 * the following URL: http://mrl.nyu.edu/~perlin/noise/
 */
class PerlinNoise : virtual public gc_cleanup {
private:
	static const size_t MAX_OCTAVES;
	static const int PERMUTATION[];
	static vector<Vector, gc_allocator<Vector> > permutation_vector;

	PerlinNoise () { }

	inline static double Fade (double t) {
		return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
	}

	inline static double Lerp (double a, double b, double t) {
		return a + t * (b - a);
	}

	inline static double Grad (int ix, int iy, int iz, double x, double y, double z, int seed) {
		int index = (31337 * ix + 6971 * iy + 1619 * iz + 1013 * seed) & 0xffffffff;
		index = ((index ^ (index >> 8)) & 0xff); // << 2;

		Vector v = permutation_vector[index];
		Vector p = Vector (x - (double) ix, y - (double) iy, z - (double) iz);

		return v.Dot (p) * 2.12;
	}

	inline static double Value (int ix, int iy, int iz, double x, double y, double z, int seed) {
		int n = (31337 * ix + 6971 * iy + 1619 * iz + 1013 * seed) & 0x7fffffff;

		n = (n >> 13) ^ n;
		n = (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;

		return 1.0 - ((double) n / 1073741824.0);
	}

	static double Noise (double x, double y, double z, int seed) {
		int ix = (x > 0.0) ? (int) x : (int) x - 1;
		int iy = (y > 0.0) ? (int) y : (int) y - 1;
		int iz = (z > 0.0) ? (int) z : (int) z - 1;

		double u = Fade (x - (double) ix);
		double v = Fade (y - (double) iy);
		double w = Fade (z - (double) iz);

		return Lerp (w, Lerp (v,	Lerp (u,	Grad (ix,     iy,     iz,     x, y, z, seed),
												Grad (ix + 1, iy,     iz,     x, y, z, seed)),
									Lerp (u,	Grad (ix,     iy + 1, iz,     x, y, z, seed),
												Grad (ix + 1, iy + 1, iz,     x, y, z, seed))),
						Lerp (v,	Lerp (u,	Grad (ix,     iy,     iz + 1, x, y, z, seed),
												Grad (ix + 1, iy,     iz + 1, x, y, z, seed)),
									Lerp (u,	Grad (ix,     iy + 1, iz + 1, x, y, z, seed),
												Grad (ix,     iy + 1, iz + 1, x, y, z, seed))));
	}

public:
	~PerlinNoise () { }

	static void Initialize (unsigned long seed = 0) {
		if (permutation_vector.empty ()) {
			niesoft::Logger::debug ("Initializing Perlin Noise");
			Random random (seed);
			for (int i = 0; i < 256; ++i) {
				Vector v = Vector (random.RandomReal (), random.RandomReal (), random.RandomReal ());
				// niesoft::StringOutputStream out;
				// out << "Perlin Permutation Vector[" << setw(3) << i << "] := " << v;
				// niesoft::Logger::debug (out.str ());
				permutation_vector.push_back (v);
			}
		}
	}

	static double Noise (const Vector &pos, size_t octaves, int seed, double frequency, double lacunarity, double persistance) {
		double value = 0.0;
		Vector v = pos, iv;
		double curr_persistance = 1.0;

		if (octaves > MAX_OCTAVES) {
			octaves = MAX_OCTAVES;
		}

		v *= frequency;

		niesoft::StringOutputStream out;
		for (size_t i = 0; i < octaves; ++i) {
			iv = Vector (IntRange (v[0]), IntRange (v[1]), IntRange (v[2]));
			value += curr_persistance * Noise (iv[0], iv[1], iv[2], (seed + i) & 0xffffffff);
			v *= lacunarity;
			out << "Current Noise Vectors { V => " << v << ", IV => " << iv <<  " }";
			niesoft::Logger::debug (out.str ());
			out << "Current Noise Value { Octave => " << setw(2) << i << ", Persistance => " << curr_persistance << ", value => " << value <<  " }";
			niesoft::Logger::debug (out.str ());
			curr_persistance *= persistance;
		}
		return value;
	}

	static double IntRange (double value) {
		if (value >= 1073741824.0) {
			return 2.0 * fmod (value, 1073741824.0) - 1073741824.0;
		} else if (value <= -1073741824.0) {
			return 2.0 * fmod (value, 1073741824.0) + 1073741824.0;
		}
		return value;
	}

};

/*---------------------------------------------------------------------------*/

typedef class NoiseType : virtual public gc_cleanup {
public:
	NoiseType () { }
	virtual ~NoiseType () { }

	virtual double Noise (const Vector &p) const = 0;

};

/*---------------------------------------------------------------------------*/

class Perlin : public NoiseType {
private:
	int octaves;
	int seed;
	double frequency;
	double lacunarity;
	double persistance;

public:
	Perlin () : NoiseType (), octaves (8), seed (0), frequency (1.0), lacunarity (2.0), persistance (0.5) { }
	virtual ~Perlin () { }

	int Octaves () const {
		return octaves;
	}
	int &Octaves () {
		return octaves;
	}

	int Seed () const {
		return seed;
	}
	int &Seed () {
		return seed;
	}

	double Frequency () const {
		return frequency;
	}
	double &Frequency () {
		return frequency;
	}

	double Lacunarity () const {
		return lacunarity;
	}
	double &Lacunarity () {
		return lacunarity;
	}

	double Persistance () const {
		return persistance;
	}
	double &Persistance () {
		return persistance;
	}

	virtual inline double Noise (const Vector &p) const {
		double noise = PerlinNoise::Noise (p, octaves, seed, frequency, lacunarity, persistance);
		niesoft::StringOutputStream out;
		out << "Perlin::Noise Value " << noise;
		niesoft::Logger::debug (out.str ());
		return noise;
	}

};

/*---------------------------------------------------------------------------*/

class Scalar : public NoiseType {
private:
	NoiseType *source;
	Vector scale;

public:
	Scalar (NoiseType *src) : NoiseType (), source (src), scale () { }
	virtual ~Scalar () { }

	Vector Scale () const {
		return scale;
	}
	Vector &Scale () {
		return scale;
	}

	virtual inline double Noise (const Vector &p) const {
		return source->Noise (p * scale);
	}

};

/*---------------------------------------------------------------------------*/

class Biased : public NoiseType {
private:
	NoiseType *source;
	double scale;
	double bias;

public:
	Biased (NoiseType *src) : NoiseType (), source (src), scale (1.0), bias (0.0) { }
	virtual ~Biased () { }

	double Scale () const {
		return scale;
	}
	double &Scale () {
		return scale;
	}

	double Bias () const {
		return bias;
	}
	double &Bias () {
		return bias;
	}

	virtual inline double Noise (const Vector &p) const {
		return source->Noise (p) * scale + bias;
	}

};

/*---------------------------------------------------------------------------*/

class AbsValue : public NoiseType {
private:
	NoiseType *source;

public:
	AbsValue (NoiseType *src) : NoiseType (), source (src) { }
	virtual ~AbsValue () { }

	virtual inline double Noise (const Vector &p) const {
		return abs (source->Noise (p));
	}

};

/*---------------------------------------------------------------------------*/

class Additive : public NoiseType {
private:
	NoiseType *source0;
	NoiseType *source1;

public:
	Additive (NoiseType *src0, NoiseType *src1) : NoiseType (), source0 (src0), source1 (src1) { }
	virtual ~Additive () { }

	virtual inline double Noise (const Vector &p) const {
		return source0->Noise (p) + source1->Noise (p);
	}

};

/*---------------------------------------------------------------------------*/

class Turbulence : public NoiseType {
private:
	NoiseType *source;
	Perlin x_module;
	Perlin y_module;
	Perlin z_module;
	double power;

public:
	Turbulence (NoiseType *src) : NoiseType (), source (src), x_module (), y_module (), z_module (), power (1.0) { }
	virtual ~Turbulence () { }

	void SetRoughness (int roughness) {
		x_module.Octaves () = roughness;
		y_module.Octaves () = roughness;
		z_module.Octaves () = roughness;
	}

	void SetSeed (int seed) {
		x_module.Seed () = seed;
		y_module.Seed () = seed + 1;
		z_module.Seed () = seed + 2;
	}

	void SetFrequency (double freq) {
		x_module.Frequency () = freq;
		y_module.Frequency () = freq;
		z_module.Frequency () = freq;
	}

	double Power () const {
		return power;
	}
	double &Power () {
		return power;
	}

	virtual inline double Noise (const Vector &p) const {
		Vector v0 = Vector (p[0] + (12414.0 / 65536.0), p[1] + (65124.0 / 65536.0), p[2] + (31337.0 / 65536.0));
		Vector v1 = Vector (p[0] + (26519.0 / 65536.0), p[1] + (18128.0 / 65536.0), p[2] + (60493.0 / 65536.0));
		Vector v2 = Vector (p[0] + (53820.0 / 65536.0), p[1] + (11213.0 / 65536.0), p[2] + (44845.0 / 65536.0));

		Vector distort = p + (Vector (x_module.Noise (v0), y_module.Noise (v1), z_module.Noise (v2)) * power);
		return source->Noise (distort);
	}

};

/*---------------------------------------------------------------------------*/

class Cylindrical : public NoiseType {
private:
	double frequency;

public:
	Cylindrical () : NoiseType (), frequency (1.0) { }
	virtual ~Cylindrical () { }

	double Frequency () const {
		return frequency;
	}
	double &Frequency () {
		return frequency;
	}

	virtual inline double Noise (const Vector &p) const {
		Vector v = Vector (p[0] * frequency, p[1], p[2] * frequency);
		double dist = sqrt (v[0] * v[0] + v[2] * v[2]);
		double dist0 = dist - floor (dist);
		double dist1 = 1.0 - dist0;
		double near = dist0 < dist1 ? dist0 : dist1;
		return 1.0 - (near * 4.0);
	}

};

/*---------------------------------------------------------------------------*/

typedef class NoiseMap {
public:
	typedef enum { NM_PLANE, NM_SPHERE, } Mapping;

private:
	double *noise;
	size_t width;
	size_t height;
	size_t length;

	double xLoBound, xHiBound;
	double zLoBound, zHiBound;
	double xExtent, zExtent;

	inline static double Lerp (double a, double b, double t) {
		return a + t * (b - a);
	}

	inline void CalculateExtents (Mapping m) {
		switch (m) {
		default:
		case NM_PLANE:
			xLoBound = -1.0, xHiBound = 1.0;
			zLoBound = -1.0, zHiBound = 1.0;
			xExtent = xHiBound - xLoBound;
			zExtent = zHiBound - zLoBound;
			break;

		case NM_SPHERE:
			// z : latitude
			// x : longitude
			xLoBound = -HALF_PI, xHiBound = HALF_PI;
			zLoBound = -PI, zHiBound = PI;
			xExtent = xHiBound - xLoBound;
			zExtent = zHiBound - zLoBound;
			width *= 2;
			length = width * height;
			break;
		}
	}

	inline double CalculateValue (const NoiseType *type, double x, double z, Mapping m) const {
		switch (m) {
		default:
		case NM_PLANE: {
			double xBlend = 1.0 - ((x - xLoBound) / xExtent);
			double zBlend = 1.0 - ((z - zLoBound) / zExtent);
			return Lerp (	Lerp (type->Noise (Vector (x, 0.0, z)),				type->Noise (Vector (x + xExtent, 0.0, z)),				xBlend),
							Lerp (type->Noise (Vector (x, 0.0, z + zExtent)),	type->Noise (Vector (x + xExtent, 0.0, z + zExtent)),	xBlend),
							zBlend);
		} break;

		case NM_SPHERE: {
			// z : latitude
			// x : longitude
			double r = cos (z);
			double sx = r * cos (x);
			double sy = sin (z);
			double sz = r * sin (x);
			return type->Noise (Vector (sx, sy, sz));
		} break;
		}

		return 0.0;
	}

public:
	NoiseMap (const NoiseType *type, size_t w, size_t h, Mapping m = NM_PLANE) : noise (NULL), width (w), height (h), length (w * h),
		xLoBound (0.0), xHiBound (0.0), zLoBound (0.0), zHiBound (0.0), xExtent (0.0), zExtent (0.0) {
		noise = new (GC) double[length + 1];
		double *work = noise;
		CalculateExtents (m);

		double dx = xExtent / (double) width;
		double dz = zExtent / (double) height;
		double x = xLoBound;
		double z = zLoBound;

		niesoft::StringOutputStream out;
		for (size_t j = 0; j < height; ++j) {
			x = xLoBound;
			for (size_t i = 0; i < width; ++i) {
				double value = CalculateValue (type, x, z, m);
				out.clear (); out.str ("");
				out << "Init NoiseMap[" << i << ", " << j << "] => " << value;
				niesoft::Logger::debug (out.str ());
				*work++ = value;
		        x += dx;
			}
			z += dz;
		}
	}
	~NoiseMap () {
		if (noise != NULL) {
			delete[] noise;
			noise = NULL;
		}
	}

	inline double operator [] (size_t index) const {
		assert (index < length);
		return *(noise + index);
	}

	inline size_t Width () const {
		return width;
	}
	inline size_t Height () const {
		return height;
	}

	inline double Get (size_t x, size_t y) const {
		niesoft::StringOutputStream out;
		out << "NoiseMap[" << x << ", " << y << "] => " << *(noise + (x + width * y));
		niesoft::Logger::debug (out.str ());
		return *(noise + (x + width * y));
	}
	inline double Get (double u, double v) const {
        double fu = u * (double) width;
        double fv = v * (double) height;
        size_t x = (size_t) fu % width;
        size_t y = (size_t) fv % height;
        return *(noise + (x + width * y));
	}

};

/*---------------------------------------------------------------------------*/
#endif /* __NOISE_H__ */
