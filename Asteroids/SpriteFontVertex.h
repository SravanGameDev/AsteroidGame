#ifndef SPRITEFONTVERTEX_H_INCLUDED
#define SPRITEFONTVERTEX_H_INCLUDED

#include <stdint.h>

struct SpriteFontVertex
{
	float x, y;
	float u, v;
	uint32_t diffuse;
};

#endif // SPRITEFONTVERTEX_H_INCLUDED
