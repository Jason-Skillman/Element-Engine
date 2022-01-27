#include "pch.h"
#include "GUID.h"

#include <random>

namespace Element {

	static std::random_device randomDevice;
	static std::mt19937_64 engine(randomDevice());
	static std::uniform_int_distribution<uint64_t> uniformDistribution;

	GUID::GUID() 
		: guid(uniformDistribution(engine)) {}

	GUID::GUID(uint64_t guid)
		: guid(guid) {}
}
