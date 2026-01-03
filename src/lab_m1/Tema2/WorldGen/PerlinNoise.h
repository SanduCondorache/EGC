#pragma once
#include <vector>

class PerlinNoise {
  private:
    std::vector<int> p;

    double fade(double t);

    double grad(int hash, double x, double y, double z);

    double lerp(double t, double a, double b);

  public:
    PerlinNoise(unsigned int seed = 237);
    double noise(double x, double y, double z);
};
