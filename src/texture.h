#ifndef __TEXTURE_H_
#define __TEXTURE_H_

#pragma once
/*---------------------------------------------------------------------------*/

class Texture : public gc_cleanup {
	typedef enum { TF_BILINEAR, TF_TRILINEAR, TF_ANISOTROPIC, } TextureFilter;

private:
	typedef std::vector<Canvas *, gc_allocator<Canvas *> > MipMapList;

	void GenerateMipMapLevels () {
		Canvas *working = mipmap_levels[0];
		size_t width = working->Width (), curr_width = width >>= 1;
		size_t height = working->Height (), curr_height = height >>= 1;
		Canvas *next = NULL;
		
		while (curr_width != 0 && curr_height != 0) {
			next = new Canvas (curr_width, curr_height);
			size_t index = 0;
			for (size_t i = 0; i < curr_width; ++i) {
				for (size_t j = 0; j < curr_height; ++j) {
					double r =
						((*working)[(i * 2) + (j * 2) * width][0]
						+ (*working)[(i * 2 + 1) + (j * 2) * width][0]
						+ (*working)[(i * 2) + (j * 2 + 1) * width][0]
						+ (*working)[(i * 2 + 1) + (j * 2 + 1) * width][0])
						* 0.25;
					double g =
						((*working)[(i * 2) + (j * 2) * width][1]
						+ (*working)[(i * 2 + 1) + (j * 2) * width][1]
						+ (*working)[(i * 2) + (j * 2 + 1) * width][1]
						+ (*working)[(i * 2 + 1) + (j * 2 + 1) * width][1])
						* 0.25;
					double b =
						((*working)[(i * 2) + (j * 2) * width][2]
						+ (*working)[(i * 2 + 1) + (j * 2) * width][2]
						+ (*working)[(i * 2) + (j * 2 + 1) * width][2]
						+ (*working)[(i * 2 + 1) + (j * 2 + 1) * width][2])
						* 0.25;
					next->Set (index, Color (r, g, b));
					++index;
				}
			}
			working = next;
			mipmap_levels.push_back (working);
			width = curr_width, height = curr_height;
			curr_width >>= 1, curr_height >>= 1;
		}
	}
	
	MipMapList mipmap_levels;
	
	inline double CalculateMipMap (const Vector &p) const {
		return p[2] * mipmap_levels.size () * 0.001;
	}
	
	inline Canvas *GetMipMap (double mipmap) const {
		int whole_part = (int) mipmap;
		if (whole_part < 0) {
			whole_part = -whole_part;
		}
		size_t index = whole_part;
		if (index >= mipmap_levels.size ()) {
			index = mipmap_levels.size () - 1;
		}
		return mipmap_levels[(size_t) index];
	}
	
	inline Color BilinearFilter (const double mipmap, double u, double v) const {
		Canvas *working = GetMipMap (mipmap);
		size_t width = working->Width ();
		size_t height = working->Height ();
		
		double fu = u * (double) width;
		double fv = v * (double) height;
		size_t u1 = (size_t) fu % width;
		size_t v1 = (size_t) fv % height;
		size_t u2 = (size_t) (fu + 1.0) % width;
		size_t v2 = (size_t) (fv + 1.0) % height;
		double ufrac = fu - floor (fu);
		double vfrac = fv - floor (fv);
		double w1 = (1.0 - ufrac) * (1.0 - vfrac);
		double w2 = ufrac * (1.0 - vfrac);
		double w3 = (1.0 - ufrac) * vfrac;
		double w4 = ufrac * vfrac;
		
		Color c1 = (*working)[v1 * width + u1];
		Color c2 = (*working)[v1 * width + u2];
		Color c3 = (*working)[v2 * width + u1];
		Color c4 = (*working)[v2 * width + u2];
		
		return c1 * w1 + c2 * w2 + c3 * w3 + c4 * w4;
	}
	
	inline Color BilinearFilter (const Vector &p, double u, double v) const {
		double mipmap = ((double) (mipmap_levels.size () - 1) * p[2]) * 0.0001;
		return BilinearFilter (mipmap, u, v);
	}

	inline Color TrilinearFilter (const Vector &p, double u, double v) const {
		double mipmap = ((double) (mipmap_levels.size () - 1) * p[2]) * 0.0001;
		double fract = mipmap - floor (mipmap);
		if (mipmap > (mipmap_levels.size () - 2)) {
			mipmap = (mipmap_levels.size () - 2);
		}
		return (1.0 - fract) * BilinearFilter (mipmap, u, v) + fract * BilinearFilter (mipmap + 1.0, u, v);
	}

	inline Color AnisotropicFilter (const Vector &p, double u, double v) const {
		// TODO: Complete this method
		return TrilinearFilter (p, u, v);
	}

protected:
	Texture () : mipmap_levels () { }

public:
	inline Texture (Canvas *canvas) : mipmap_levels () {
		mipmap_levels.push_back (canvas);
		GenerateMipMapLevels ();
	}
	virtual ~Texture () {
	  mipmap_levels.clear ();
	}
	
	operator Canvas () const {
		return (*AsCanvas ());
	}

	inline Color operator [] (size_t index) const {
		return (*mipmap_levels[0])[index];
	}
	inline Color &operator[] (size_t index) {
		return (*mipmap_levels[0])[index];
	}
	
	Canvas *AsCanvas() const {
		return mipmap_levels[0];
	}

	virtual Color Filter (const Vector &p, double u, double v) const {
		// TODO: Make this configurable?
		return TrilinearFilter (p, u, v);
	}

};

/*---------------------------------------------------------------------------*/

// Texture *GenerateCheckerboard (const Color &color1 = Color::WHITE, const Color &color2 = Color::BLACK);
void WriteCanvasToTgaImage (const char *name, const Canvas &canvas);
extern void (*WriteTgaImage) (const char *, unsigned int *, size_t, size_t);
extern unsigned int *(*LoadTgaImage) (const char *, size_t&, size_t&);

/*---------------------------------------------------------------------------*/
#endif /* __TEXTURE_H_ */
