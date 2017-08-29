/*-----------------------------------------------------------------------------
 The Mersenne Twister by Matsumoto and Nishimura <matumoto@math.keio.ac.jp>.
 It sets new standards for the period, quality and speed of random number
 generators. The incredible period is 2^19937 - 1, a number with about 6000
 digits; the 32-bit random numbers exhibit best possible equidistribution
 properties in dimensions up to 623; and it's fast, very fast.
 -----------------------------------------------------------------------------*/

#include "gc.h"
#include "random.h"

/*---------------------------------------------------------------------------*/

#define LOWER_MASK				(rand_t) 0x7fffffff
#define TEMPERING_MASK_B		(rand_t) 0x9d2c5680
#define TEMPERING_MASK_C		(rand_t) 0xefc60000
#define TEMPERING_SHIFT_L(y)	(y >> 18)
#define TEMPERING_SHIFT_S(y)	(y << 7)
#define TEMPERING_SHIFT_T(y)	(y << 15)
#define TEMPERING_SHIFT_U(y)	(y >> 11)
#define MATRIX_A				(rand_t) 0x9908b0df
#define UPPER_MASK				(rand_t) 0x80000000

/*---------------------------------------------------------------------------*/

const rand_t Random::mag01[2] = { 0x0, MATRIX_A };

/*---------------------------------------------------------------------------*/

rand_t Random::RandomInt (void) {
	int kk;
	rand_t y;

	if (k == N) {
		for (kk = 0; kk < N - M; kk++) {
			y = (ptgfsr[kk] & UPPER_MASK) | (ptgfsr[kk + 1] & LOWER_MASK);
			ptgfsr[kk] = ptgfsr[kk + M] ^ (y >> 1) ^ mag01[y & 0x1];
		}
		for (; kk < N - 1; kk++) {
			y = (ptgfsr[kk] & UPPER_MASK) | (ptgfsr[kk + 1] & LOWER_MASK);
			ptgfsr[kk] = ptgfsr[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 0x1];
		}
		y = (ptgfsr[N - 1] & UPPER_MASK) | (ptgfsr[0] & LOWER_MASK);
		ptgfsr[N - 1] = ptgfsr[M - 1] ^ (y >> 1) ^ mag01[y & 0x1];
		k = 0;
	}
	y = ptgfsr[k++];
	y ^= TEMPERING_SHIFT_U (y);
	y ^= TEMPERING_SHIFT_S (y) & TEMPERING_MASK_B;
	y ^= TEMPERING_SHIFT_T (y) & TEMPERING_MASK_C;
	return y ^= TEMPERING_SHIFT_L (y);
}
