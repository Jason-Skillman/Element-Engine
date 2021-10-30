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
		float GetSeconds() const {
			return time;
		}

		/// <summary>
		/// Returns time in milliseconds.
		/// </summary>
		float GetMilliseconds() const {
			return time * 1000;
		}

		operator float() const {
			return time;
		}
	};
}
