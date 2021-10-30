#pragma once
#include "pch.h"

namespace Element {

	struct Resolution {
		uint32_t width, height;
	};

	constexpr Resolution Resolution_720 = Resolution{ 1280, 720 };
	constexpr Resolution Resolution_1080 = Resolution{ 1920, 1080 };
	constexpr Resolution Resolution_1440 = Resolution{ 2560, 1440 };
	constexpr Resolution Resolution_2160 = Resolution{ 3840, 2160 };
}
