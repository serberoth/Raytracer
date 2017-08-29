
#include "common.h"

/*---------------------------------------------------------------------------*/

Texture *GenerateCheckerboard (const Color &color1, const Color &color2) {
	Canvas *canvas = new Canvas (512, 512);
	int index = 0;
	for (size_t y = 0; y < 512; ++y) {
		for (size_t x = 0; x < 512; ++x) {
			bool c = (((x & 0x80) == 0) ^ ((y & 0x80) == 0));
			canvas->Set(index, (c ? color1 : color2));
			++index;
		}
	}
	return new Texture (canvas);
}

/*---------------------------------------------------------------------------*/

void WriteCanvasToTgaImage (const char *name, const Canvas &canvas) {
	unsigned int *data = new (GC) unsigned int[canvas.Width () * canvas.Height ()];
	size_t index = 0;
	for (size_t j = 0; j < canvas.Height (); ++j) {
		for (size_t i = 0; i < canvas.Width (); ++i) {
			data[index] = canvas[index].To32bit ();
			++index;
		}
	}

	WriteTgaImage (name, data, canvas.Width (), canvas.Height ());
	// delete[] data;
}

/*---------------------------------------------------------------------------*/

void Write32bitTgaImage (const char *filename, unsigned int *buffer, size_t width, size_t height) {
	size_t length = width * height * sizeof (unsigned int);
	unsigned char *data = new (GC) unsigned char[length + 18];

	data[2] = 2;
	data[12] = (unsigned char) (width & 0xff);
	data[13] = (unsigned char) (width >> 8);
	data[14] = (unsigned char) (height & 0xff);
	data[15] = (unsigned char) (height >> 8);
	data[16] = 32; // 24;
	data[17] = 0x20;

#ifdef APPLE_PPC
	for (size_t index = 0; index < width * height; ++index) {
		unsigned int value = buffer[index];
		buffer[index] = (value & 0xff) << 24 | ((value >> 8) & 0xff) << 16 | ((value >> 16) & 0xff) << 8 | ((value >> 24) & 0xff);
	}
#endif

	memcpy (data + 18, buffer, sizeof (unsigned char) * length);

	FILE *file = fopen (filename, "wb");
	assert (file != NULL);
	fwrite (data, sizeof (unsigned char), length + 18, file);
	fclose (file);
}

/*---------------------------------------------------------------------------*/

void Write24bitTgaImage (const char *filename, unsigned int *buffer, size_t width, size_t height) {
        assert (filename != NULL);
        assert (buffer != NULL);

        size_t length = width * height * 3; // sizeof (unsigned int);
        unsigned char *data = new unsigned char[length + 18];
        assert (data != NULL);
        memset (data, 0, length + 18);

        data[2] = 2;
        data[12] = (unsigned char) (width & 0xff);
        data[13] = (unsigned char) (width >> 8);
        data[14] = (unsigned char) (height & 0xff);
        data[15] = (unsigned char) (height >> 8);
        data[16] = 24; // 32
        data[17] = 0x20;

#ifdef __APPLE_PPC
        for (size_t index = 0; index < width * height; ++index) {
                unsigned int value = buffer[index];
                buffer[index] = (value & 0xff) << 24 | ((value >> 8) & 0xff) << 16 | ((value >> 16) & 0xff) << 8 | ((value >> 24) & 0xff);
        }
#endif

        // memcpy (data + 18, buffer, sizeof (unsigned char) * length);
        unsigned char *work = data + 18;
        for (size_t index = 0; index < width * height; ++index) {
                unsigned int value  = buffer[index];
                *work++ = (unsigned char) (value & 0xff);
                *work++ = (unsigned char) (value >> 8);
                *work++ = (unsigned char) (value >> 16);
        }

        FILE *file = fopen (filename, "wb");
        assert (file != NULL);
        fwrite (data, sizeof (unsigned char), length + 18, file);
        fclose (file);
}

/*---------------------------------------------------------------------------*/

unsigned int *Load32bitTgaImage (const char *filename, size_t &width, size_t &height) {
	assert (filename != NULL);
	
	FILE *file = fopen (filename, "rb");
	assert (file != NULL);
	
	fseek (file, 0, SEEK_END);
	size_t length = ftell (file);
	fseek (file, 0, SEEK_SET);
	unsigned char *data = new (GC) unsigned char[length + 1];
	assert (data != NULL);
	
	fread (data, length, 1, file);
	fclose (file);
	
	assert (data[2] == 2 && data[16] == 32);
	width = data[13] << 8 | data[12];
	height = data[15] << 8 | data[14];
	
	unsigned int *buffer = new unsigned int [width * height];
	assert (buffer != NULL);
	memcpy (buffer, data + 18, sizeof (unsigned char) * width * height * sizeof (unsigned int));
	
#ifdef APPLE_PPC
	for (size_t index = 0; index < width * height; ++index) {
		unsigned int value = buffer[index];
		buffer[index] = (value & 0xff) << 24 | ((value >> 8) & 0xff) << 16 | ((value >> 16) & 0xff) << 8 | ((value >> 24) & 0xff);
	}
#endif

	return buffer;
}

/*---------------------------------------------------------------------------*/

void (*WriteTgaImage) (const char *, unsigned int *, size_t, size_t) = Write24bitTgaImage;
unsigned int *(*LoadTgaImage) (const char *, size_t&, size_t&) = Load32bitTgaImage;
