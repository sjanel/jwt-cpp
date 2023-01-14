#ifndef STRING_TYPES_H
#define STRING_TYPES_H

namespace jwt {
#if __cplusplus >= 201703L

#include <string_view>

#define JWT_HAS_STRING_VIEW

	using string_constant = std::string_view;
	using string_view = std::string_view;

#else

#include <string>

	using string_constant = std::string;
	using string_view = const std::string&;

#endif
} // namespace jwt

#endif