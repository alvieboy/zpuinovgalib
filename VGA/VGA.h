#ifndef __VGA_H__
#define __VGA_H__

#if defined(ZPU)

#include "zpuino.h"
#define VGABASE IO_SLOT(9)
#define CHARRAMBASE IO_SLOT(10)
#define VGAPTR REGISTER(VGABASE,0);
#define CHARRAMPTR REGISTER(CHARRAMBASE,0);

typedef volatile unsigned int *vgaptr_t;
typedef volatile unsigned int *charptr_t;

static inline vgaptr_t getVgaMem() {
	return &VGAPTR;
}

static inline vgaptr_t getCharRam() {
	return &CHARRAMPTR;
}

#elif defined(__linux__)

#include "vga-linux.h"
#include <unistd.h>

typedef MemProxy vgaptr_t;
typedef MemProxy charptr_t;
inline vgaptr_t getVgaMem() {
	return MemProxy(vga);
}

inline vgaptr_t getCharRam() {
	return MemProxy(charram);
}

#else
#error No support
#endif

const unsigned int VGA_HSIZE = 160;
const unsigned int VGA_VSIZE = 120;

class VGA_class {
public:
	VGA_class() {};

	typedef unsigned char pixel_t;

	inline unsigned int getHSize() const { return VGA_HSIZE; }
	inline unsigned int getVSize() const { return VGA_VSIZE; }

	inline vgaptr_t getBasePointer() const {
		return getVgaMem();
	}
    inline vgaptr_t getBasePointer(unsigned x, unsigned y) const {
		vgaptr_t p = getBasePointer();
		p += x;
		p += (y * getHSize());
		return p;
	}

	inline vgaptr_t getCharacterBasePointer() const {
		return getCharRam();
	}

	void readArea(int x, int y, int width, int height, pixel_t *dest);
	void writeArea(int x, int y, int width, int height, pixel_t *source);

	void setColor(pixel_t color)
	{
		fg=color;
	}

	void setBackgroundColor(pixel_t color)
	{
        bg=color;
	}

	inline void putPixel(int x, int y)
	{
		*(getBasePointer(x,y)) = fg;
	}

	pixel_t getPixel(int x, int y)
	{
		return *(getBasePointer(x,y));
	}

	inline void putPixel(int x, int y, pixel_t color)
	{
		*(getBasePointer(x,y)) = color;
	}

	inline void setColor(unsigned r, unsigned g, unsigned b)
	{
		setColor(r<<5 | g<<3 | b);
	}

	void drawRect(unsigned x, unsigned y, unsigned width, unsigned height);

	void clearArea(unsigned x, unsigned y, unsigned width, unsigned height);

	void printchar(unsigned int x, unsigned int y, unsigned char c, bool trans=false);
	void printtext(unsigned x, unsigned y, const char *text, bool trans=false);

	void clear();

protected:
	pixel_t fg,bg;
};

const VGA_class::pixel_t RED = 0xE0;
const VGA_class::pixel_t GREEN = 0x1C;
const VGA_class::pixel_t BLUE = 0x03;

extern VGA_class VGA;

#endif
