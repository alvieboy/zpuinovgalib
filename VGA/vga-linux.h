#ifdef __linux__

#include <SDL.h>

class MemoryAccessor
{
public:
	virtual void mwrite(int offset, int value)=0;
	virtual int mread(int offset)const =0;
};

class MemProxy
{
public:
	MemProxy(MemoryAccessor&s): x(0),me(s) {};
	MemProxy(MemoryAccessor&s, int rx): x(rx),me(s) {};

	inline operator int () const {
		return me.mread(x);
	}

	MemProxy &operator+=(int v)
	{
		x+=v;
        return *this;
	}

	MemProxy operator+(int v) {
		return MemProxy(me,x+v);
	}

	MemProxy operator[](int x) {
		return MemProxy(me,x);
	}

	MemProxy operator++(int v) {
		MemProxy copy(*this);
		x++;
		return copy;
	}

	MemProxy &operator--(int v) {
		x--;
		return *this;
	}
	MemProxy &operator=(int v) {
		me.mwrite(x,v);
		return *this;
	}

	MemProxy &operator=(const MemProxy &r) {
		me.mwrite(x, (int)r);
		return *this;
	}
	MemProxy &operator*() {
		return *this;
	}
private:
	int x;
	MemoryAccessor &me;
};

class CharRam: public MemoryAccessor
{
public:
	int mread(int x) const {
		return cram[x];
	}
	void mwrite(int x,int v)
	{
	}
	static unsigned char cram[2048];
};

class SDLVga: public MemoryAccessor
{
public:

	int init();
	void shutdown();
	void setPixel(int x,int y, int value);
	int getPixel(int x,int t) const;
	void sync();

	static const int maxx = 800/5;
	static const int maxy = 600/5;

	int mread(int x) const {
		int y = x / maxx;
		x = x % maxx;
		return getPixel(x,y);
	}
	void mwrite(int x,int val)
	{
		int y = x / maxx;
		x = x % maxx;
		return setPixel(x,y,val);
	}

protected:
	SDL_Surface *screen;
};

extern SDLVga vga;
extern CharRam charram;

#endif
