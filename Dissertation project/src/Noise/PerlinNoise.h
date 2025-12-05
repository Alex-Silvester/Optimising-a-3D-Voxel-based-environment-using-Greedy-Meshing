// [header]
// A simple implementation of Perlin and Improved Perlin Noise
// [/header]
// [compile]
// c++ -o perlinnoise -O3 -Wall perlinnoise.cpp
// [/compile]
// [ignore]
// Copyright (C) 2016  www.scratchapixel.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// [/ignore]

#define _USE_MATH_DEFINES
#include <cmath>
#include <random>
#include <functional>

#include <glm/vec3.hpp>

#define PI 3.14159265358979323846f

namespace Noise
{ 
  template<typename T = float>
  inline T lerp(const T& lo, const T& hi, const T& t)
  {
    return lo * (1 - t) + hi * t;
  }
  
  inline
  float smoothstep(const float& t)
  {
    return t * t * (3 - 2 * t);
  }
  
  inline
  float quintic(const float& t)
  {
    return t * t * t * (t * (t * 6 - 15) + 10);
  }
  
  inline
  float smoothstepDeriv(const float& t)
  {
    return t * (6 - 6 * t);
  }
  
  inline
  float quinticDeriv(const float& t)
  {
    return 30 * t * t * (t * (t - 2) + 1);
  }
  
  //singleton perlin noise generator
  class PerlinNoise
  {
  public:
  
    PerlinNoise(const PerlinNoise& obj) = delete;
  
    virtual ~PerlinNoise() 
    {
      delete ptr;
    }
  
    //[comment]
    // Improved Noise implementation (2002)
    // This version compute the derivative of the noise function as well
    //[/comment]
    float eval(const glm::vec3& p, glm::vec3& derivs) const
    {
      int xi0 = ((int)std::floor(p.x)) & tableSizeMask;
      int yi0 = ((int)std::floor(p.y)) & tableSizeMask;
      int zi0 = ((int)std::floor(p.z)) & tableSizeMask;

      int xi1 = (xi0 + 1) & tableSizeMask;
      int yi1 = (yi0 + 1) & tableSizeMask;
      int zi1 = (zi0 + 1) & tableSizeMask;

      float tx = p.x - ((int)std::floor(p.x));
      float ty = p.y - ((int)std::floor(p.y));
      float tz = p.z - ((int)std::floor(p.z));

      float u = quintic(tx);
      float v = quintic(ty);
      float w = quintic(tz);

      // generate vectors going from the grid points to p
      float x0 = tx, x1 = tx - 1;
      float y0 = ty, y1 = ty - 1;
      float z0 = tz, z1 = tz - 1;

      float a = gradientDotV(hash(xi0, yi0, zi0), x0, y0, z0);
      float b = gradientDotV(hash(xi1, yi0, zi0), x1, y0, z0);
      float c = gradientDotV(hash(xi0, yi1, zi0), x0, y1, z0);
      float d = gradientDotV(hash(xi1, yi1, zi0), x1, y1, z0);
      float e = gradientDotV(hash(xi0, yi0, zi1), x0, y0, z1);
      float f = gradientDotV(hash(xi1, yi0, zi1), x1, y0, z1);
      float g = gradientDotV(hash(xi0, yi1, zi1), x0, y1, z1);
      float h = gradientDotV(hash(xi1, yi1, zi1), x1, y1, z1);

      float du = quinticDeriv(tx);
      float dv = quinticDeriv(ty);
      float dw = quinticDeriv(tz);

      float k0 = a;
      float k1 = (b - a);
      float k2 = (c - a);
      float k3 = (e - a);
      float k4 = (a + d - b - c);
      float k5 = (a + f - b - e);
      float k6 = (a + g - c - e);
      float k7 = (b + c + e + h - a - d - f - g);

      derivs.x = du * (k1 + k4 * v + k5 * w + k7 * v * w);
      derivs.y = dv * (k2 + k4 * u + k6 * w + k7 * v * w);
      derivs.z = dw * (k3 + k5 * u + k6 * v + k7 * v * w);

      return k0 + k1 * u + k2 * v + k3 * w + k4 * u * v + k5 * u * w + k6 * v * w + k7 * u * v * w;
    }
  
