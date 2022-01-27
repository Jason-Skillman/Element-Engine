#pragma once

#include <xhash>

namespace Element {

	class GUID {
	private:
		uint64_t guid;
	public:
		GUID();
		GUID(uint64_t guid);
		GUID(const GUID&) = default;

	public:
		operator uint64_t() const {
			return guid;
		}
	};
}

namespace std {

	/// <summary>This is required for Element::GUID to be used as a hash in a hash function. Eg. unordered_map<Element::GUID, int></summary>
	template<>
	struct hash<Element::GUID> {
		std::size_t operator()(const Element::GUID& guid) const {
			return hash<uint64_t>()((uint64_t)guid);
		}
	};
}
