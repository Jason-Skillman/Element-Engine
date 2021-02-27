#include "pch.h"
#include "Random.h"

namespace Hazel {
	
	std::mt19937 Random::random;
	std::uniform_int_distribution<std::mt19937::result_type> Random::distribution;
}