    //[comment]
    // classic/original Perlin noise implementation (1985)
    //[/comment]
    float eval(const glm::vec3& p) const
    {
      int xi0 = (int)p.x & tableSizeMask;
      int yi0 = (int)p.y & tableSizeMask;
      int zi0 = (int)p.z & tableSizeMask;

      int xi1 = (xi0 + 1) & tableSizeMask;
      int yi1 = (yi0 + 1) & tableSizeMask;
      int zi1 = (zi0 + 1) & tableSizeMask;

      float tx = p.x - (int)(p.x);
      float ty = p.y - (int)(p.y);
      float tz = p.z - (int)(p.z);

      float u = smoothstep(tx);
      float v = smoothstep(ty);
      float w = smoothstep(tz);

      // gradients at the corner of the cell
      const glm::vec3& c000 = gradients[hash(xi0, yi0, zi0)];
      const glm::vec3& c100 = gradients[hash(xi1, yi0, zi0)];
      const glm::vec3& c010 = gradients[hash(xi0, yi1, zi0)];
      const glm::vec3& c110 = gradients[hash(xi1, yi1, zi0)];

      const glm::vec3& c001 = gradients[hash(xi0, yi0, zi1)];
      const glm::vec3& c101 = gradients[hash(xi1, yi0, zi1)];
      const glm::vec3& c011 = gradients[hash(xi0, yi1, zi1)];
      const glm::vec3& c111 = gradients[hash(xi1, yi1, zi1)];

      // generate vectors going from the grid points to p
      float x0 = tx, x1 = tx - 1;
      float y0 = ty, y1 = ty - 1;
      float z0 = tz, z1 = tz - 1;

      glm::vec3 p000 = glm::vec3(x0, y0, z0);
      glm::vec3 p100 = glm::vec3(x1, y0, z0);
      glm::vec3 p010 = glm::vec3(x0, y1, z0);
      glm::vec3 p110 = glm::vec3(x1, y1, z0);

      glm::vec3 p001 = glm::vec3(x0, y0, z1);
      glm::vec3 p101 = glm::vec3(x1, y0, z1);
      glm::vec3 p011 = glm::vec3(x0, y1, z1);
      glm::vec3 p111 = glm::vec3(x1, y1, z1);

      // linear interpolation
      float a = lerp(glm::dot(c000, p000), glm::dot(c100, p100), u);
      float b = lerp(glm::dot(c010, p010), glm::dot(c110, p110), u);
      float c = lerp(glm::dot(c001, p001), glm::dot(c101, p101), u);
      float d = lerp(glm::dot(c011, p011), glm::dot(c111, p111), u);

      float e = lerp(a, b, v);
      float f = lerp(c, d, v);

      return lerp(e, f, w); // g
    }
  
  private:
  
    PerlinNoise(const unsigned& seed)
    {
      std::mt19937 generator(seed);
      std::uniform_real_distribution<float> distribution;
      auto dice = std::bind(distribution, generator);
      for (unsigned i = 0; i < tableSize; ++i) 
      {
        float theta = acos(2 * dice() - 1);
        float phi = 2 * dice() * PI;
    
        float x = cos(phi) * sin(theta);
        float y = sin(phi) * sin(theta);
        float z = cos(theta);
    
        gradients[i] = glm::vec3(x, y, z);
    
        permutationTable[i] = i;
      }
    
      std::uniform_int_distribution<unsigned> distributionInt;
      auto diceInt = std::bind(distributionInt, generator);
      // create permutation table
      for (unsigned i = 0; i < tableSize; ++i)
      {
        std::swap(permutationTable[i], permutationTable[diceInt() & tableSizeMask]);
      }
    
      // extend the permutation table in the index range [256:512]
      for (unsigned i = 0; i < tableSize; ++i)
      {
        permutationTable[tableSize + i] = permutationTable[i];
      }
    }
  
    static PerlinNoise* ptr;
  
    inline uint8_t hash(const int& x, const int& y, const int& z) const
    {
      return permutationTable[permutationTable[permutationTable[x] + y] + z];
    }
  
    //[comment]
    // Compute dot product between vector from cell corners to P with predefined gradient directions
    //
    //    perm: a value between 0 and 255
    //
    //    float x, float y, float z: coordinates of vector from cell corner to shaded point
    //[/comment]
    float gradientDotV(uint8_t perm, float x, float y, float z) const
    {
      switch (perm & 15) 
      {
        case  0: return  x + y; // (1,1,0)
        case  1: return -x + y; // (-1,1,0)
        case  2: return  x - y; // (1,-1,0)
        case  3: return -x - y; // (-1,-1,0)
        case  4: return  x + z; // (1,0,1)
        case  5: return -x + z; // (-1,0,1)
        case  6: return  x - z; // (1,0,-1)
        case  7: return -x - z; // (-1,0,-1)
        case  8: return  y + z; // (0,1,1),
        case  9: return -y + z; // (0,-1,1),
        case 10: return  y - z; // (0,1,-1),
        case 11: return -y - z; // (0,-1,-1)
        case 12: return  y + x; // (1,1,0)
        case 13: return -x + y; // (-1,1,0)
        case 14: return -y + z; // (0,-1,1)
        case 15: return -y - z; // (0,-1,-1)
      }
    }
  
    static const unsigned tableSize = 256u;
    static const unsigned tableSizeMask = tableSize - 1u;
    glm::vec3 gradients[tableSize];
    unsigned permutationTable[tableSize * 2];
  
  public:
    static PerlinNoise& noise(const unsigned& seed = 2016)
    {
      if (ptr == NULL)
      {
        ptr = new PerlinNoise(seed);
      }
      return *ptr;
    }
  
  };
  PerlinNoise* PerlinNoise::ptr = NULL;
}