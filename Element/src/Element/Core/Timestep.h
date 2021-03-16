#pragma once

namespace Element {
	class Timestep {
	private:
		float time;
		
	public:
		Timestep(float time = 0.0f)
			: time(time) {}

	public:
		/// <summary>
		/// Returns time in seconds.
		/// </summary>
		/// <returns></returns>
		inline float GetSeconds() const {
			return time;
		}

		/// <summary>
		/// Returns time in milliseconds.
		/// </summary>
		/// <returns></returns>
		inline float GetMilliseconds() const {
			return time * 1000;
		}

		inline operator float() const {
			return time;
		}
	};
}
