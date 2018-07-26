
#include <glm/glm.hpp>

#include "HeightsGenerator.h"
#include "Constants.h"

HeightsGenerator::HeightsGenerator()
{
}

HeightsGenerator::~HeightsGenerator()
{
}

float HeightsGenerator::GenerateHeight(int x, int z)
{
	float total = 0.f;
	float d = std::pow(2.f, OCTAVES - 1);

	for (int i = 0; i < OCTAVES; ++i)
	{
		float freq = std::pow(2.f, i) / d;
		float amp = std::pow(ROUGHNESS, i) * AMPLITUDE;

		total += GetInterpolatedNoise(x * 0.25f, z * 0.25f) * amp;
	}
		
	return total;
}

float HeightsGenerator::GetNoise(int x, int z)
{
	std::pair<int, int> p = std::make_pair(x, z);
	if (m_noise_map.find(p) == m_noise_map.end())
	{
		m_noise_map[p] = GenerateRandomFloat();
	}

	return m_noise_map[p];
}

float HeightsGenerator::GetSmoothNoise(int x, int z)
{
	float corners = (GetNoise(x-1, z-1) + GetNoise(x+1, z-1) + GetNoise(x-1, z-1) + GetNoise(x+1, z+1)) / 16.f;
	float sides = (GetNoise(x-1, z) + GetNoise(x+1, z) + GetNoise(x, z-1) + GetNoise(x, z+1)) / 8.f;
	float center = GetNoise(x, z) / 4.f;

	return corners + sides + center;
}

float HeightsGenerator::GetInterpolatedNoise(float x, float z)
{
	int iX = static_cast<int>(x);
	int iZ = static_cast<int>(z);

	float fX = x - iX;
	float fZ = z - iZ;

	float v1 = GetSmoothNoise(iX, iZ);
	float v2 = GetSmoothNoise(iX + 1, iZ);
	float v3 = GetSmoothNoise(iX, iZ + 1);
	float v4 = GetSmoothNoise(iX + 1, iZ + 1);

	float i1 = InterpolationCosinus(v1, v2, fX);
	float i2 = InterpolationCosinus(v3, v4, fX);

	return InterpolationCosinus(i1, i2, fZ);
}

void HeightsGenerator::Clean()
{
	m_noise_map.clear();
}

float HeightsGenerator::GenerateRandomFloat() const
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 2.f - 1.f;
}

float HeightsGenerator::InterpolationCosinus(float a, float b, float blend) const
{
	double theta = blend * M_PI;
	float f = (1.f - std::cos(theta)) * 0.5f;

	return a * (1.f - f) + b * f;
}
