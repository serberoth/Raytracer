#ifndef __PROCEDURAL_H__
#define __PROCEDURAL_H__

#pragma once
/*---------------------------------------------------------------------------*/

class Procedural : public Texture {
public:
	typedef struct WeightedColor : virtual public gc {
		double weight;
		Color color;

		WeightedColor (double w, const Color &c) : weight (w), color (c) { }
		WeightedColor (const WeightedColor &c) : weight (c.weight), color (c.color) { }
		~WeightedColor () { }

		inline WeightedColor &operator = (const WeightedColor &c) {
			weight = c.weight;
			color = c.color;
			return *this;
		}

		inline bool operator == (const WeightedColor &c) const {
            double diff = c.weight - weight;
            return (diff > std::numeric_limits<double>::epsilon ()) && (diff < std::numeric_limits<double>::epsilon ());
		}
		inline bool operator == (double _weight) const {
            double diff = _weight - weight;
            return (diff > std::numeric_limits<double>::epsilon ()) && (diff < std::numeric_limits<double>::epsilon ());
		}

		inline bool operator < (const WeightedColor &c) const {
			return weight < c.weight;
		}
		inline bool operator > (const WeightedColor &c) const {
			return weight > c.weight;
		}
		inline bool operator <= (const WeightedColor &c) const {
			return !(*this > c);
		}
		inline bool operator >= (const WeightedColor &c) const {
			return !(*this < c);
		}

		inline friend bool operator < (double weight, const WeightedColor &c) {
			return weight < c.weight;
		}
		inline friend bool operator > (double weight, const WeightedColor &c) {
			return weight > c.weight;
		}
		inline friend bool operator <= (double weight, const WeightedColor &c) {
			return !(weight > c.weight);
		}
		inline friend bool operator >= (double weight, const WeightedColor &c) {
			return !(weight < c.weight);
		}

	};

	typedef std::vector<WeightedColor, gc_allocator<WeightedColor> > GradientList;

private:
    GradientList gradient;
    NoiseMap *map;
    NoiseType *type;

public:
	Procedural (NoiseType *t, NoiseMap::Mapping mapping = NoiseMap::NM_PLANE) : Texture (), gradient (), map (NULL), type (t) {
		PerlinNoise::Initialize ();
		map = new NoiseMap (type, 128, 128, mapping);
	}
	virtual ~Procedural () {
		if (map != NULL) {
			delete map;
			map = NULL;
		}
		type = NULL;
	}

	operator Canvas () const {
		return (*AsTexture ());
	}

    Texture *AsTexture () const {
    	Canvas *canvas = new Canvas (map->Width (), map->Height ());
    	size_t offset = 0;
    	for (size_t y = 0; y < map->Height (); ++y) {
    		for (size_t x = 0; x < map->Width (); ++x) {
    			double noise = map->Get (x, y);
    			GradientList::size_type index = 0;
    			for (GradientList::const_iterator iter = gradient.begin (); iter != gradient.end (); ++iter, ++index) {
    				if (noise < (*iter).weight) {
    					break;
    				}
    			}
    			GradientList::size_type index0 = index - 1;
    			index0 = ((index0 < 0) ? 0 : ((index0 >= gradient.size ()) ? gradient.size () - 1 : index0));
    			GradientList::size_type index1 = index;
    			index1 = ((index1 < 0) ? 0 : ((index0 >= gradient.size ()) ? gradient.size () - 1 : index1));

				niesoft::StringOutputStream out;
    			if (index0 == index1) {
					out << "[" << setw(3) << x << ", " << setw(3) << y << " (" << setw(6) << offset << ")] " << gradient[index0].color;
					niesoft::Logger::debug (out.str ());
    				canvas->Set (offset, gradient[index0].color);
    				++offset;
    			} else {
					double weight0 = gradient[index0].weight;
					double weight1 = gradient[index1].weight;
					double alpha = (noise - weight0) / (weight1 - weight0);

					out.clear (); out.str ("");
					out << "noise => " << noise << ", weights => { 0 => " << weight0 << ", 1 => " << weight1 << "}, alpha => " << alpha;
					niesoft::Logger::debug (out.str ());

					Color color = gradient[index0].color.Blend (gradient[index1].color, alpha);
					out.clear (); out.str ("");
					out << "[" << setw(3) << x << ", " << setw(3) << y << " (" << setw(6) << offset << ")] " << color;
					niesoft::Logger::debug (out.str ());
					canvas->Set (offset, color);
					++offset;
    			}
    		}
    	}
    	return new Texture (canvas);
    }

	virtual inline Color Filter (const Vector &p, double u, double v) const {
		double noise = map->Get (u, v);
		GradientList::size_type index = 0;
		for (GradientList::const_iterator iter = gradient.begin (); iter != gradient.end (); ++iter, ++index) {
			if (noise < *iter) {
				break;
			}
		}
		GradientList::size_type index0 = index - 1;
		index0 = ((index0 < 0) ? 0 : ((index0 >= gradient.size ()) ? gradient.size () - 1 : index0));
		GradientList::size_type index1 = index;
		index1 = ((index1 < 0) ? 0 : ((index1 >= gradient.size ()) ? gradient.size () - 1 : index1));

		if (index0 == index1) {
			return gradient[index0].color;
		}

		double weight0 = gradient[index0].weight;
		double weight1 = gradient[index1].weight;
		double alpha = (noise - weight0) / (weight1 - weight0);

		return gradient[index0].color.BlendLerp (gradient[index1].color, alpha);
	}

