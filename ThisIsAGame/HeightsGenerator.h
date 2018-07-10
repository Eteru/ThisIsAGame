#pragma once

#include <map>
#include <utility>

class HeightsGenerator
{
public:
	using NoiseGeneratorMap = std::map<std::pair<int, int>, float>;

	HeightsGenerator();
	~HeightsGenerator();

	float GenerateHeight(int x, int z);

	float GetNoise(int x, int z);
	float GetSmoothNoise(int x, int z);
	float GetInterpolatedNoise(float x, float z);


	void Clean();

private:
	const int OCTAVES = 3;
	const float AMPLITUDE = 10.f;
	const float ROUGHNESS = 0.3f;
	NoiseGeneratorMap m_noise_map;

	float GenerateRandomFloat() const;
	float InterpolationCosinus(float a, float b, float blend) const;
};

