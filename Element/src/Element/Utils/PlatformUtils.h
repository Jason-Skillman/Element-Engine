#pragma once

#include <string>

namespace Element {

	class FileDialog {
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="filter"></param>
		/// <returns>Return empty string if canceled.</returns>
		static std::string OpenFile(const char* filter);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="filter"></param>
		/// <returns>Return empty string if canceled.</returns>
		static std::string SaveFile(const char* filter);
	};
}
