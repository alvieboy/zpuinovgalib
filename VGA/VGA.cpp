#include "VGA.h"

void VGA_class::printchar(unsigned int x, unsigned int y, unsigned char c, bool trans)
{
	vgaptr_t vmem = getBasePointer(x,y);
	charptr_t vchar = getCharacterBasePointer();

	int i,j;
	unsigned int v;

	vchar += c << 3;

	for (i=0; i<8;i++) {
		v = *vchar;
		for (j=0; j<8;j++) {

			int rx,ry;
			rx = x + j;
			ry = (y + i);

			/* Check bounds */
			if (rx>=0 && rx<(int)getHSize() && ry>=0 &&  ry<(int)getVSize()) {
				rx = rx + ry * (int)getHSize();


				if (v&0x80) {
					vmem[rx] = fg;
				} else {
					if (!trans)
						vmem[rx] = bg;
				}
				v<<=1;
			}
		}
		vchar++;
	}
}

void VGA_class::drawRect(unsigned x, unsigned y, unsigned width, unsigned height)
{
	vgaptr_t vmem = getBasePointer(x,y);
	unsigned w,h;

	for (h=0;h<height;h++) {
		for (w=0;w<width;w++) {
			*vmem++=fg;
		}
		vmem+=getHSize()-width;
	}
}

void VGA_class::printtext(unsigned x, unsigned y, const char *text, bool trans)
{
	while (*text) {
		printchar(x,y,*text,trans);
		text++;
		x+=8;
	}
}

void VGA_class::readArea(int x, int y, int width, int height, pixel_t *dest)
{
	vgaptr_t vmem = getBasePointer(x,y);
	int w;

	while (height--) {
		for (w=0;w<width;w++) {
			*dest++ = *vmem++;
		}
		vmem+=getHSize()-width;
	}
}

void VGA_class::writeArea(int x, int y, int width, int height, pixel_t *source)
{
	vgaptr_t vmem = getBasePointer(x,y);
	int w;

	while (height--) {
		for (w=0;w<width;w++) {
			*vmem++ = *source++;
		}
		vmem+=getHSize()-width;
	}
}

void VGA_class::clear()
{
	vgaptr_t vmem = getBasePointer();
	unsigned count = getHSize() * getVSize();
	while (--count) {
		*vmem++=0;
	}
}

VGA_class VGA;
