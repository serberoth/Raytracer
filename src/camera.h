#ifndef __CAMERA_H_
#define __CAMERA_H_

#pragma once
/*---------------------------------------------------------------------------*/

class Camera : public gc_cleanup {
private:
	size_t screen_width;
	size_t screen_height;
	Vector origin;

	double left;
	double top;
	double right;
	double bottom;

public:
	inline Camera (const size_t width, const size_t height, const Vector &_origin, const double &_left, const double &_top, const double &_right, const double &_bottom) : screen_width (width), screen_height (height), origin (_origin), left (_left), top (_top), right (_right), bottom (_bottom) { }
	virtual ~Camera () { }

	inline size_t ScreenWidth () const {
		return screen_width;
	}

	inline size_t ScreenHeight () const {
		return screen_height;
	}

	inline Vector Origin () const {
		return origin;
	}
	inline Vector &Origin () {
		return origin;
	}

	inline double Left () const {
		return left;
	}

	inline double Top () const {
		return top;
	}

	inline double Right () const {
		return right;
	}

	inline double Bottom () const {
		return bottom;
	}

	inline double AspectRatio () const {
		return (double) screen_width / (double) screen_height;
	}

	inline double Width () const {
		return right - left;
	}

	inline double Height () const {
		return bottom - top;
	}

	inline double DeltaX () const {
		return Width () / (double) screen_width;
	}

	inline double  DeltaY () const {
		return Height () / (double) screen_height;
	}

}; /* class Camera */

/*---------------------------------------------------------------------------*/
#endif /* __CAMERA_H_ */