	virtual inline void AddColor (double weight, const Color &color) {
		if (!gradient.empty ()) {
			for (GradientList::iterator iter = gradient.begin (); iter != gradient.end (); ++iter) {
				if ((*iter) == weight) {
					(*iter).color = color;
					return;
				}
			}
		}
		gradient.push_back (WeightedColor (weight, color));
		std::sort (gradient.begin (), gradient.end ());
	}

};

/*---------------------------------------------------------------------------*/

class Checkerboard : public Procedural {
private:
	class Checker : public NoiseType {
	public:
		Checker () : NoiseType () { }
		virtual ~Checker () { }

		virtual inline double Noise (const Vector &p) const {
			int ix = (int) floor (PerlinNoise::IntRange (p[0]));
			int iy = (int) floor (PerlinNoise::IntRange (p[1]));
			int iz = (int) floor (PerlinNoise::IntRange (p[2]));

			return ((ix & 1) ^ (iy & 1) ^ (iz & 1)) ? -1.0 : 1.0;
		}

	};

public:
	Checkerboard (const Color &color1 = Color::WHITE, const Color &color2 = Color::BLACK) : Procedural (new Checker ()) {
		Procedural::AddColor (-1.0, color1);
		Procedural::AddColor (1.0, color2);
	}
	virtual ~Checkerboard () { }

	virtual void AddColor (double weight, const Color &color) {
		// Do nothing, mwa, ha, ha, ha!!!
	}

};

/*---------------------------------------------------------------------------*/

class BasicPerlin : public Procedural {
public:
	BasicPerlin () : Procedural (new Perlin ()){ }
	virtual ~BasicPerlin () { }

};

/*---------------------------------------------------------------------------*/

class Wood : public Procedural, public NoiseType {
private:
	NoiseType *source;

public:
	Wood () : Procedural (this), source (NULL) {
		Cylindrical base;
		base.Frequency() = 16.0;
		Perlin grain;
		grain.Seed () = 0;
		grain.Frequency () = 48.0;
		grain.Persistance () = 0.5;
		grain.Lacunarity () = 2.20703125;
		grain.Octaves () = 3;
		Scalar scaled = Scalar (&grain);
		scaled.Scale () = Vector (1.0, 0.25, 1.0);
		Biased biased = Biased (&scaled);
		biased.Scale () = 0.25;
		biased.Bias () = 0.125;
		Additive added = Additive (&base, &biased);
		Turbulence turbulence = Turbulence (&added);
		turbulence.SetSeed (1);
		turbulence.SetFrequency (4.0);
		turbulence.SetRoughness (4);
		turbulence.Power () = 1.0 / 256.0;

		source = &turbulence;
	}
	virtual ~Wood () { }

	virtual inline double Noise (const Vector &p) const {
		return source->Noise (p);
	}

};

/*---------------------------------------------------------------------------*/

class Turbulance : public Procedural {
public:
	Turbulance () : Procedural (NULL) { }
	virtual ~Turbulance () { }

    virtual inline double Noise (const Vector &p) const {
        // sum ( 1 / f * ( noise ) )
    	/*
        return PerlinNoise::Noise (p) + 0.5 * PerlinNoise::Noise (p * 2.0)
                + 0.25 * PerlinNoise::Noise (p * 4.0) + 0.125 * PerlinNoise::Noise (p * 8.0)
                + 0.0125 * PerlinNoise::Noise (p * 16.0) + 0.00125 * PerlinNoise::Noise (p * 32.0)
                + 0.000125 * PerlinNoise::Noise (p * 64.0) + 0.0000125 * PerlinNoise::Noise (p * 128.0);
         */
    	return 0.0;
    }

};

/*---------------------------------------------------------------------------*/

class Lava : public Procedural {
public:
    Lava () : Procedural (NULL) { }
    virtual ~Lava () { }

    virtual inline double Noise (const Vector &p) const {
        // sum ( 1 / f * ( |noise| ) )
    	/*
        return abs (PerlinNoise::Noise (p)) + 0.5 * abs (PerlinNoise::Noise (p * 2.0))
                + 0.25 * abs (PerlinNoise::Noise (p * 4.0)) + 0.125 * abs (PerlinNoise::Noise (p * 8.0))
                + 0.0125 * abs (PerlinNoise::Noise (p * 16.0)) + 0.00125 * abs (PerlinNoise::Noise (p * 32.0))
                + 0.000125 * abs (PerlinNoise::Noise (p * 64.0)) + 0.0000125 * abs (PerlinNoise::Noise (p * 128.0));
         */
    	return 0.0;
    }

};

/*---------------------------------------------------------------------------*/

class Marble : public Procedural {
public:
	Marble () : Procedural (NULL) { }
	~Marble () { }

    virtual inline double Noise (const Vector &p) const {
        // sin ( x + sum ( 1 / f * ( |noise| ) ) )
    	/*
        return sin (p[0] + abs (PerlinNoise::Noise (p)) + 0.5 * abs (PerlinNoise::Noise (p * 2.0))
                + 0.25 * abs (PerlinNoise::Noise (p * 4.0)) + 0.125 * abs (PerlinNoise::Noise (p * 8.0))
                + 0.0125 * abs (PerlinNoise::Noise (p * 16.0)) + 0.00125 * abs (PerlinNoise::Noise (p * 32.0))
                + 0.000125 * abs (PerlinNoise::Noise (p * 64.0)) + 0.0000125 * abs (PerlinNoise::Noise (p * 128.0)));
         */
    	return 0.0;
    }

};

/*---------------------------------------------------------------------------*/
#endif /* __PROCEDURAL_H__ */
