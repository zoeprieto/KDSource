#include <stdio.h>
#include <stdint.h>
#include <MersenneTwister.h>

MersenneTwister64* MT64_create(uint64_t *seed){
    MersenneTwister64* MT = (MersenneTwister64*)malloc(sizeof(MersenneTwister64));
    initializeMersenneTwister64(MT,seed);
}


void initializeMersenneTwister64(MersenneTwister64 *MT, uint64_t *seed) {
    MT->state[0] = seed;
    for (int i = 1; i < MT_N; ++i) {
        MT->state[i] = (6364136223846793005ULL * (MT->state[i - 1] ^ (MT->state[i - 1] >> 62)) + i);
    }
    MT->index = MT_N;
}

uint64_t MT64_rand(MersenneTwister64 *MT) {
    if (MT->index == MT_N) {
        int i;
        for (i = 0; i < MT_N - MT_M; ++i) {
            uint64_t x = (MT->state[i] & MT_UPPER_MASK) | (MT->state[i + 1] & MT_LOWER_MASK);
            MT->state[i] = MT->state[i + MT_M] ^ (x >> 1) ^ ((x & 1) ? MT_MATRIX_A : 0);
        }
        for (; i < MT_N - 1; ++i) {
            uint64_t x = (MT->state[i] & MT_UPPER_MASK) | (MT->state[i + 1] & MT_LOWER_MASK);
            MT->state[i] = MT->state[i + (MT_M - MT_N)] ^ (x >> 1) ^ ((x & 1) ? MT_MATRIX_A : 0);
        }
        uint64_t x = (MT->state[MT_N - 1] & MT_UPPER_MASK) | (MT->state[0] & MT_LOWER_MASK);
        MT->state[MT_N - 1] = MT->state[MT_M - 1] ^ (x >> 1) ^ ((x & 1) ? MT_MATRIX_A : 0);
        MT->index = 0;
    }

    uint64_t y = MT->state[MT->index++];
    y ^= (y >> 29) & 0x5555555555555555ULL;
    y ^= (y << 17) & 0x71D67FFFEDA60000ULL;
    y ^= (y << 37) & 0xFFF7EEE000000000ULL;
    y ^= (y >> 43);

    return y;
}

