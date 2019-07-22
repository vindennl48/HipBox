#ifndef DB_H
#define DB_H

#include <math.h>

inline double db2lin(double db) {
	if (db <= -99.0f) return 0.0f;
	else {
		return powf(10.0f, db * 0.05f);
	}
}

/* This converts range 0-100 to */
inline double slider2db(double slider) {
  if (slider < 1) return -99.0f;
  else {
    return ((log(slider-0.03f) / log(1.1f)) - 41.0f) * 1.25f;
  }
}

inline double slider2lin(double slider) {
  return db2lin( slider2db(slider) );
}

static inline double
lin2db( double lin )
{
	if (lin == 0.0f) return -99.0f;
	else return (20.0f * log10f(lin));
}

#endif
