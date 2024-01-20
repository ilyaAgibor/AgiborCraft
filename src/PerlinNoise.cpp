#include "PerlinNoise.h"
#include <cmath>
#include <random>
#include <algorithm>
#include <numeric>


double clamp(double v, double lo, double hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

double PerlinNoise::smoothstep(double edge0, double edge1, double x) {
	// Scale, bias and saturate x to 0..1 range
	x = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
	// Evaluate polynomial
	return x * x * (3 - 2 * x);
}

double PerlinNoise::fade(double t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

double PerlinNoise::lerp(double a, double b, double t) {
    return a + t * (b - a);
}
double PerlinNoise::grad(int hash, double x, double y, double z) {
    int h = hash & 15;
    // Convert lower 4 bits of hash into 12 gradient directions
    double u = h < 8 ? x : y,
        v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    // Calculate the dot product of the gradient vector and the input vector
    double dot = ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
    // Map the dot product from the range [-1, 1] to the range [0, 1]
    return (dot + 1.0) / 2.0;
}




PerlinNoise::PerlinNoise(unsigned int seed) {
    // Initialize permutation vector with values from 0 to 255
    p.resize(256);
    std::iota(p.begin(), p.end(), 0);

    // Shuffle the permutation vector using the given seed
    std::default_random_engine engine(seed);
    std::shuffle(p.begin(), p.end(), engine);

    // Duplicate the permutation vector to avoid buffer overflow when indexing
    p.insert(p.end(), p.begin(), p.end());
}

double PerlinNoise::noise(double x, double y, double z) {
    int X = (int)std::floor(x) & 255;
    int Y = (int)std::floor(y) & 255;
    int Z = (int)std::floor(z) & 255;

    x -= std::floor(x);
    y -= std::floor(y);
    z -= std::floor(z);

    double u = fade(x);
    double v = fade(y);
    double w = fade(z);

    double A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z;
    double B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;

    double res = lerp(w, lerp(v, lerp(u, grad(p[AA], x, y, z),
        grad(p[BA], x - 1, y, z)),
        lerp(u, grad(p[AB], x, y - 1, z),
            grad(p[BB], x - 1, y - 1, z))),
        lerp(v, lerp(u, grad(p[AA + 1], x, y, z - 1),
            grad(p[BA + 1], x - 1, y, z - 1)),
            lerp(u, grad(p[AB + 1], x, y - 1, z - 1),
                grad(p[BB + 1], x - 1, y - 1, z - 1))));

    return (res + 1.0) / 2.0;
}


int PerlinNoise::noise_int(double x, double y, double z, int max_int) {
	double noise_val = noise(x, y, z);
    int z_val = (int)(noise_val * max_int);
	return z_val;
}

