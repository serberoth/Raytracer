#ifndef __CANVAS_H__
#define __CANVAS_H__

#pragma once
/*---------------------------------------------------------------------------*/

class Canvas : public gc_cleanup {
private:
	Color *buffer;
	size_t width;
	size_t height;
	size_t size;

public:
	Canvas (size_t w, size_t h) : buffer (NULL), width (w), height (h), size (w * h) {
		buffer =  new (GC) Color[size + 1];
	}
	Canvas (Color *_buffer, size_t w, size_t h) : buffer (_buffer), width (w), height (h), size (w * h) { }
	~Canvas () {
		if (buffer != NULL) {
			delete buffer;
			buffer = NULL;
		}
		width = height = size = 0;
	}

	Color operator[] (size_t index) const {
		assert (index < size);
		return buffer[index];
	}
	Color &operator[] (size_t index) {
		assert (index < size);
		return buffer[index];
	}

	inline size_t Width () const {
		return width;
	}
	inline size_t Height () const {
		return height;
	}

	inline Color Get (size_t index) const {
		assert (index < size);
		return buffer[index];
	}
	inline Color Get (size_t x, size_t y) const {
		register size_t index = x + y * width;
		assert (index < size);
		return buffer[index];
	}
	inline void Set (size_t index, const Color &c) {
		assert (index < size);
		buffer[index] = c;
	}
	inline void Set (size_t x, size_t y, const Color &c) {
		register size_t index = x + y * width;
		assert (index < size);
		buffer[index] = c;
	}

};

/*---------------------------------------------------------------------------*/
#endif /* __CANVAS_H__ */
