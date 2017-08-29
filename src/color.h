#ifndef __COLOR_H_
#define __COLOR_H_

#pragma once
/*---------------------------------------------------------------------------*/

class Color : public gc {
private:
	union {
		struct {
			double v[4];
		};
		struct {
			double r, g, b, a;
		};
	};

	inline static double Clamp (double &n) {
		if (n < 0.0) {
			n = 0.0;
		} else if (n > 1.0) {
			n = 1.0;
		}
		return n;
	}

public:
	static const Color BLACK;
	static const Color WHITE;
	static const Color RED;
	static const Color GREEN;
	static const Color BLUE;
	static const Color CYAN;
	static const Color YELLOW;
	static const Color MAGENTA;
	static const Color LIGHT_GRAY;
	static const Color GRAY;
	static const Color DARK_GRAY;

	inline Color () : r (0.0), g (0.0), b (0.0), a (1.0) { }
	inline Color (double _r, double _g, double _b, double _a = 1.0) : r (_r), g (_g), b (_b), a (_a) { }
	inline explicit Color (unsigned int c) : r ((double) ((c >> 16) & 0xff) * 0.00390625), g ((double) ((c >> 8) & 0xff) * 0.00390625), b ((double) (c & 0xff) * 0.00390625), a ((double) ((c >> 24) & 0xff) * 0.00390625) { }
	virtual ~Color () { }

	inline Color operator = (const Color &c) {
		r = c.r, g = c.g, b = c.b; a = c.a;
		return *this;
	}

	inline operator unsigned int () const {
		return To32bit ();
	}

	inline friend Color operator + (const Color &a, const Color &b) {
		return Color (a.r + b.r, a.g + b.g, a.b + b.b);
	}
	inline friend Color operator + (double s, const Color &c) {
		return Color (s + c.r, s + c.g, s + c.b);
	}
	inline friend Color operator + (const Color &c, double s) {
		return Color (c.r + s, c.g + s, c.b + s);
	}

	inline friend Color operator - (const Color &a, const Color &b) {
		return Color (a.r - b.r, a.g - b.g, a.b - b.b);
	}
	inline friend Color operator - (double s, const Color &c) {
		return Color (s - c.r, s - c.g, s - c.b);
	}
	inline friend Color operator - (const Color &c, double s) {
		return Color (c.r - s, c.g - s, c.b - s);
	}

	inline friend Color operator * (const Color &a, const Color &b) {
		return Color (a.r * b.r, a.g * b.g, a.b * b.b);
	}
	inline friend Color operator * (double s, const Color &c) {
		return Color (s * c.r, s * c.g, s * c.b);
	}
	inline friend Color operator * (const Color &c, double s) {
		return Color (c.r * s, c.g * s, c.b * s);
	}

	inline friend Color operator / (const Color &a, const Color &b) {
		return Color (a.r / b.r, a.g / b.g, a.b / b.b);
	}
	inline friend Color operator / (double s, const Color &c) {
		return Color (s / c.r, s / c.g, s / c.b);
	}
	inline friend Color operator / (const Color &c, const double s) {
		assert (s <= -std::numeric_limits<double>::epsilon () || s >= std::numeric_limits<double>::epsilon ());
		double inv = 1.0 / s;
		return Color (c.r * inv, c.g * inv, c.b * inv);
	}

	inline Color operator += (const Color &c) {
		r += c.r, g += c.g, b += c.b;
		return *this;
	}
	inline Color operator -= (const Color &c) {
		r -= c.r, g -= c.g, b -= c.b;
		return *this;
	}
	inline Color operator *= (const Color &c) {
		r *= c.r, g *= c.g, b *= c.b;
		return *this;
	}
	inline Color operator /= (const Color &c) {
		r /= c.r, g /= c.g, b /= c.b;
		return *this;
	}
	
	inline Color operator *= (double s) {
		r *= s, g *= s, b *= s;
		return *this;
	}
	inline Color operator /= (double s) {
		assert (s <= -std::numeric_limits<double>::epsilon () || s >= std::numeric_limits<double>::epsilon ());
		double inv = 1.0 / s;
		r *= inv, g *= inv, b *= inv;
		return *this;
	}

	inline double operator [] (size_t index) const {
		assert (index < 3);
		return *(v + index);
	}
	inline double &operator [] (size_t index) {
		assert (index < 3);
		return *(v + index);
	}

	inline Color BlendLerp (const Color &c, double alpha) const {
		return Color (r + alpha * (c.r - r),
				g + alpha * (c.g - g),
				b + alpha * (c.b - b),
				a + alpha * (c.a - a));
	}
	inline Color Blend (const Color &c, double alpha) const {
		return Color ((c.r * alpha) + (r * (1.0 - alpha)),
				(c.g * alpha) + (g * (1.0 - alpha)),
				(c.b * alpha) + (b * (1.0 - alpha)),
				(c.a * alpha) + (a * (1.0 - alpha)));
	}

	inline Color Clamp () {
		Clamp (r);
		Clamp (g);
		Clamp (b);
		Clamp (a);
		return *this;
	}
	
	inline unsigned int To32bit () const {
		int _a = (int) (a * 256.0);
		int _r = (int) (r * 256.0);
		int _g = (int) (g * 256.0);
		int _b = (int) (b * 256.0);
		_a = (_a > 255) ? 255 : _a;
		_r = (_r > 255) ? 255 : _r;
		_g = (_g > 255) ? 255 : _g;
		_b = (_b > 255) ? 255 : _b;
		return (_a << 24) | (_r << 16) | (_g << 8) | (_b);
	}

	inline friend std::ostream &operator << (std::ostream &out, const Color &c) {
		return out << "(" << c.a << ", " << c.r << ", " << c.g << ", " << c.b << ")";
	}

}; /* class Color */

/*---------------------------------------------------------------------------*/
#endif /* __COLOR_H_ */
