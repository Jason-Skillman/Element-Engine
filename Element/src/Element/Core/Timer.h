#pragma once

#include <chrono>

namespace Element {

	class Timer {
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> start;

	public:
		Timer() {
			Reset();
		}

		void Timer::Reset() {
			start = std::chrono::high_resolution_clock::now();
		}

		float Timer::Elapsed() {
			return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count() * 0.001f * 0.001f * 0.001f;
		}

		float Timer::ElapsedMillis() {
			return Elapsed() * 1000.0f;
		}
	};
}
