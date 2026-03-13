#pragma once
#include <cstdint>
#include <string>

namespace dae {
	template <size_t length>
	struct sdbm_hash
	{
		consteval static unsigned int _calculate(const char* const text, unsigned int& value) {
			const unsigned int character = sdbm_hash<length - 1>::_calculate(text, value);
			value = character + (value << 6) + (value << 16) - value;
			return text[length - 1];
		}
		consteval static unsigned int calculate(const char* const text) {
			unsigned int value = 0;
			const auto character = _calculate(text, value);
			return character + (value << 6) + (value << 16) - value;
		}

		static unsigned int _calculateRT(const char* const text, unsigned int& value) {
			const unsigned int character = sdbm_hash<length - 1>::_calculate(text, value);
			value = character + (value << 6) + (value << 16) - value;
			return text[length - 1];
		}
		static unsigned int calculateRT(const char* const text) {
			unsigned int value = 0;
			const auto character = _calculateRT(text, value);
			return character + (value << 6) + (value << 16) - value;
		}
	};

	template <> struct sdbm_hash<1> {
		consteval static int _calculate(const char* const text, unsigned int&) { return text[0]; }
	};

	template <size_t N>
	consteval unsigned int make_sdbm_hash(const char(&text)[N]) {
		return sdbm_hash<N>::calculate(text);
	}


	inline unsigned int _calculateRT(const char* const text, size_t length, unsigned int& value)
	{
		if (length <= 1)
		{
			return text[0];
		}

		const unsigned int character = _calculateRT(text, length - 1, value);
		value = character + (value << 6) + (value << 16) - value;
		return text[length - 1];
	}

	inline unsigned int calculateRT(const char* const text, size_t length)
	{
		if (length == 0) return 0;

		unsigned int value = 0;
		const auto character = _calculateRT(text, length, value);
		return character + (value << 6) + (value << 16) - value;
	}

	inline unsigned int make_sdbm_hash_rt(const std::string& text)
	{
		return calculateRT(text.c_str(), text.length() + 1);
	}
}