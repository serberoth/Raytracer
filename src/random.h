#ifndef __RANDOM_H__
#define __RANDOM_H__

#pragma once
/*---------------------------------------------------------------------------*/

#ifdef ARCH_X86
typedef unsigned long rand_t;
#else
typedef unsigned int rand_t;
#endif

class Random : public gc {
private:
	enum { M = 397, N = 624, };

	static const rand_t mag01[2];

	int k;
	rand_t ptgfsr[N];

public:
	Random (rand_t seed) : k (1), ptgfsr () {
		ptgfsr[0] = seed;
		for (k = 1; k < N; k++) {
			ptgfsr[k] = (0x6c078965 * (ptgfsr[k - 1] ^ ((ptgfsr[k - 1]) >> 30)) + k) & 0xFFFFFFFF;
			// ptgfsr[k] = 69069 * ptgfsr[k - 1];
		}
		k = 1;
	}
	~Random () { }

	rand_t RandomInt ();
	inline rand_t RandomInt (int range) {
		return range ? (RandomInt () % range) : 0;
	}

	inline double RandomReal () {
		rand_t a = RandomInt () >> 5, b = RandomInt () >> 6;
		return (a * 67108864.0 + b) * (1.0 / 9007199254740992.0);
	}

};

/*---------------------------------------------------------------------------*/
#endif /* __RANDOM_H__ */
