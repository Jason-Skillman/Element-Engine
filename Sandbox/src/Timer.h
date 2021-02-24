#pragma once

#if 0
#include <chrono>
#include <iostream>
#include <functional>

struct ProfileResult {
	const char* name;
	float time;
};

#define PROFILE_SCOPE(name, profileResults) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { profileResults.push_back(profileResult); });

template<typename Fn>
class Timer {
private:
	const char* name;
	Fn func;
	std::chrono::time_point<std::chrono::steady_clock> startTimepoint;
	bool isStopped;
	
public:
	Timer(const char* name, Fn&& func)
		: name(name), func(func), isStopped(false) {

		startTimepoint = std::chrono::high_resolution_clock::now();
	}
	~Timer() {
		if(!isStopped)
			Stop();
	}

public:
	void Stop() {
		auto endTimepoint = std::chrono::high_resolution_clock::now();

		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(startTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		isStopped = true;

		float duration = (end - start) * 0.001f;
		func({ name, duration });
	}
};
#endif
