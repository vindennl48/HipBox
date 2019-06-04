#ifndef DB_H
#define DB_H

#include <math.h>

static inline double
db2lin( double db )
{
	if (db <= -90.0f) return 0.0f;
	else {
		return powf(10.0f, db * 0.05f);
	}
}

static inline double
lin2db( double lin )
{
	if (lin == 0.0f) return -90.0f;
	else return (20.0f * log10f(lin));
}

#endif
