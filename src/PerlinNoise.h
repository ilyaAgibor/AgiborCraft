#pragma once
#include <vector>

class PerlinNoise {
public:
	static double smoothstep(double edge0, double edge1, double x);
	static double fade(double t);
	static double lerp(double t, double a, double b);
	static double grad(int hash, double x, double y, double z);
	PerlinNoise(unsigned int seed);
	double noise(double x, double y, double z);
	int noise_int(double x, double y, double z, int max_int);
private:
	std::vector<int> p;
};
