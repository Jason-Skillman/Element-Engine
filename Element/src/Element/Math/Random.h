#pragma once

#include <random>

namespace Element {

	class Random {
	private:
		static std::mt19937 random;
		static std::uniform_int_distribution<std::mt19937::result_type> distribution;
		
	public:
		static void Init() {
			random.seed(std::random_device()());
		}

		/// <summary>
		/// Returns a value between 0 and 1.
		/// </summary>
		static float Float() {
			return static_cast<float>(distribution(random)) / static_cast<float>(std::numeric_limits<uint32_t>::max());
		}

		/// <summary>
		/// Returns a random number between min(inclusive) and max(inclusive).
		/// </summary>
		static float Range(int min, int max) {
			return (Float() * (max + 1 - min)) + min;
		}

		/// <summary>
		/// Returns a random number between min(exclusive) and max(exclusive).
		/// </summary>
		static float Range(float min, float max) {
			return (Float() * (max - min)) + min;
		}
	};
}
